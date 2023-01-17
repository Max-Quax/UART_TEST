/*
 * DAD_UART.c
 *
 *  Created on: Jan 16, 2023
 *      Author: maxim
 */

#include <DAD_UART.h>

uint8_t DAD_UART_Find_Second_Mod_Reg(float divisionFactor){
    //TODO lookup table
    float

}

void DAD_UART_Set_Config(uint16_6 baudRate, eUSCI_UART_ConfigV1* uartConfig){
    //Set Baud rate and modulation registers
    float divisionFactor = ((float)CS_getSMCLK()) / baudRate;                   //N
    uartConfig.selectClockSource =  EUSCI_A_UART_CLOCKSOURCE_SMCLK;             //3MHz
    uartConfig.clockPrescalar = (uint_fast16_t)(divisionFactor/16);             //int(N/16)
    uartConfig.firstModReg = (uint_fast8_t)((divisionFactor/16-((int)divisionFactor/16))*16);            //
    uartConfig.secondModReg = DAD_UART_Find_Second_Mod_Reg(divisionFactor);     //Set modulation rate
    uartConfig.overSampling = EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION;    // Oversampling

    //Set other UART configuration
    uartConfig.parity = EUSCI_A_UART_NO_PARITY;
    uartConfig.msborLsbFirst = EUSCI_A_UART_LSB_FIRST;
    uartConfig.numberofStopBits = EUSCI_A_UART_ONE_STOP_BIT;
    uartConfig.dataLength = EUSCI_A_UART_8_BIT_LEN;
    uartConfig.uartMode = EUSCI_A_UART_MODE;
}

void DAD_UART_Init(uint32_t moduleInstance, eUSCI_UART_ConfigV1* config){
    UART_initModule(EUSCI_A0_BASE, &uartConfig);
    UART_enableModule(EUSCI_A0_BASE);
}

