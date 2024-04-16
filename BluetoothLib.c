
#include <math.h>
#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "BluetoothLib.h"
#include "UART_MACROS.h"

//yooooooo
//yaaaaa
/*!*******************************************************************
    @brief Initializes and configures UART5 to send and receive based on input parameters
            Interrupt called on UART receive
    @param baud UART Baudrate
    @param word  UART Word Length
    @param stopUART # of Stop Bits
    @param parity UART Parity
    @param parity_toggle toggle parity for UART
**********************************************************************/
void Bluetooth_Init(uint32_t baud, uint16_t word, uint16_t stop, uint16_t parity, uint16_t parity_toggle){

    // BRD Calculations
    float BRDFloat = (PLL_SPEED / (16 * baud));
    int IBRDVal = floor(BRDFloat);
    int FBRDVal = floor((BRDFloat - IBRDVal) * 64 + 0.5);

    // Activating the clocks for the GPIO and UART
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R4;    // Enable Port E Clock
    SYSCTL_RCGCUART_R |= SYSCTL_RCGCUART_R5;    // Enable UART 5 Clock

    // Activating and configuring PortE GPIO
    GPIO_PORTE_DEN_R = 0x30;            // PE[4:5] Digital Enable
    GPIO_PORTE_DIR_R = 0x20;            // PE[5] Output | PE[4] Input
    GPIO_PORTE_AFSEL_R = 0x30;          // PE[4:5] Alternate Function Select
    GPIO_PORTE_PCTL_R = 0x00110000;     // PE[4:5] Port Control Select UART5

    // Activating and configuring UART5
    UART5_CTL_R &= 0xFFFE;              // Control | Disabling UART
    UART5_IBRD_R = IBRDVal;             // Integer Baud-Rate Divisor
    UART5_FBRD_R = FBRDVal;             // Fractional Baud-Rate Divisor
    UART5_LCRH_R = 0x00;                // Line Control | Parity Enable + FIFO Enable
    UART5_LCRH_R = 0x10 | word | stop | parity | parity_toggle;		// Line Control | FIFO Enable
    UART5_CC_R = 0x00;                   // Clock Configuration | System Clock
    UART5_IFLS_R = 0x00;                // FIFO Level Trigger | RX >= 1/8 | TX <= 7/8

    // Enabling Interrupt
    UART5_IM_R = 0x0000;                // Clear Interrupt Mask
    UART5_ICR_R = 0xFFFF;               // Clear Interrupt Registers
    UART5_IM_R = 0x0010;                // Set Interrupt Mask | RX
    NVIC_EN1_R = 0x20000000;            // Enabling Interrupt for UART 5 | Vector 61
    NVIC_PRI15_R = 0x00006000;          // Setting UART 5 Interrupt Priority to 3

    UART5_CTL_R = 0x0301;              // Control | Enabling UART
};

/*!*******************************************************************
    @brief Sends an 8-bit code to the UART5 transmit FIFO
    @param data 8-bit code to send through UART
**********************************************************************/
void Bluetooth_Tx(uint8_t data){
    // Wait for TX FIFO to not be full
    while((UART5_FR_R & UART_FR_TXFF)){};
    // Write data to UART TX FIFO
    UART5_DR_R = data;
};

/*!*******************************************************************
    @brief Reads and returns the UART5 Receive FIFO to get an 8-bit code
    @param data 8-bit code to received through UART
**********************************************************************/
uint8_t Bluetooth_Rx(void){
    uint8_t data;
    // Wait for RX FIFO to not be full
    while((UART5_FR_R & UART_FR_RXFE) != 0){};
    // Write UART RX FIFO to data
    data = (UART5_DR_R & 0xFF);
    return data;
};

/*!*******************************************************************
    @brief Interrupt for UART5, called when data received into RX FIFO
            Sends received code from UART to PORTF_DATA to control LED
**********************************************************************/
void UART5_Handler(void){
    uint8_t tim = 0;
    // Data received by UART
    if (UART5_MIS_R&0x0010){
        // Receive
        out_char = UART_RX();
        // Clear Interrupts
        UART5_ICR_R = 0xFFFF;
    };
};
