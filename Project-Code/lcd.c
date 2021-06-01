 /******************************************************************************
 *
 * Module: LCD
 *
 * File Name: lcd.c
 *
 * Description: Lcd module
 *
 * Author: Youssef Galal
 *
 *******************************************************************************/

#include "lcd.h"

void LCD_init(void)
{
	LCD_CTRL_PORT_DIR |= (1<<E) | (1<<RW) | (1<<RS);
	#if (DATA_BITS_MODE == 4)
		#ifdef UPPER_PORT_PINS
		LCD_DATA_PORT_DIR |= 0xF0; /* Upper port pins are output pins  */
		#else
		LCD_DATA_PORT_DIR |= 0x0F /* Lower port pins are output pins*/
		#endif
		LCD_sendCommand(FOUR_BITS_DATA_MODE);
		LCD_sendCommand(TWO_LINE_LCD_FOUR_BIT_MODE);
	#elif (DATA_BITS_MODE == 8)
		LCD_DATA_PORT_DIR |= 0xFF; /* All pins are output pins  */
		LCD_sendCommand(TWO_LINE_LCD_EIGHT_BIT_MODE);
	#endif
	LCD_sendCommand(CURSOR_OFF); /* cursor off */
	LCD_sendCommand(CLEAR_COMMAND); /* clear LCD at the beginning */
}

void LCD_sendCommand(uint8 command)
{
	CLEAR_BIT(LCD_CTRL_PORT,RS);
	CLEAR_BIT(LCD_CTRL_PORT,RW);
	_delay_ms(1);
	SET_BIT(LCD_CTRL_PORT,E);
	_delay_ms(1);
	#if (DATA_BITS_MODE == 4)
		#ifdef UPPER_PORT_PINS
		LCD_DATA_PORT = (LCD_DATA_PORT & 0x0F) | (command & 0xF0);
		#else
		LCD_DATA_PORT = (LCD_DATA_PORT & 0xF0) | ( (command & 0xF0) >> 4 );
		#endif
		_delay_ms(1);
		CLEAR_BIT(LCD_CTRL_PORT,E);
		_delay_ms(1);
		SET_BIT(LCD_CTRL_PORT,E);

		#ifdef UPPER_PORT_PINS
		LCD_DATA_PORT = (LCD_DATA_PORT & 0x0F) | ( (command & 0xF0) << 4 );
		#else
		LCD_DATA_PORT = (LCD_DATA_PORT & 0xF0) | (command & 0x0F);
		#endif
		_delay_ms(1);
		CLEAR_BIT(LCD_CTRL_PORT,E);
		_delay_ms(1);
	#elif (DATA_BITS_MODE == 8)
		LCD_DATA_PORT = command;
		_delay_ms(1);
		CLEAR_BIT(LCD_CTRL_PORT,E);
		_delay_ms(1);
	#endif
}

void LCD_displayCharacter(uint8 data)
{
	SET_BIT(LCD_CTRL_PORT,RS); /*RS=1 for data mode*/
	CLEAR_BIT(LCD_CTRL_PORT,RW); /*RW=0 to write data */
	_delay_ms(1);
	SET_BIT(LCD_CTRL_PORT,E);
	_delay_ms(1);
#if (DATA_BITS_MODE == 4)
	#ifdef UPPER_PORT_PINS
	LCD_DATA_PORT = (LCD_DATA_PORT & 0x0F) | (data & 0xF0);
	#else
	LCD_DATA_PORT = (LCD_DATA_PORT & 0xF0) | ( (data & 0xF0) >> 4 );
	#endif
	_delay_ms(1);
	CLEAR_BIT(LCD_CTRL_PORT,E);
	_delay_ms(1);
	SET_BIT(LCD_CTRL_PORT,E);

	#ifdef UPPER_PORT_PINS
	LCD_DATA_PORT = (LCD_DATA_PORT & 0x0F) | ( (data & 0xF0) << 4 );
	#else
	LCD_DATA_PORT = (LCD_DATA_PORT & 0xF0) | (data & 0x0F);
	#endif
	_delay_ms(1);
	CLEAR_BIT(LCD_CTRL_PORT,E);
	_delay_ms(1);
#elif (DATA_BITS_MODE == 8)
	LCD_DATA_PORT = data;
	_delay_ms(1);
	CLEAR_BIT(LCD_CTRL_PORT,E);
	_delay_ms(1);
#endif
}

void LCD_displayString(const char *Str)
{
	uint8 i=0;
	while(Str[i]!='\0')
	{
		LCD_displayCharacter(Str[i]);
		i++;
	}
}

void LCD_goToRowColumn(uint8 row,uint8 col)
{
	uint8 Address;
	switch(row)
	{
	case 0 :
		Address = col;
		break;
	case 1 :
		Address = col + 0x40;
		break;
	case 2 :
		Address = col + 0x10;
		break;
	case 3 :
		Address = col + 0x50;
		break;
	}
	/* to write to a specific address in the LCD
	 * we need to apply the corresponding command 0b10000000+Address */
	LCD_sendCommand(Address |SET_CURSOR_LOCATION);
}

void LCD_displayStringRowColumn(uint8 row,uint8 col, const char *Str)
{
	LCD_goToRowColumn(row,col);
	LCD_displayString(Str);
}

void LCD_clearScreen(void)
{
	LCD_sendCommand(CLEAR_COMMAND);
}

void LCD_intgerToString(uint8 data)
{
	char buff[16];
	itoa(data,buff,10); /* fucntion takes data in decimal value base10, puts it in buff and converts it string*/
	LCD_displayString(buff);
}
