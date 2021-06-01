 /******************************************************************************
 *
 * Module: Keypad
 *
 * File Name: keypad.h
 *
 * Description: header file for the keypad
 *
 * Author: Youssef Galal
 *
 *******************************************************************************/


#ifndef KEYPAD_H_
#define KEYPAD_H_

#include <avr/io.h>
#include "std_types.h"
#include "common_macros.h"
#include "micro_config.h"
#include <util/delay.h>

/*******************************************************************************
 *                      Preprocessor Macros                                    *
 *******************************************************************************/

/* Keypad configurations for number of rows and columns */
#define N_col 4
#define N_row 4
/* Keypad Port Configurations */
#define KEYPAD_PORT_IN PINA
#define KEYPAD_PORT_OUT PORTA
#define KEYPAD_PORT_DIR DDRA

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
uint8 Keypad_getPressedKey(void);


#endif /* KEYPAD_H_ */
