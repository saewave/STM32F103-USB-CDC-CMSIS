#include "stm32f10x.h"
#include "usblib.h"

USBLIB_WByte _LineState;

int main(void)
{

    /* ============ 48 MHz ============= */
    RCC->CFGR &= ~RCC_CFGR_SW; // Change System Clock to HSI
    while ((RCC->CFGR & RCC_CFGR_SWS) != 0x00) {
        __NOP();
    };
    RCC->CR &= ~RCC_CR_PLLON; // Disable Pll
    while ((RCC->CR & RCC_CR_PLLON)) {
        __NOP();
    };
    RCC->CFGR &= ~0x3C0000;
    RCC->CFGR |= RCC_CFGR_PLLMULL4; // Set Pll Mul to 4
    RCC->CFGR |= RCC_CFGR_USBPRE;
    RCC->CFGR |= RCC_CFGR_PLLSRC;
    RCC->CR |= RCC_CR_PLLON;
    while (!(RCC->CR & RCC_CR_PLLON)) {
        __NOP();
    };
    RCC->CFGR |= RCC_CFGR_SW_1; // Change System Clock to PLL
    while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_1) {
        __NOP();
    };

    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_IOPBEN | RCC_APB2ENR_AFIOEN;
    /* ========= PB13 USB CONNECT ========= */
    /* PB12 - LED. Output PP */
    GPIOB->CRH |= GPIO_CRH_MODE12_0;
    GPIOB->CRH &= ~GPIO_CRH_CNF12;

    /* PB13 - USB EN. Output PP */
    GPIOB->CRH |= GPIO_CRH_MODE13_0;
    GPIOB->CRH &= ~GPIO_CRH_CNF13;
    
    /* =========== TIM1 ========== */
    RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
    TIM1->PSC = 1000 - 1;
    TIM1->ARR = 48000 - 1;
    TIM1->DIER |= TIM_DIER_UIE;
    //TIM1->CR1 = TIM_CR1_CEN | TIM_CR1_ARPE;
    NVIC_SetPriority(TIM1_UP_IRQn, 15);
    NVIC_EnableIRQ(TIM1_UP_IRQn);

    GPIOB->ODR &= ~GPIO_ODR_ODR13; //LOW
    for (int i = 0; i < 1000000; i++) {
        __NOP();
    };

    USBLIB_Init();
    GPIOB->ODR |= GPIO_ODR_ODR13; //UP

    while (1) {};
}

void TIM1_UP_IRQHandler() {
    TIM1->SR &= ~TIM_SR_UIF;
    GPIOB->ODR ^= GPIO_ODR_ODR12;

    if (_LineState.L) {      //App connected to the virtual port
        USBLIB_Transmit((uint16_t *)"Welcome to the club!\r\n", 22, 100);
    } else {
        USBLIB_Transmit((uint16_t *)"Bye bye!\r\n", 10, 100);
    }
}

void uUSBLIB_DataReceivedHandler(uint16_t *Data, uint16_t Length)
{
    USBLIB_Transmit(Data, Length, 100);
}

void uUSBLIB_LineStateHandler(USBLIB_WByte LineState)
{
    if (LineState.L) {      //App connected to the virtual port
        _LineState = LineState;
        TIM1->CR1 = TIM_CR1_CEN | TIM_CR1_ARPE;
    }
}
