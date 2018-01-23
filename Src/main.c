#include "stm32f10x.h"
#include "main.h"
#include "usblib.h"

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

    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_IOPBEN;
    /* ========= PB13 USB CONNECT ========= */
    /* PB12 - LED. Output PP */
    GPIOB->CRH |= GPIO_CRH_MODE12_0;
    GPIOB->CRH &= ~GPIO_CRH_CNF12;

    /* PB13 - USB EN. Output PP */
    GPIOB->CRH |= GPIO_CRH_MODE13_0;
    GPIOB->CRH &= ~GPIO_CRH_CNF13;

    GPIOB->ODR &= ~GPIO_ODR_ODR13; //LOW
    for (int i = 0; i < 1000000; i++) {
        __NOP();
    };

    USBLIB_Init();
    //    GPIOB->ODR |= GPIO_ODR_ODR12;
    GPIOB->ODR |= GPIO_ODR_ODR13; //UP

    while (1) {
        //        USBLIB_HandleStatus();
    };
}
