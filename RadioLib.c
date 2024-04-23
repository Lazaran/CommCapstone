/*!*******************************************************************
    @details Library to transceive UART communications via Radio
**********************************************************************/

// Library Includes
#include <stdint.h>
#include <math.h>
#include "tm4c123gh6pm.h"
#include "RadioLib.h"
#include "CommProtocol.h"

/*!*******************************************************************
    @brief Initializes and configures UART1 to send and receive based on input parameters
            Interrupt called on UART receive
    @param baud UART Baudrate
    @param word UART Word Length
    @param stop # of Stop Bits
    @param parity UART Parity
    @param parity_toggle toggle parity for UART
**********************************************************************/
void Radio_Init(uint32_t baud, uint16_t word, uint16_t stop, uint16_t parity, uint16_t parity_toggle){

    // BRD Calculations
    float RAD_BRDFloat = (CPU_SPEED / (16 * baud));
    int RAD_IBRDVal = floor(RAD_BRDFloat);
    int RAD_FBRDVal = floor((RAD_BRDFloat - RAD_IBRDVal) * 64 + 0.5);
	
    // Activating the clocks for the GPIO and UART
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R1;   // Enable Port B Clock
    SYSCTL_RCGCUART_R |= SYSCTL_RCGCUART_R1;   // Enable UART 1 Clock

    // Activating and configuring PortB GPIO
    GPIO_PORTB_DEN_R = 0x03;        // PB[0:1] Digital Enable
    GPIO_PORTB_DIR_R = 0x02;        // PB[1] Output | PB[0] Input
    GPIO_PORTB_AFSEL_R = 0x03;      // PB[0:1] Alternate Function Select
	GPIO_PORTB_PCTL_R = 0x11;       // PB[0:1] Port Control Select UART1
	
    // Activating and configuring UART1
	UART1_CTL_R &= 0xFFFE;          // Control | Disabling UART
	UART1_IBRD_R = RAD_IBRDVal;     // Integer Baud-Rate Divisor
	UART1_FBRD_R = RAD_FBRDVal;     // Fractional Baud-Rate Divisor
	UART1_LCRH_R = 0x00 | word | stop | parity | parity_toggle;     // Line Control | FIFO Enable
	UART1_CC_R = 0x00;              // Clock Configuration | System Clock
	UART1_IFLS_R = 0x00;            // FIFO Level Trigger | RX >= 1/8 | TX <= 7/8

    // Enabling Interrupt
    UART1_IM_R = 0x0000;            // Clear Interrupt Mask
	UART1_IM_R = 0x0010;            // Set Interrupt Mask | RX
	UART1_ICR_R = 0xFFFF;           // Clear Interrupt Registers
	NVIC_EN0_R = 0x40;              // Enabling Interrupt for UART 1 | Vector 6
	NVIC_PRI1_R = 0x600000;         // Setting UART 1 Interrupt Priority to 3
    
	UART1_CTL_R = 0x301;            // Control | Enabling UART
}

/*!*******************************************************************
    @brief Gets the received message from the UART RX register
**********************************************************************/
void UART1_Handler(void) {
    // Check if receiving
    if (UART1_RIS_R & 0x10){
        // Receive
        Now_Msg = Radio_Rx();
    }
    // Clear interrupt
    UART1_ICR_R = 0xFFFF;
};

/*!*******************************************************************
    @brief Sends an 1 to 4 byte code to the UART1 transmit FIFO
    @param data 1 to 4 byte code to send through UART
**********************************************************************/
void Radio_Tx(uint32_t data){
    // If FIFO enabled
    if (UART1_LCRH_R & 0x10){
        // While still data to transmit
        while (data > 0){
            // Write data to TX FIFO
            UART1_DR_R = data & 0xFF;
            // Wait for TX to stop transmitting
            while((UART1_FR_R & UART_FR_TXFE) == 0){};
            // Bitshift data right 1 byte
            data >>= 8;
        }
    }
    // If FIFO disabled
    else {
        // Write data to TX register
        // UART1_DR_R = data;
        UART1_DR_R = data;
        // Wait for TX to stop transmitting
        while((UART1_FR_R & UART_FR_TXFE) == 0){};
    }
};

/*!*******************************************************************
    @brief Receives a 1 to 4 byte code through UART1 Receive FIFO
    @param data 1 to 4 byte code to receive through UART
**********************************************************************/
uint32_t Radio_Rx(void){
	uint32_t data;
    // If FIFO enabled
    if (UART1_LCRH_R & 0x10){
        // While RX not empty
        while ((UART1_FR_R & UART_FR_RXFE) == 0){
            // Take 8 LSB of FIFO layer
            data = (UART1_DR_R & 0xFF);
            // Bitshift over 1 byte
            data <<= 8;
        }
    }
    // If FIFO disabled
    else {
        // Take byte from holding register
        data = (UART1_DR_R & 0xFF);
    }
    // Return RX data
    return data; 
};
