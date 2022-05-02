#ifndef PARPADEO_H
#define PARPADEO_H
#include <stm32f1xx_hal.h>

void Parpadeo_init(GPIO_TypeDef *puertoLed,uint32_t pinLed);

void Parpadeo_loop(void);

#endif
