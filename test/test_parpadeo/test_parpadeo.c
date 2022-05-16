#include <parpadeo.h>
#include <unity.h>
#include <stm32f1xx_hal.h>
#include <stm32f1xx.h>
#include <stdbool.h>

enum {TIEMPO_ENCENDIDO = 500UL, TIEMPO_APAGADO = 500UL, ESPERA_MAXIMA = 600UL,TOLERANCIA = 1};

void SysTick_Handler(void)
{
    HAL_IncTick();
}

static GPIO_TypeDef miPuerto={0};
static bool pinResetPendiente(GPIO_TypeDef *puerto, uint32_t pin)
{
    return (puerto->BRR | (puerto->BSRR >> 16)) & pin;
}
static bool pinSetPendiente(GPIO_TypeDef *puerto, uint32_t pin)
{
    return (puerto->BSRR) & pin;
}
static void actualizaPin(GPIO_TypeDef *puerto)
{
    puerto->ODR = (puerto->ODR & !(puerto->BRR | (puerto->BSRR>>16))) | (puerto->BSRR & 0xFFFF);
    puerto->IDR =puerto->ODR;
    puerto->BSRR = puerto->BRR = 0;
}

void setUp(void)
{
    miPuerto = (GPIO_TypeDef){0};
}
void tearDown(void){}

static void debeConfigurarPuertoLed(void)
{
    enum{
        MI_PIN = GPIO_PIN_5,
        CRL_ESPERADO = GPIO_CRL_MODE5_1,
        CRH_ESPERADO = 0,
        BITSET_ESPERADO = MI_PIN
    };

    Parpadeo_init(&miPuerto, MI_PIN);
    
    TEST_ASSERT_EQUAL_HEX32(CRL_ESPERADO,miPuerto.CRL);
    TEST_ASSERT_EQUAL_HEX32(CRH_ESPERADO,miPuerto.CRH);
    TEST_ASSERT_MESSAGE(pinSetPendiente(&miPuerto,MI_PIN),"LED debe iniciar apagado");
}

static void debeEncenderLed(void)
{
    enum{MI_PIN=GPIO_PIN_5};
    uint32_t tiempoInicial;
    Parpadeo_init(&miPuerto, MI_PIN);
    actualizaPin(&miPuerto);
    tiempoInicial = HAL_GetTick();
    while(!pinResetPendiente(&miPuerto,MI_PIN) && (HAL_GetTick() - tiempoInicial) < ESPERA_MAXIMA)
        Parpadeo_loop();
    TEST_ASSERT(pinResetPendiente(&miPuerto,MI_PIN));
}

static void debeApagarLedLuegoDe500msEncendido(void)
{
    enum{MI_PIN=GPIO_PIN_5};
    uint32_t tiempoInicial;
    uint32_t transcurrido;
    Parpadeo_init(&miPuerto, MI_PIN);
    actualizaPin(&miPuerto);

    tiempoInicial = HAL_GetTick();
    while(!pinResetPendiente(&miPuerto,MI_PIN) && (HAL_GetTick() - tiempoInicial) < ESPERA_MAXIMA)
        Parpadeo_loop();
    actualizaPin(&miPuerto);

    tiempoInicial = HAL_GetTick();
    while(!pinSetPendiente(&miPuerto,MI_PIN) && (HAL_GetTick() - tiempoInicial) < ESPERA_MAXIMA)
        Parpadeo_loop();
    transcurrido = HAL_GetTick() - tiempoInicial;

    TEST_ASSERT(pinSetPendiente(&miPuerto,MI_PIN));
    TEST_ASSERT_LESS_OR_EQUAL_UINT32(TIEMPO_ENCENDIDO+TOLERANCIA,transcurrido);
    TEST_ASSERT_GREATER_OR_EQUAL_UINT32(TIEMPO_ENCENDIDO,transcurrido);
}

static void debeEncenderLedLuegoDe500msApagado(void)
{
    enum{MI_PIN=GPIO_PIN_5, TIEMPO_MAXIMO=2500, TIEMPO_ENCENDIDO=500};
    uint32_t tiempoInicial;
    uint32_t transcurrido;

    Parpadeo_init(&miPuerto, MI_PIN);
    actualizaPin(&miPuerto);

    tiempoInicial = HAL_GetTick();
    while(!pinResetPendiente(&miPuerto,MI_PIN) && (HAL_GetTick() - tiempoInicial) < TIEMPO_MAXIMO)
        Parpadeo_loop();
    actualizaPin(&miPuerto);

    tiempoInicial = HAL_GetTick();
    while(!pinSetPendiente(&miPuerto,MI_PIN) && (HAL_GetTick() - tiempoInicial) < TIEMPO_MAXIMO)
        Parpadeo_loop();
    actualizaPin(&miPuerto);

    tiempoInicial = HAL_GetTick();
    while(!pinResetPendiente(&miPuerto,MI_PIN) && (HAL_GetTick() - tiempoInicial) < TIEMPO_MAXIMO)
        Parpadeo_loop();
    transcurrido = HAL_GetTick() - tiempoInicial;

    TEST_ASSERT(pinResetPendiente(&miPuerto,MI_PIN));
    TEST_ASSERT_LESS_OR_EQUAL_UINT32(TIEMPO_ENCENDIDO+TOLERANCIA,transcurrido);
    TEST_ASSERT_GREATER_OR_EQUAL_UINT32(TIEMPO_ENCENDIDO,transcurrido);
}

int main(void)
{
    SystemCoreClockUpdate();
    HAL_Init();
    HAL_Delay(500);
    UNITY_BEGIN();
    RUN_TEST(debeConfigurarPuertoLed);
    RUN_TEST(debeEncenderLed);
    RUN_TEST(debeApagarLedLuegoDe500msEncendido);
    RUN_TEST(debeEncenderLedLuegoDe500msApagado);
    return UNITY_END();    
}