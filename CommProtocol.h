/*!*******************************************************************
    @details Header for library to parse transceived data
**********************************************************************/
#ifndef COMMPROTOCOL_H
#define COMMPROTOCOL_H

// Includes
#include <stdint.h>
#include <stdbool.h>

// UART values
#define BAUD_9600 9600      // 9600 kbps baudrate
#define	BAUD_115200 115200	// 115200
#define WORD_8BIT 0x60      // 8-bit words
#define STOP_2BIT 0x08      // 2 Stop Bits
#define STOP_1BIT 0x00      // 1 Stop Bit
#define PARITY_ODD 0x00     // Odd Parity
#define PARITY_EVEN 0x04    // Even Parity
#define PARITY_ENABLE 0x02	// Parity Enabled
#define PARITY_DISABLE 0x00	// Parity Disabled
#define NORMAL_SPEED 16000000 // Default CPU speed
#define PLL_SPEED 80000000  // PLL speed
// Current CPU Speed
#define CPU_SPEED PLL_SPEED

// LED colors
#define LED_RED		0x02
#define LED_BLUE	0x04
#define LED_GREEN	0x08
#define LED_PURPLE  0x06
#define LED_YELLOW  0x0A
#define LED_CYAN    0x0C
#define LED_WHITE	0x0E
#define LED_OFF     0x00
#define LED_RANGE   0x0E

// Protocol values
#define RW_RANGE 0x80
#define MSG_RANGE 0x60
#define ADDR_RANGE 0x1F
#define RW_READ 0x00
#define RW_WRITE 0x80
#define MSG_ON 0x20
#define MSG_OFF 0x40
#define MSG_FLASH 0x60
#define EMERG_ADDR 0x00
#define BROADCAST_ADDR 0x1F

// Station Type Selectors
#define S_CONTROLLER 1
#define S_REPEATER 2
#define S_END 3

// Station Numbers
#define STATION_1 1
#define STATION_2 2
#define STATION_3 3
#define STATION_4 4

/* CHANGES WHICH STATION THE PROGRAMMED BOARD IS*/
// This Station Number
#define STATION_NUM STATION_1

// Station Properties Selection
// Option 1
#if STATION_NUM == STATION_1
// Station Color
#define STATION_LED LED_PURPLE
// Station Type Selection
#define STATION_TYPE S_CONTROLLER
// The 5-bit device address of this station
#define STATION_ADDR 0x01
// Option 2
#elif STATION_NUM == STATION_2
// Station Color
#define STATION_LED LED_GREEN
// Station Type Selection
#define STATION_TYPE S_REPEATER
// The 5-bit device address of this station
#define STATION_ADDR 0x02
// Option 3
#elif STATION_NUM == STATION_3
// Station Color
#define STATION_LED LED_RED
// Station Type Selection
#define STATION_TYPE S_REPEATER
// The 5-bit device address of this station
#define STATION_ADDR 0x03
// Option 4
#elif STATION_NUM == STATION_4
// Station Color
#define STATION_LED LED_CYAN
// Station Type Selection
#define STATION_TYPE S_END
// The 5-bit device address of this station
#define STATION_ADDR 0x04

#endif

// Volatile Variables
extern volatile uint32_t Now_Msg;
extern volatile uint32_t Old_Msg;
extern volatile bool EmergencyModeFlag;

// Prototypes
bool DuplicateCheck(void);

bool EmergencyCheck(void);

bool BroadcastCheck(void);

bool StationCheck(void);

bool FinalForward(void);

#endif