// Standard Includes
#include <stdio.h>	
#include <stdint.h>

// Non-Physical Device Includes
#include "PLL.h"
#include "SysTick.h"
#include "ST7735.h"

// Physical Device Includes
#include "tm4c123gh6pm.h"
#include "BluetoothLib.h"
#include "RadioLib.h"
#include "UART_MACROS.h"

// Station Numbers
#define CONTROLLER_SELECT 1
#define REPEATER_SELECT 2
#define END_SELECT 3

/* Station Type 
    Use Station Number Macro to Select what
    station type the board running this code is
*/ 
#define STATION_TYPE 


void WaitForInterrupt(void);

/* ----| format_Print |----
Prints the input number and text to the LCD at the positions defined
Input:
    int16_t x   : X position for the string to draw at
    int16_t y   : Y position for the string to draw at
    int num     : Number to format into drawn string
    char* text  : String to format into drawn string
Output:
    None
*/
void format_Print(int16_t x, int16_t y, int num, char* text){
	char buffer[100];
    if (sizeof(text)/sizeof(text[0]) <= 10){
        // Format the inputs number and string for display
        snprintf(buffer, 100, "%s %d", text, num);
        // Draw formatted string to LCD
        ST7735_DrawString(x,y,buffer,ST7735_WHITE);
    };
};

/*----| main |----
Main code to be run by program
Input:
    None
Output:
	None
*/
int main(){
    // Initializations
    PLL_Init();
    SysTick_Init();
    ST7735_InitR(INITR_REDTAB);
    PortF_Init();
    Bluetooth_Init(BAUD_115200,WORD_8BIT,STOP_1BIT,PARITY_EVEN,PARITY_DISABLE);
    Radio_Init(BAUD_IN,WORD_IN,STOP_IN,PARITY_IN);

    while(1){
        WaitForInterrupt();
        // GPIO_PORTF_DATA_R = GREEN;
        switch(out_char){
            case 'N':
                GPIO_PORTF_DATA_R = GREEN;
                break;
            case 'F':
                GPIO_PORTF_DATA_R = BLUE;
                break;
            default:
                break;
        }
    };
};
