/*
 * DAD_UART.h
 *
 *  Created on: Jan 16, 2023
 *      Author: maxim
 */

//Intended as an abstraction to the UART driver


#ifndef DAD_UART_H_
#define DAD_UART_H_

/* DriverLib Includes */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>

// Import the UART driver definitions
#include <ti/drivers/UART.h>

uint8_t DAD_UART_Find_Second_Mod_Reg(float divisionFactor);

void DAD_UART_Set_Config(uint16_t baudRate, eUSCI_UART_ConfigV1* config);

void DAD_UART_Init(uint32_t moduleInstance, eUSCI_UART_ConfigV1* config);

//TODO Stop UART
// void DAD_UART_Stop(uint32_t moduleInstance);

//TODO
void DAD_UART_Write_Test(uint32_t moduleInstance, char c);

//TODO
//void DAD_UART_Write_To_HMI();

//TODO
//void DAD_UART_Write_To_BT();

//TODO
//void DAD_UART_Read_From_HMI();

//TODO
//void DAD_UART_Read_From_BT();

#endif /* DAD_UART_H_ */