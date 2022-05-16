#include "unity_config.h"
#include <stm32f1xx.h>
#include <stm32f1xx_hal.h>
#include <stm32f1xx_hal_gpio.h>
#include <stm32f1xx_hal_usart.h>

static USART_HandleTypeDef husart1;

void unityOutputStart(void)
{
    SystemCoreClockUpdate();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_USART1_CLK_ENABLE();

    GPIO_InitTypeDef config = {.Pin=GPIO_PIN_9,
                               .Mode=GPIO_MODE_AF_PP,
                               .Speed=GPIO_SPEED_FREQ_MEDIUM};
    HAL_GPIO_Init(GPIOA,&config);
    
    husart1 = (USART_HandleTypeDef){
    .Instance=USART1,
    .Init={.Mode=USART_MODE_TX,
           .BaudRate=115200,
           .WordLength=USART_WORDLENGTH_8B,
           .Parity=USART_PARITY_NONE,
           .StopBits=USART_STOPBITS_1}};
    HAL_USART_Init(&husart1);
}
void unityOutputChar(char c)
{
    HAL_USART_Transmit(&husart1,(uint8_t*)&c,1,1000);
}
void unityOutputFlush(void)
{
}
void unityOutputComplete(void)
{
    HAL_USART_DeInit(&husart1);
}