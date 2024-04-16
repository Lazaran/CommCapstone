
#include <stdint.h>
#include <math.h>
#include "tm4c123gh6pm.h"
#include "RadioLib.h"
#include "UART_MACROS.h"

/*!*******************************************************************
    @brief Initializes and configures UART1 to send and receive based on input parameters
            Interrupt called on UART receive
    @param baud UART Baudrate
    @param word  UART Word Length
    @param stopUART # of Stop Bits
    @param parity UART Parity
    @param parity_toggle toggle parity for UART
**********************************************************************/
void Radio_Init(uint32_t baud, uint16_t word, uint16_t stop, uint16_t parity, uint16_t parity_toggle){

    // BRD Calculations
    float BRDFloat = (PLL_SPEED / (16 * baud));
    int IBRDVal = floor(BRDFloat);
    int FBRDVal = floor((BRDFloat - IBRDVal) * 64 + 0.5);
	
    // Activating the clocks for the GPIO and UART
    SYSCTL_RCGCGPIO_R  |= SYSCTL_RCGCGPIO_R1;  // Enable Port B Clock
    SYSCTL_RCGCUART_R  |= SYSCTL_RCGCUART_R1;	 // Enable UART1 Clock

    // Activating and configuring PortB GPIO
    GPIO_PORTB_DEN_R = 0x03;        // PB[0:1] Digital Enable
    GPIO_PORTB_DIR_R = 0x02;        // PB[1] Output | PB[0] Input
    GPIO_PORTB_AFSEL_R = 0x03;      // PB[0:1] Alternate Function Select
	GPIO_PORTB_PCTL_R = 0x11;       // PB[0:1] Port Control Select UART1
	
    // Activating and configuring UART1
	UART1_CTL_R &= 0xFFFE;           // Control | Disabling UART
	UART1_IBRD_R = IBRDVal;         // Integer Baud-Rate Divisor
	UART1_FBRD_R = FBRDVal;         // Fractional Baud-Rate Divisor
	UART1_LCRH_R = 0x00;            // Line Control | Parity Enable + FIFO Enable
	UART1_LCRH_R = 0x10 | word | stop | parity | parity_toggle;     // Line Control | FIFO Enable
	UART1_CC_R = 0x00;              // Clock Configuration | System Clock
	UART1_IFLS_R = 0x00;            // FIFO Level Trigger | RX >= 1/8 | TX <= 7/8

    // Enabling Interrupt
    UART5_IM_R = 0x0000;            // Clear Interrupt Mask
	UART1_ICR_R = 0xFF;             // Clear Interrupt Registers
	UART1_IM_R = 0x0010;            // Set Interrupt Mask | RX
	NVIC_EN0_R = 0x40;              // Enabling Interrupt for UART 1 | Vector 61 | CHANGE
	NVIC_PRI1_R = 0x600000;         // Setting UART 1 Interrupt Priority to 3 | CHANGE
    
	UART1_CTL_R = 0x301;            // Control | Enabling UART
}

/*!*******************************************************************
    @brief Sends an 8-bit code to the UART5 transmit FIFO
    @param data 8-bit code to send through UART
**********************************************************************/
void Radio_Tx(uint8_t data){
    // Wait for TX FIFO to not be full
    while((UART1_FR_R & UART_FR_TXFF)){};
    // Write data to UART TX FIFO
    UART1_DR_R = data;
};

/*!*******************************************************************
    @brief Reads and returns the UART5 Receive FIFO to get an 8-bit code
    @param data 8-bit code to received through UART
**********************************************************************/
uint8_t Radio_Rx(void){
	uint8_t data;
    // Wait for RX FIFO to not be full
	while((UART1_FR_R & 0x40) != 0){}; // DIFFERENCE THIS IS RX FULL, BT has RX EMPTY
    // Write UART RX FIFO to data
    data = (UART1_DR_R & 0xFF);
	return data; 
};

/*!*******************************************************************
    OLD FOR RADIO | CHANGING PORTF DIRECTLY
    @brief Interrupt for UART1, called when data received into RX FIFO
            Sends received code from UART to PORTF_DATA to control LED
**********************************************************************/
void UART1_Handler(void) {
    // Data received by UART
    if (UART1_MIS_R&0x0010){
        // Keeps Non-LED values, sets LED with RX values
        GPIO_PORTF_DATA_R = ((GPIO_PORTF_DATA_R&0xF1) | UART_RX());
        // Clear Interrupts
        UART1_ICR_R = 0xFFFF;
    };
};
