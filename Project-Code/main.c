#include "inc.h"
#include "init.h"

extern void ownerOptions();
extern void saveDefaultPassword();
const uint8 *Food[9] = {
                    "1-Big Tasty",
                    "2-Big Mac",
                    "3-Cheese Burger",
                    "4-French Fries",
                    "5-Cheesy Fries",
                    "6-Coleslaw",
                    "7-Pepsi",
                    "8-Cocacola",
                    "9-7UP",
                    };
const uint8 priceList[9]={50,60,65,15,20,10,10,10,10};
uint8 foodAvailable[9] = {0,0,0,0,0,0,0,0,0};
uint8 FoodQuantityCheck();
void showFood(int foodType);
int main()
{
    initSystem();   //LCD-UART- EEPROM init
    uint8 totalPrice=0;
    uint8 keys[3]={0,0,0};
    uint8 response;
    uint8 quantityChange;
    uint8 i;
    saveDefaultPassword();  
    ownerOptions();
    while(1)
    {
        while(FoodQuantityCheck()==1)
        {
            LCD_clearScreen();
            LCD_displayStringRowColumn(0,0,"Missing Items");
            LCD_displayStringRowColumn(1,0,"Please call");
            LCD_displayStringRowColumn(2,0,"the owner");
            while(Keypad_getPressedKey() != 'A')
            {
                _delay_ms(500);
            }
            ownerOptions();
        }
        LCD_clearScreen();
        LCD_displayStringRowColumn(0,6,"Hello!");
        LCD_displayStringRowColumn(1,0,"Press 0");
        LCD_displayStringRowColumn(2,0,"To Order!!");
        
        while(Keypad_getPressedKey() != 0)     //Looping forever until 1 is pressed
        {
            _delay_ms(500);
        }
        
        showFood(0);

        while(     (keys[0] != 1 || foodAvailable[0]==0) 
                && (keys[0] != 2 || foodAvailable[1]==0)
                && (keys[0] != 3 || foodAvailable[2]==0)
            ) //exiting this loop when we get a good choice and food is available
        {
            keys[0] = Keypad_getPressedKey();
            _delay_ms(500);
        }
        LCD_goToRowColumn(3,0);
        LCD_displayCharacter(keys[0] + ASCII_VALUE_BASE);
        LCD_displayString(" is Selected");
        totalPrice += priceList[keys[0]-1];
        _delay_ms(500);
        showFood(3);

        while(     (keys[1] != 4 || foodAvailable[3]==0) 
                && (keys[1] != 5 || foodAvailable[4]==0)
                && (keys[1] != 6 || foodAvailable[5]==0)
            ) //exiting this loop when we get a good choice and food is available
        {
            keys[1] = Keypad_getPressedKey();
            _delay_ms(500);
        }
        LCD_goToRowColumn(3,0);
        LCD_displayCharacter(keys[1] + ASCII_VALUE_BASE);
        LCD_displayString(" is Selected");
        totalPrice += priceList[keys[1]-1];
        _delay_ms(500);
        
        showFood(6);
        while(     (keys[2] != 7 || foodAvailable[6]==0) 
                && (keys[2] != 8 || foodAvailable[7]==0)
                && (keys[2] != 9 || foodAvailable[8]==0)
            ) //exiting this loop when we get a good choice and food is available
        {
            keys[2] = Keypad_getPressedKey();
            _delay_ms(500);
        }
        LCD_goToRowColumn(3,0);
        LCD_displayCharacter(keys[2] + ASCII_VALUE_BASE);
        LCD_displayString(" is Selected");
        totalPrice += priceList[keys[2]-1];
        _delay_ms(500);
        
        LCD_clearScreen();
        LCD_displayStringRowColumn(0,0,"Order completed");
        LCD_displayStringRowColumn(1,0,"Price is ");
        LCD_intgerToString(totalPrice);
        LCD_displayStringRowColumn(2,0,"1->OK 0->Cancel");
        response = Keypad_getPressedKey();
        while(response != 0 && response != 1) //exiting this loop when we get a good choice
        {
            response = Keypad_getPressedKey();
            _delay_ms(500);
        }
        if(response == 1)
        {
            //Send order using uart
            UART_sendString("------------------------\r\n");
            UART_sendString(Food[keys[0]-1]);
            UART_sendString("\r\n");
            UART_sendString(Food[keys[1]-1]);
            UART_sendString("\r\n");
            UART_sendString(Food[keys[2]-1]);
            UART_sendString("\r\n");
            UART_sendString("------------------------\r\n");
            LCD_clearScreen();
            LCD_displayStringRowColumn(0,0,"Order Accepted!");
            for(i=0;i<3;i++)
            {
                EEPROM_readByte(FOOD_ADDRESS+keys[i]-1, &quantityChange);
                _delay_ms(10);
                EEPROM_writeByte(FOOD_ADDRESS+keys[i]-1, quantityChange-1);
                _delay_ms(10);
            }
            _delay_ms(500);
        }
        else
        {
            LCD_clearScreen();
            LCD_displayStringRowColumn(0,0,"Order Cancelled");
            _delay_ms(500);
        }
        keys[0]=0;
        keys[1]=0;
        keys[2]=0;
        totalPrice=0;




        

        
    }
    return 0;
}

void showFood(int foodType)
{
    uint8 foodQuantity=0,i;
    LCD_clearScreen();
    for(i=foodType;i<foodType+3;i++)
    {
        EEPROM_readByte(FOOD_ADDRESS+i,&foodQuantity);
        if(foodQuantity != 0)
        {
            LCD_displayStringRowColumn(i-foodType,0,Food[i]);
        }
    }
    

}

uint8 FoodQuantityCheck()
{
    uint8 i;    uint8 savedQuantity;
    uint8 missingType=1;
    for(i=0;i<9;i++)
    {
        EEPROM_readByte(FOOD_ADDRESS+i,&savedQuantity);
        if(savedQuantity == 0 )
        {
            foodAvailable[i]=0;
        }
        else
        {
            foodAvailable[i]=1;
            missingType=0;
        }
        if(i==2 || i==5 || i==8)
        {
            if(missingType == 1)
            {
                return 1;
            }
            else
            {
                missingType=1;
            }
        }
    }
    return 0;


}