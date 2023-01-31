/*
 * DAD_UART.h
 *
 *  Created on: Jan 16, 2023
 *      Author: maxim
 */

/*
 // Example usage:

  eUSCI_UART_ConfigV1 uartConfig;
     DAD_UART_Set_Config(9600, &uartConfig);

     // initialize and enable EUSCI_A0
     DAD_UART_Init(EUSCI_A0_BASE, &uartConfig);

     while(true){
         DAD_UART_Write_Test(EUSCI_A0_BASE, 'P');
     }

 */


//Intended as an abstraction to the UART driver


#ifndef DAD_UART_H_
#define DAD_UART_H_

// Driverlib
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

// Standard Includes
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

// Specific to UART HAL
#include <ti/drivers/UART.h>            // Import the UART driver definitions
#include <modifiedRingbuf.h>            // Import ring buffer

typedef struct DAD_UART_Struct_{
    unsigned char* bufPtr;
    eUSCI_UART_ConfigV1 uartConfig;
    uint32_t moduleInst;
    RingBuf_Object UART_Buffer;
} DAD_UART_Struct;


// Populates the config, sets the baud rate
void DAD_UART_Set_Config(uint16_t baudRate, uint32_t moduleInstance, DAD_UART_Struct* UARTPtr);

// Initializes the uart module with the specified config information. Needs a buffer to put characters in.
void DAD_UART_Init(DAD_UART_Struct* UARTPtr, size_t bufferSize);

// TODO Stop UART
void DAD_UART_Stop(DAD_UART_Struct* UARTPtr);

// TODO Get a full message from UART
// bool DAD_UART_GetMSG(DAD_UART_Struct* UARTPtr, unsigned char* c);

// Get single char from UART
unsigned char DAD_UART_GetChar(DAD_UART_Struct* UARTPtr);

void DAD_UART_Write_Test(DAD_UART_Struct* UARTPtr, char c);

// At least 1 char is ready
bool DAD_UART_HasChar(DAD_UART_Struct* UARTPtr);

// Returns number of chars in buffer
size_t DAD_UART_NumCharsInBuffer(DAD_UART_Struct* UARTPtr);

// Used in Set_Config
    //Returns the value for the second modulation register
    // Uses fractional part of division factor to look through table.
    // Return value just below fractional part
static uint8_t DAD_UART_Find_Second_Mod_Reg(float divisionFactor);

// TODO At least 1 full message is ready
//bool DAD_UART_MsgReady(DAD_UART_Struct* UARTPtr);

// TODO - CRC?
// TODO - DMA?

#endif /* DAD_UART_H_ */
