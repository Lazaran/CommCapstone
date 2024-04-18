/*!*******************************************************************
    @details Library to parse transceived data
**********************************************************************/

// Library Includes
#include <stdint.h>
#include <stdbool.h>
#include "LEDBTNS.h"
#include "RadioLib.h"
#include "BluetoothLib.h"
#include "CommProtocol.h"
#include <tm4c123gh6pm.h>

/*!*******************************************************************
    @brief Check: Is the received message a duplicate of the previous message
    @returns True if message was duplicate, false otherwise
**********************************************************************/
bool DuplicateCheck(void){
    if (Now_Msg == Old_Msg) {
        return true;
    }
    else {
        return false;
    }
}

/*!*******************************************************************
    @brief Check: Is the received message an Emergency Flash/Off command
            Action: LED Flash or Off
    @returns True if message was emergency, false otherwise
**********************************************************************/
bool EmergencyCheck(void){
    // Check if message emmergency
    if ((Now_Msg & ADDR_RANGE) == EMERG_ADDR){
        // Flash
        if ((Now_Msg & MSG_RANGE) == MSG_FLASH){
            //DisableButtons
            EmergencyModeFlag = true;
            //LED Flash Enable
            TIMER0_CTL_R = 0x01;    // Control: Enable Timer A
        }
        else if ((Now_Msg & MSG_RANGE) == MSG_OFF){
            //EnableButtons
            EmergencyModeFlag = false;
            //LED Flash Disable
            TIMER0_CTL_R = 0x00;    // Control: Enable Timer A
            //LED Off
            SetLED(LED_OFF);
        }
        // Forward message if not End Station
        if (STATION_TYPE != S_END){
            Radio_Tx(Now_Msg);
        }
        return true;
    };
    return false;
};

/*!*******************************************************************
    @brief Check: Is the received message a Broadcast On/Off command
            Action: LED On or Off
    @returns True if message was broadcast, false otherwise
**********************************************************************/
bool BroadcastCheck(void){
    // Check if message for all
    if ((Now_Msg & ADDR_RANGE) == BROADCAST_ADDR){
        // On/Off
        if ((Now_Msg & MSG_RANGE) == MSG_ON){
            SetLED(STATION_LED);
        }
        else if ((Now_Msg & MSG_RANGE) == MSG_OFF){
            SetLED(LED_OFF);
        }
        // Forward message if not End Station
        if (STATION_TYPE != S_END){
            Radio_Tx(Now_Msg);
        }
        return true;
    };
    return false;
};

/*!*******************************************************************
    @brief Check: Is the received message a Station On/Off command
            Action: LED On or Off
    @returns True if message was station-specific, false otherwise
**********************************************************************/
bool StationCheck(void){
    // Check if message for this station
    if ((Now_Msg & ADDR_RANGE) == STATION_ADDR){
        // On/Off
        if ((Now_Msg & MSG_RANGE) == MSG_ON){
            SetLED(STATION_LED);
        }
        else if ((Now_Msg & MSG_RANGE) == MSG_OFF){
            SetLED(LED_OFF);
        }
        // Drop
        return true;
    };
    return false;
};

/*!*******************************************************************
    @brief Check: Is the received message a Read or a Write type | CHANGE TYPE TO OTHER WORD
            Action: Read | App to Station msg | Forward to other stations
                    Write | Station to App msg | Forward Repeater to Controller or Controller to Application
    @returns True if message was station-specific, false otherwise
**********************************************************************/
bool FinalForward(void){
    // Forward
    // Check if App to Station Msg
    if ((Now_Msg & RW_RANGE) == RW_READ){
        // Send to Stations if not End Station
        if (STATION_TYPE != S_END){
            Radio_Tx(Now_Msg);
        }
        return true;
    }
    // Check if Station to App Msg
    else if ((Now_Msg & RW_RANGE) == RW_WRITE){
        // Check if Station is Controller
        if (STATION_TYPE == S_CONTROLLER){
            // Send to App
            Bluetooth_Tx(Now_Msg);
        }
        // Check if Station is Repeater (Excluding End Station)
        else if (STATION_TYPE == S_REPEATER){
            // Send to Stations
            Radio_Tx(Now_Msg);
        };
        return true;
    }; 
    return false;
};
