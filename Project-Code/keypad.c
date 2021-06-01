 /******************************************************************************
 *
 * Module: Keypad
 *
 * File Name: keypad.c
 *
 * Description: source file for the keypad
 *
 * Author: Youssef Galal
 *
 *******************************************************************************/



#include "keypad.h"

#if (N_col == 3)

static uint8 KeyPad_4x3_adjustSwitchNumber(uint8 button_number);
#elif ( N_col == 4 )

static uint8 KeyPad_4x4_adjustSwitchNumber(uint8 button_number);
#endif

uint8 Keypad_getPressedKey(void)
{
	uint8 col,row;
	while(1)
	{
		for(col=0; col<N_col;col++)
		{
			KEYPAD_PORT_DIR = (0b00010000 << col); /* we open one column each time and close every column and the rows too*/

			KEYPAD_PORT_OUT = (~(0b00010000<<col)); /* we clear the selected column to make it LOW and enable pull-up resistors for rows pins */

			for(row=0; row<N_row; row++)
			{
				if(BIT_IS_CLEAR(KEYPAD_PORT_IN,row))
				{
					#if (N_col == 3)
					return KeyPad_4x3_adjustSwitchNumber((row*N_col)+1+col);
					#elif (N_col == 4 )
					return KeyPad_4x4_adjustSwitchNumber((row*N_col)+col+1);
					#endif
				}
			}
		}
	}
}
#if ( N_col == 3 )
static uint8 KeyPad_4x3_adjustSwitchNumber(uint8 button_number)
{
	switch (button_number)
	{
	case 10 : return '*';
				break;
	case 11 : return 0;
				break;
	case 12 : return '#';
				break;
	default : return button_number;
	}
}
#elif ( N_col == 4 )
static uint8 KeyPad_4x4_adjustSwitchNumber(uint8 button_number)
{
	switch(button_number)
	{
	case 1 : return 1 ;
			break;
	case 2 : return 2;
			break;
	case 3 : return 3;
			break;
	case 4 : return 'A' ;
			break;
	case 5 : return 4 ;
			break;
	case 6 : return 5 ;
			break;
	case 7 : return 6;
			break;
	case 8 : return 'B';
			break;
	case 9 : return 7;
			break;
	case 10 : return 8;
			break;
	case 11 : return 9;
			break;
	case 12 : return 'C';
			break;
	case 13 : return '*';
			break;
	case 14 : return 0;
			break;
	case 15 : return '#';
			break;
	case 16 : return 'D' ;
			break;
	default : return button_number;
	}
}

#endif
/*
 * 		4x4 KeyPad
 *		
 *      1 2 3 A
 *      4 5 6 B
 *      7 8 9 C
 * 		* 0 # D
 * 
 * 		4x3 KeyPad
 * 		1 2 3
 * 		4 5 6
 * 		7 8 9
 * 		* 0 #
 *
 */







