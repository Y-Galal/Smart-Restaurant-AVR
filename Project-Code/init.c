#include "init.h"
void initSystem()
{
   EEPROM_init();
    LCD_init();
    UART_init();
}
