#include "inc.h"

extern const uint8 *Food[9];
uint8 ownerEnterPassword();
 

void ownerOptions()
{
    uint8 countFlag = 0;
    uint8 valueToSave = 0;
    uint8 password[4];
    uint8 savedPassword[4];
    uint8 i = 0;
    uint8 response;
    uint8 savedData;
    uint8 notEqual=0;
    while(ownerEnterPassword());

    LCD_clearScreen();
    LCD_displayStringRowColumn(0, 0, "Hello Owner");
    LCD_displayStringRowColumn(1, 0, "1-Edit Password");
    LCD_displayStringRowColumn(2, 0, "2-Add Food");
    LCD_displayStringRowColumn(3, 0, "3-Order Mode");

    response = Keypad_getPressedKey();
    while (response != 1 && response != 2 && response != 3)
    {
        _delay_ms(500);
        response = Keypad_getPressedKey();
    }
    if (response == 1)
    {
        LCD_clearScreen();
        LCD_displayStringRowColumn(0,0,"Old Password");
        LCD_goToRowColumn(1,0);
        countFlag = 0;
        while (countFlag != 4)
        {
            _delay_ms(500);
            response = Keypad_getPressedKey();
            if (response >= 0 && response <= 9)
            {
                password[countFlag] = response;
                countFlag++;
                LCD_displayCharacter('*');
            }
            
        }
        for(i=0;i<4;i++)
        {
            EEPROM_readByte(PASSWORD_ADDRESS,&(savedPassword[i]));
            if( password[i] != savedPassword[i] )
            {
                notEqual=1;
                break;

            }
        }
        if(notEqual == 1)
        {
            LCD_clearScreen();
            LCD_displayStringRowColumn(0,0,"Passwords don't");
            LCD_displayStringRowColumn(1,0,"match!!");
            _delay_ms(500);
        }
        else
        {
            _delay_ms(500);
            LCD_clearScreen();
            LCD_displayStringRowColumn(0,0,"New Password");
            LCD_goToRowColumn(1,0);
            countFlag = 0;
            while (countFlag != 4)
            {
                _delay_ms(500);
                response = Keypad_getPressedKey();
                if (response >= 0 && response <= 9)
                {
                    password[countFlag] = response;
                    countFlag++;
                    LCD_displayCharacter('*');
                }
                
            }
            for (i = 0; i < 4; i++)
            {
                EEPROM_writeByte(PASSWORD_ADDRESS + i, password[i]); //Saving PW
                _delay_ms(10);
            }
            LCD_clearScreen();
            LCD_displayStringRowColumn(0,0,"Password Saved");
            EEPROM_writeByte(ADDRESS_OF_FIRST,99);
            _delay_ms(500);
        }
    }
    else if(response ==2) //add food
    {
        LCD_clearScreen();
        countFlag = 0;
        for (i = 0; i < 9; i++) //Getting and saving food quantity
        {
            LCD_displayStringRowColumn(0, 0, "Enter Quantity");
            LCD_displayStringRowColumn(1, 0, Food[i]);
            LCD_goToRowColumn(2, 0);
            valueToSave=0;
            while (countFlag != 2)
            {
                _delay_ms(500);
                response = Keypad_getPressedKey();
                if ( (response >= 0 && response <= 9) || response == '#')
                {
                    //LCD_displayCharacter(response + ASCII_VALUE_BASE);
                    if(response == '#')
                    {
                        valueToSave/=10;
                        break;
                    }
                    else
                    {
                        LCD_intgerToString(response);
                        if (countFlag == 0)
                        {
                            valueToSave = 10 * response;
                            countFlag++;
                        }
                        else if (countFlag == 1)
                        {
                            valueToSave += response;
                            countFlag++;
                        }
                    }
                }
                
            }
            countFlag = 0;

            EEPROM_writeByte(FOOD_ADDRESS + i, valueToSave);
            LCD_displayStringRowColumn(3, 0, " Saved!");
            _delay_ms(500);
            LCD_clearScreen();
        }
    }
    else if (response == 3) // go to order
    {
        return;
    }
   
}

void saveDefaultPassword()
{
    uint8 firstTime;
    EEPROM_readByte(ADDRESS_OF_FIRST,&firstTime);
    _delay_ms(10);
    if(firstTime != 99)
    {    
        uint8 i=0;
        for(i=0;i<4;i++)
        {
            EEPROM_writeByte(PASSWORD_ADDRESS+i,0);
            _delay_ms(10);
        }
        for(i=0;i<10;i++)
        {
            EEPROM_writeByte(FOOD_ADDRESS+i,0);
            _delay_ms(10);

        }
        EEPROM_writeByte(ADDRESS_OF_FIRST,99);
    }
}

uint8 ownerEnterPassword()
{
    uint8 pwFlag=0;
    uint8 key;
    uint8 i;
    uint8 notEqual=0;
    uint8 password[4], savedPassword[4];
    LCD_clearScreen();
    LCD_displayStringRowColumn(0,0,"Enter Password");
    _delay_ms(1000);
    LCD_goToRowColumn(1,0);
    while (pwFlag != 4)
    {
        _delay_ms(500);
        key = Keypad_getPressedKey();
        if (key >= 0 && key <= 9)
        {
            password[pwFlag] = key;
            pwFlag++;
            LCD_displayCharacter('*');
        }  
    }
    for(i=0;i<4;i++)
    {
        EEPROM_readByte(PASSWORD_ADDRESS+i,&(savedPassword[i]));
        if( password[i] != savedPassword[i] )
        {
            notEqual=1;
            break;

        }
    }
    if(notEqual == 1)
    {
        _delay_ms(500);
        LCD_clearScreen();
        LCD_displayStringRowColumn(0,0,"Password is");
        LCD_displayStringRowColumn(1,0,"incorrect!!");
        _delay_ms(500);
        return 1;
    }
    else
    {
        _delay_ms(500);
        LCD_clearScreen();
        LCD_displayStringRowColumn(0,0,"Passwords is");
        LCD_displayStringRowColumn(1,0,"correct");
        _delay_ms(500);
        return 0;
    }
}