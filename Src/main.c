#include "stm32f10x.h"
#include "usblib.h"

#define USART_BUF_LEN 64
#define APB1CLK 72000000UL
#define APB2CLK 72000000UL
#define BAUDRATE 115200UL

#define UART_DIV_SAMPLING16(_PCLK_, _BAUD_) (((_PCLK_)*25) / (4 * (_BAUD_)))
#define UART_DIVMANT_SAMPLING16(_PCLK_, _BAUD_) (UART_DIV_SAMPLING16((_PCLK_), (_BAUD_)) / 100)
#define UART_DIVFRAQ_SAMPLING16(_PCLK_, _BAUD_) (((UART_DIV_SAMPLING16((_PCLK_), (_BAUD_)) - (UART_DIVMANT_SAMPLING16((_PCLK_), (_BAUD_)) * 100)) * 16 + 50) / 100)

#define UART_BRR_SAMPLING16(_PCLK_, _BAUD_) (((UART_DIVMANT_SAMPLING16((_PCLK_), (_BAUD_)) << 4) +    \
                                              (UART_DIVFRAQ_SAMPLING16((_PCLK_), (_BAUD_)) & 0xF0)) + \
                                             (UART_DIVFRAQ_SAMPLING16((_PCLK_), (_BAUD_)) & 0x0F))

uint8_t Usart1Buf[USART_BUF_LEN];
uint8_t Usart2Buf[USART_BUF_LEN];

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

    GPIOB->ODR &= ~GPIO_ODR_ODR13; //LOW
    for (int i = 0; i < 1000000; i++) {
        __NOP();
    };

    USBLIB_Init();
    GPIOB->ODR |= GPIO_ODR_ODR13; //UP

    while (1) {
        //        USBLIB_HandleStatus();
    };
}

void uUSBLIB_DataReceivedHandler(uint16_t *Data, uint16_t Length)
{
    USBLIB_Transmit(Data, Length, 100);
}
