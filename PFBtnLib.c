#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "PFBtnLib.h"

/*!*******************************************************************
    @brief Interrupt for GPIO Port F, called when either SW1 or SW2 is pressed
Sends a code to the UART transmit FIFO to control a connected boards' LED
**********************************************************************/
void GPIOPortF_Handler(void){
    // SW1 Pressed
    if (GPIO_PORTF_MIS_R&0x10){
        UART_TX(0x00);     
    }
    // SW2 Pressed
    else if (GPIO_PORTF_MIS_R&0x01){
        UART_TX(0x0A);
    }
    // Clear Inteerupts
    GPIO_PORTF_ICR_R = 0xFF;

};

/*!*******************************************************************
    @brief Initializes and configures GPIO Port F to use onboard buttons and LED.
Interrupts called on SW1 or SW2 press
    @param data 8-bit code to send through UART
**********************************************************************/
void PortF_Init(void){ 
    // GPIO Port F Initialization
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R5;    // Enable Port F Clock
    GPIO_PORTF_LOCK_R = GPIO_LOCK_KEY;  // Unlock Port F Changes
    GPIO_PORTF_CR_R = 0x1F;             // PF[4:0] Commit Write On
    GPIO_PORTF_AMSEL_R = 0x00;          // PF[7:0] Analog Mode Off
    GPIO_PORTF_PCTL_R = 0x00000000;     // PF[0]   Port Control Off
    GPIO_PORTF_DIR_R = 0x0E;            // PF[3:1] Output | PF[4,0] Input 
    GPIO_PORTF_AFSEL_R = 0x00;          // PF[7:0] Alternate Function Off
    GPIO_PORTF_PUR_R = 0x11;            // PF[4&0] Pull-Up On
    GPIO_PORTF_DEN_R = 0x1F;            // PF[4:0] Digital Mode On

    //Enabling Interrupt   
    GPIO_PORTF_IM_R = 0x00;             // PF[7:0] Clear Interrupt Mask 
    GPIO_PORTF_IS_R &= ~0x11;           // PF[0,4] Edge Trigger Interrupt 
    GPIO_PORTF_IBE_R = 0x11;            // PF[0,4]   Trigger Both Edges
    GPIO_PORTF_ICR_R = 0xFF;            // Clear Interrupt Registers
    GPIO_PORTF_IM_R = 0x11;             // PF[0,4]  Set Interrupt Mask
    NVIC_PRI7_R = 0x00A00000;           // Setting Port F Interrupt Priority to 5
    NVIC_EN0_R = 0x40000000;            // Enabling Interrupt for Port F 
}

/*!*******************************************************************
    @brief Initializes and configures GPIO Port F to use onboard buttons and LED.
            Interrupts called on SW1 or SW2 press
    @param data 8-bit code to send through UART
**********************************************************************/
void PortF_Init(void){ 
    // GPIO Port F Initialization
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R5;    // Enable Port F Clock
    GPIO_PORTF_LOCK_R = GPIO_LOCK_KEY;  // Unlock Port F Changes
    GPIO_PORTF_CR_R = 0xFF;             // PF[7:0] Commit Write On
    GPIO_PORTF_AMSEL_R = 0x00;          // PF[7:0] Analog Mode Off
    GPIO_PORTF_PCTL_R = 0x00000000;     // PF[0]   Port Control Off
    GPIO_PORTF_DIR_R = 0x0E;            // PF[3:1] Output | PF[4,0] Input 
    GPIO_PORTF_AFSEL_R = 0x00;          // PF[7:0] Alternate Function Off
    GPIO_PORTF_PUR_R = 0x11;            // PF[4&0] Pull-Up On
    GPIO_PORTF_DEN_R = 0x1F;            // PF[4:0] Digital Mode On

    //Enabling Interrupt   
    GPIO_PORTF_IM_R = 0x00;             // PF[7:0] Clear Interrupt Mask 
    GPIO_PORTF_IS_R &= ~0x11;           // PF[0,4] Edge Trigger Interrupt 
    GPIO_PORTF_IBE_R = 0x11;            // PF[0,4]   Trigger Both Edges
    GPIO_PORTF_ICR_R = 0xFF;            // Clear Interrupt Registers
    GPIO_PORTF_IM_R = 0x11;             // PF[0,4]  Set Interrupt Mask
    NVIC_PRI7_R = 0x00A00000;           // Setting Port F Interrupt Priority to 5
    NVIC_EN0_R = 0x40000000;            // Enabling Interrupt for Port F 
}