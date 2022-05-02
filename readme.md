# Proyecto de ejemplo PlatformIO/CubeMX

Proyecto de PlatformIO partiendo de la base de un proyecto CubeMx.

Incluye configuración para ejecutar pruebas (modificar test_port en platformio.ini según sea necesario).

Ejemplo de parpadeo de LED, con pruebas de aceptación.

Código generado por CubeMx en Core/Inc y Core/Src

# Nota

Makefile, startup_stm32f103xb.s y STM32F103C8Tx_FLASH.ld en el directorio raíz *son ignorados por PlatformIO*. En su lugar se emplean los ubicados dentro de la carpeta `.platformio/packages/framework-stm32cubef1/Drivers/CMSIS/Device/ST/STM32F1xx/Source/Templates/gcc/startup_stm32f103xb.S` y `.platformio/platforms/ststm32/ldscripts/stm32f103x8.ld`
