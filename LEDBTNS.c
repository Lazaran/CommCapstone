// Library Includes
#include <stdio.h>
#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "LEDBTNS.h"
#include "CommProtocol.h"

/*!*******************************************************************
    @brief Enables the Tiva's Onboard buttons at Port F
**********************************************************************/
void LEDBTNS_Init(void){
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R5;    // Enable Port F Clock
    
    GPIO_PORTF_LOCK_R = 0x4C4F434B;     // Lock for Port F
    GPIO_PORTF_CR_R = 0x1F;             // PF[0:4] Commit Write On
    GPIO_PORTF_AMSEL_R = 0x00;          // PF[0:7] Analog Mode Disable
    GPIO_PORTF_PCTL_R = 0x00000000;     // PF[0] Port Control Disable
    GPIO_PORTF_DIR_R = 0x0E;            // PF[1:3] Output | PF[0&4] Input 
    GPIO_PORTF_AFSEL_R = 0x00;          // PF[0:7] Alternate Function Select
    GPIO_PORTF_PUR_R = 0x11;            // PF[0&4] Pull-Up Enable
    GPIO_PORTF_DEN_R = 0x1F;            // PF[0:4] Digital Enable

    // Interrupt Enable
    GPIO_PORTF_IM_R = 0x00;             // Mask Interrupts
    GPIO_PORTF_IM_R = 0x11;             // PF[0&4] Unmask Interrupts
    GPIO_PORTF_IS_R = 0x00;             // PF[0:4] Edge Trigger Interrupt 
    GPIO_PORTF_IEV_R = 0x00;            // PF[0] Trigger Falling Edge | WHATEVER MAKES BUTTON DOWNPRESS WORK
    GPIO_PORTF_ICR_R = 0xFF;            // PF[0:7] Clear Interrupt Register
    NVIC_EN0_R = 0x40000000;            // Enabling Interrupt for Port F | Vector 30
    NVIC_PRI7_R = 0x200000;             // Setting Port F Interrupt Priority to 2
}

/*!*******************************************************************
    @brief Enables GPTM Timer 0A
**********************************************************************/
void FlashTimer_Init(void){
    SYSCTL_RCGCTIMER_R |= SYSCTL_RCGCTIMER_R0;      // Enable Timer 0 Clock

    // TIMER ENABLE
    TIMER0_CTL_R = 0x0000;          // Control: Disable timer
    TIMER0_CFG_R = 0x00000000;      // Configuration: Set 32-bit Timer
    TIMER0_TAMR_R = 0x0002;         // Timer A Mode: Set Periodic Timer
    TIMER0_TAILR_R = 0x02625A00;    // Interval Load: Load value: 40,000,000

    // Interrupt Enable
    TIMER0_IMR_R = 0x00;            // Mask Interrupts            
    TIMER0_IMR_R = 0x01;            // Unmask Timer 0A Time-out Interrupt
    TIMER0_ICR_R = 0xFFFFF;         // Clear Interrupt Register
    NVIC_EN0_R = 0x00080000;        // Enabling Interrupt for Timer 0A | Vector 19
    NVIC_PRI4_R = 0xA0000000;       // Setting Timer 0A Interrupt Priority to 5
}

/*!*******************************************************************
    @brief Interrupt handler for Timer 0A timeout
            SW1 : LED On | Transmit to App
            SW2 : LED Off | Transmit to App
**********************************************************************/
void Timer0A_Handler(void){
    // If Emergency mode is active
    if (EmergencyModeFlag == true){
        // If the timer has hit timeout
        if (TIMER0_RIS_R == TIMER_RIS_TATORIS){
            // Flash LED
            FlashLED(STATION_LED);
        }
    }
    // Clearinterrupt
    TIMER0_ICR_R = 0xFFFFF;
};

/*!*******************************************************************
    @brief Interrupt handler for Port F changing
            SW1 : LED On | Transmit to App
            SW2 : LED Off | Transmit to App
**********************************************************************/
void GPIOPortF_Handler(void){
    // If Emergency mode is inactive
    if (EmergencyModeFlag == false){
        // Check if SW2 pressed
        if (GPIO_PORTF_RIS_R & SW2_REG){
            // Turn off the LED
            Now_Msg = RW_WRITE | MSG_OFF | STATION_ADDR;
        }
        // Check if SW1 pressed
        else if (GPIO_PORTF_RIS_R & SW1_REG){
            // Turn on the LED
            Now_Msg = RW_WRITE | MSG_ON | STATION_ADDR;
        };
    }
    // Clear interrupt
    GPIO_PORTF_ICR_R = 0xFF;
};

/*!*******************************************************************
    @brief Sets the LED range of Port F
    @param data Hex code for ORing the LED
**********************************************************************/
void SetLED(uint8_t data){
    // Set LED with data
    GPIO_PORTF_DATA_R = (data & 0xE);      
};

/*!*******************************************************************
    @brief XORs the LED range of Port F
    @param data Hex code for XORing the LED
**********************************************************************/
void FlashLED(uint8_t data){
    // XOR LED with data
    GPIO_PORTF_DATA_R ^= (data & 0xE);      
};