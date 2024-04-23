/*!*******************************************************************
    @details Library to transceive UART communications via Bluetooth
**********************************************************************/

// Library Includes
#include <math.h>
#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "BluetoothLib.h"
#include "CommProtocol.h"

/*!*******************************************************************
    @brief Initializes and configures UART5 to send and receive based on input parameters
            Interrupt called on UART receive
    @param baud UART Baudrate
    @param word UART Word Length
    @param stop # of Stop Bits
    @param parity UART Parity
    @param parity_toggle toggle parity for UART
**********************************************************************/
void Bluetooth_Init(uint32_t baud, uint16_t word, uint16_t stop, uint16_t parity, uint16_t parity_toggle){

    // BRD Calculations
    float BT_BRDFloat = (CPU_SPEED / (16 * baud));
    int BT_IBRDVal = floor(BT_BRDFloat);
    int BT_FBRDVal = floor((BT_BRDFloat - BT_IBRDVal) * 64 + 0.5);

    // Activating the clocks for the GPIO and UART
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R4;    // Enable Port E Clock
    SYSCTL_RCGCUART_R |= SYSCTL_RCGCUART_R5;    // Enable UART 5 Clock

    // Activating and configuring PortE GPIO
    GPIO_PORTE_DEN_R = 0x30;            // PE[4:5] Digital Enable
    GPIO_PORTE_DIR_R = 0x20;            // PE[5] Output | PE[4] Input
    GPIO_PORTE_AFSEL_R = 0x30;          // PE[4:5] Alternate Function Select
    GPIO_PORTE_PCTL_R = 0x110000;       // PE[4:5] Port Control Select UART5

    // Activating and configuring UART5
    UART5_CTL_R &= 0xFFFE;              // Control | Disabling UART
    UART5_IBRD_R = BT_IBRDVal;          // Integer Baud-Rate Divisor
    UART5_FBRD_R = BT_FBRDVal;          // Fractional Baud-Rate Divisor
    UART5_LCRH_R = 0x00 | word | stop | parity | parity_toggle;		// Line Control | FIFO Enable
    UART5_CC_R = 0x00;                  // Clock Configuration | System Clock
    UART5_IFLS_R = 0x00;                // FIFO Level Trigger | RX >= 1/8 | TX <= 7/8

    // Enabling Interrupt
    UART5_IM_R = 0x0000;                // Clear Interrupt Mask
    UART5_IM_R = 0x0010;                // Set Interrupt Mask | RX
    UART5_ICR_R = 0xFFFF;               // Clear Interrupt Registers
    NVIC_EN1_R = 0x20000000;            // Enabling Interrupt for UART 5 | Vector 61
    NVIC_PRI15_R = 0x8000;              // Setting UART 5 Interrupt Priority to 4

    UART5_CTL_R = 0x0301;              // Control | Enabling UART
};

/*!*******************************************************************
    @brief Gets the received message from the UART RX register
**********************************************************************/
void UART5_Handler(void) {
    // Check if receiving
    if (UART5_RIS_R & 0x10){
        // Receive
        Now_Msg = Bluetooth_Rx();
    }
    // Clear interrupt
    UART5_ICR_R = 0xFFFF;
};

/*!*******************************************************************
    @brief Sends an 1 to 4 byte code to the UART5 transmit FIFO
    @param data 1 to 4 byte code to send through UART
**********************************************************************/
void Bluetooth_Tx(uint32_t data){
    // If FIFO enabled
    if (UART5_LCRH_R & 0x10){
        // While still data to transmit
        while (data > 0){
            // Write data to TX FIFO
            UART5_DR_R = data & 0xFF;
            // Wait for TX to stop transmitting
            while((UART5_FR_R & UART_FR_TXFE) == 0){};
            // Bitshift data right 1 byte
            data >>= 8;
        }
    }
    // If FIFO disabled
    else {
        // Write data to TX register
        UART5_DR_R = data & 0xFF;
        // Wait for TX to stop transmitting
        while((UART5_FR_R & UART_FR_TXFE) == 0){};
    }
};

/*!*******************************************************************
    @brief Receives a 1 to 4 byte code through UART5 Receive FIFO
    @param data 1 to 4 byte code to receive through UART
**********************************************************************/
uint32_t Bluetooth_Rx(void){
	uint32_t data;
    // If FIFO enabled
    if (UART5_LCRH_R & 0x10){
        // While RX not empty
        while ((UART5_FR_R & UART_FR_RXFE) == 0){
            // Take 8 LSB of FIFO layer
            data = (UART5_DR_R & 0xFF);
            // Bitshift over 1 byte
            data <<= 8;
        }
    }
    // If FIFO disabled
    else {
        // Take byte from holding register
        data = (UART5_DR_R & 0xFF);
    }
    // Return RX data
    return data; 
};
