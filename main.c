/*!*******************************************************************
    @details DESCRIPTION OF THE LAB
**********************************************************************/

// Standard Includes
#include <stdio.h>	
#include <stdint.h>
#include <stdbool.h>

// Non-Physical Device Includes
#include "PLL.h"
#include "SysTick.h"
#include "ST7735.h"

// Physical Device Includes
#include "tm4c123gh6pm.h"
#include "BluetoothLib.h"
#include "RadioLib.h"
#include "LEDBTNS.h"
#include "CommProtocol.h"

// Messages being compared
volatile uint32_t Now_Msg = 0;
volatile uint32_t Old_Msg = 0;
volatile bool EmergencyModeFlag = false;

void EnableInterrupts(void);
void WaitForInterrupt(void);

/*!*******************************************************************
    @brief Prints the input number and text to the LCD at the positions defined
    @param x X position for the string to draw at
    @param y Y position for the string to draw at
    @param num Number to format into drawn string
    @param text String to format into drawn string
**********************************************************************/
void format_Print(int16_t x, int16_t y, int num, char* text){
	char buffer[100];
    if (sizeof(&text)/sizeof(text[0]) <= 10){
        // Format the inputs number and string for display
        snprintf(buffer, 100, "%s %d", text, num);
        // Draw formatted string to LCD
        ST7735_DrawString(x,y,buffer,ST7735_WHITE);
    };
};

/*!*******************************************************************
    @brief Main code to be run by program
**********************************************************************/
int main(){
    // Initializations
    PLL_Init();
		SysTick_Init();
    //ST7735_InitR(INITR_REDTAB);
    LEDBTNS_Init();
    FlashTimer_Init();
    Radio_Init(BAUD_9600,WORD_8BIT,STOP_1BIT,PARITY_ODD,PARITY_ENABLE);
    Bluetooth_Init(BAUD_115200,WORD_8BIT,STOP_1BIT,PARITY_EVEN,PARITY_DISABLE);
    EnableInterrupts();

    //ST7735_DrawString(0,0,"< Hello",ST7735_WHITE);
    while(1){
				SysTick_Wait10ms(100);
        // Wait for system to receive from radio/bluetooth or manually transmit from buttons
        //WaitForInterrupt();

        // Check if received message is a duplicate
        if (DuplicateCheck() == true) {continue;}
        else {Old_Msg = Now_Msg;};

        // Check if Message is Emergency
        if (EmergencyCheck() == true) {continue;};

        // Check if Message is Broadcast
        if (BroadcastCheck() == true) {continue;};

        // Check if Message is Station-Specific
        if (StationCheck() == true) {continue;};

        // If message passed all checks, send it along
        if (FinalForward() == true) {continue;};
    };
};


