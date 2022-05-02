#include <parpadeo.h>

static struct Parpadeo
{
    GPIO_TypeDef *puertoLed;
    uint32_t pinLed;
}self;

enum {MEDIO_PERIODO = 500UL};

static void initPuerto(void)
{
    GPIO_InitTypeDef init = {
        .Mode=GPIO_MODE_OUTPUT_PP,
        .Speed=GPIO_SPEED_FREQ_LOW,
        .Pin=self.pinLed
    };
    HAL_GPIO_WritePin(self.puertoLed,self.pinLed,GPIO_PIN_SET);
    HAL_GPIO_Init(self.puertoLed,&init);
}

void Parpadeo_init(GPIO_TypeDef *puertoLed,uint32_t pinLed)
{
    self.puertoLed=puertoLed;
    self.pinLed=pinLed;
    initPuerto();
}

void Parpadeo_loop(void)
{
    HAL_Delay(MEDIO_PERIODO);
    HAL_GPIO_TogglePin(self.puertoLed,self.pinLed);
}
