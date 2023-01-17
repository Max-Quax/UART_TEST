/*
 * DAD_UART.c
 *
 *  Created on: Jan 16, 2023
 *      Author: maxim
 */

#include <DAD_UART.h>

uint8_t DAD_UART_Find_Second_Mod_Reg(float divisionFactor){
    //TODO lookup table
    float fract = divisionFactor - ((int)divisionFactor);
    float table[] = {0.0000, 0.0529, 0.0715, 0.0835, 0.1001, 0.1252, 0.1430, 0.1670, 0.2147, 0.2224, 0.2503, 0.3000, 0.3335, 0.3575, 0.3753, 0.4003, 0.4286, 0.4378,
                     0.5002, 0.5715, 0.5715, 0.6003, 0.6254, 0.6432, 0.6667, 0.7001, 0.7147, 0.7503, 0.7861, 0.8004, 0.8333, 0.8464, 0.8572, 0.8751, 0.9004, 0.9170, 0.9288, 1};

    if(divisionFactor == 0)
        return 0;
    if(divisionFactor == 1)
        return 1;
    for(uint8_t i = 0; i <= 0xFF; i++){
        if(divisionFactor > table[i])
            return i - 1;
    }
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
    UART_initModule(moduleInstance, &uartConfig);
    UART_enableModule(moduleInstance);

    //Which interrupt is it
    uint32_t interrupt;
    switch(moduleInstance){
    case:EUSCI_A0_BASE
            interrupt = INT_EUSCIA0;
    case:EUSCI_A1_BASE
            interrupt = INT_EUSCIA1;
    case:EUSCI_A2_BASE
            interrupt = INT_EUSCIA2;
    case:EUSCI_A3_BASE
            interrupt = INT_EUSCIA3;
    break;

    //Enable Interrupts
    MAP_UART_enableInterrupt(moduleInstance, EUSCI_A_UART_RECEIVE_INTERRUPT);
    MAP_Interrupt_enableInterrupt(interrupt);
    MAP_Interrupt_enableSleepOnIsrExit();
    MAP_Interrupt_enableMaster();
    }
}

