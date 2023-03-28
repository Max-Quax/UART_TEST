/* DriverLib Includes */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>


// Import the UART driver definitions
#include <ti/drivers/UART.h>
#include <HAL/DAD_UART.h>
#include <HAL/DAD_Timer.h>

// Test mode - can be send or receive
    // TESTMODE_RECEIVE
    // TESTMODE_SEND
    // TESTMODE_FILL_EMPTY
    // TESTMODE_RX_2SOURCES
#define TESTMODE_RX_2SOURCES

typedef enum {READ, WRITE} testState;

int main(void)
{
    //Stop WDT
    MAP_WDT_A_holdTimer();

#ifdef TESTMODE_RX_2SOURCES
    // Related to baudrate generation and sampling. Config struct also
    DAD_UART_Struct uartConfig1, uartConfig2;

    DAD_UART_Set_Config(57600, EUSCI_A1_BASE, &uartConfig1);
    DAD_UART_Set_Config(57600, EUSCI_A2_BASE, &uartConfig2);

    // initialize and enable EUSCI_A0
    size_t bufferSize = 512;
    DAD_UART_Init(&uartConfig2, bufferSize);        // This might be part of the problem
//    MAP_UART_initModule(uartConfig2.moduleInst, &uartConfig2.uartConfig);
//    MAP_UART_enableModule(uartConfig2.moduleInst);
//    // TODO buffptr
//    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P3,    // GPIO for A2 UART
//                              GPIO_PIN3 | GPIO_PIN2, GPIO_PRIMARY_MODULE_FUNCTION);
//    MAP_UART_enableInterrupt(uartConfig2.moduleInst, EUSCI_A_UART_RECEIVE_INTERRUPT);
//    MAP_Interrupt_enableInterrupt(INT_EUSCIA2);
    // Debug - disable module
//    UART_disableModule(uartConfig2.moduleInst);
//    UART_disableInterrupt(uartConfig2.moduleInst, EUSCI_A_UART_RECEIVE_INTERRUPT);
//    Interrupt_disableInterrupt(INT_EUSCIA2);

    if(!DAD_UART_Init(&uartConfig1, bufferSize)){   // If initialization fails, loop
        while(1);
    }

    while(1){
        char c = 's';

        // Test A2
        // Blocks until char 'b' is received. Receiving 'b' begins test
        while(c != 'b'){
            while(!DAD_UART_HasChar(&uartConfig2));  // Block until has char
            c = DAD_UART_GetChar(&uartConfig2);
        }

        // Receive actual test chars
        c = 't';
        while(c == 't'){                            // All chars should be 't'.
            c = DAD_UART_GetChar(&uartConfig2);
        }
        //assert(c == 'q');                           // Check that character received really is a q and not corrupted


        //MAP_UART_disableModule(uartConfig2.moduleInst);
        //MAP_UART_enableModule(uartConfig1.moduleInst);
        MAP_GPIO_toggleOutputOnPin(GPIO_PORT_P1, GPIO_PIN0); // Toggle LED


        // Test A1
        // Blocks until char 'b' is received. Receiving 'b' begins test
        while(c != 'b'){
            while(!DAD_UART_HasChar(&uartConfig1)); // Block until has char
            c = DAD_UART_GetChar(&uartConfig1);
        }

        // Receive actual test chars
        c = 't';
        while(c == 't'){                            // All chars should be 't'.
            c = DAD_UART_GetChar(&uartConfig1);
        }
        //assert(c == 'q');                         // Check that character received really is a q and not corrupted
//        MAP_UART_disableModule(uartConfig1.moduleInst);
//        MAP_UART_enableModule(uartConfig2.moduleInst);
    }
    #endif


    #ifdef TESTMODE_SEND
    // Related to baudrate generation and sampling. Config struct also
    DAD_UART_Struct uartConfig;
    DAD_UART_Set_Config(57600, EUSCI_A2_BASE, &uartConfig);

    // initialize and enable EUSCI_A0
    size_t bufferSize = 1024;
    DAD_UART_Init(&uartConfig, bufferSize);
    double timeElapsed;


    // Timer setup
    Timer_A_UpModeConfig timerCfg;
    DAD_Timer_Initialize_ms(60000, TIMER_A1_BASE, &timerCfg);

    // Generate test string
    int i;
    char msg[2049] = "t";
    for(i = 0; i < 2048; i++)
        strcat(msg, "t");

    // Send test string over UART
    DAD_Timer_Start(TIMER_A1_BASE);                         // Begin test timer
    DAD_UART_Write_Char(&uartConfig, 's');
    DAD_UART_Write_Str(&uartConfig, msg);
    DAD_UART_Write_Char(&uartConfig, 'q');
    timeElapsed = DAD_Timer_Stop(TIMER_A1_BASE, &timerCfg); // End test timer

    #endif

    #ifdef TESTMODE_RECEIVE
    // Related to baudrate generation and sampling. Config struct also
    DAD_UART_Struct uartConfig;
    DAD_UART_Set_Config(57600, EUSCI_A2_BASE, &uartConfig);

    // initialize and enable EUSCI_A0
    size_t bufferSize = 1024;
    DAD_UART_Init(&uartConfig, bufferSize);
    double timeElapsed;


    char c = '/s';
    double timeElapsedMS;

    // Timer setup
    Timer_A_UpModeConfig timerCfg;
    DAD_Timer_Initialize_ms(60000, TIMER_A1_BASE, &uartConfig);


    // Blocks until char 'b' is received. Receiving 'b' begins test
    while(c != 'b'){
        while(!DAD_UART_HasChar(&uartConfig));  // Block until has char
        c = DAD_UART_GetChar(&uartConfig);
    }
    DAD_Timer_Start(TIMER_A1_BASE);             // Begin test timer


    // Receive actual test chars
    c = 't';
    while(c == 't'){                            // All chars should be 't'.
        c = DAD_UART_GetChar(&uartConfig);
    }

    // Receiving a 'q' ends the test
    timeElapsed = DAD_Timer_Stop(TIMER_A1_BASE, &timerCfg); // End test timer
    assert(c == 'q');                           // Check that character received really is a q and not corrupted

    #endif

#ifdef TESTMODE_FILL_EMPTY
    // Related to baudrate generation and sampling. Config struct also
    DAD_UART_Struct uartConfig;
    DAD_UART_Set_Config(57600, EUSCI_A2_BASE, &uartConfig);

    // initialize and enable EUSCI_A0
    size_t bufferSize = 1024;
    DAD_UART_Init(&uartConfig, bufferSize);
    double timeElapsed;


    testState state = READ;
    // Timer setup
    Timer_A_UpModeConfig timerCfg;
    DAD_Timer_Initialize_ms(1000, TIMER_A1_BASE, &timerCfg);


    // Blocks until char 'b' is received. Receiving 'b' begins test
    while(true){
        switch(state){
        case READ:
            if(DAD_Timer_Has_Finished(TIMER_A1_BASE) &&
                bufferSize*.75 < DAD_UART_NumCharsInBuffer(&uartConfig)){
                state = WRITE;
            }

            break;
        case WRITE:
            // Stop reading
            DAD_UART_DisableInt(&uartConfig);

            // Flush out chars from read buffer
            while(DAD_UART_NumCharsInBuffer(&uartConfig) > 0){
                DAD_UART_GetChar(&uartConfig);
            }

            // Finished writing to HMI/microSD, start listening again
            state = READ;
            DAD_UART_EnableInt(&uartConfig);
            DAD_Timer_Start(TIMER_A1_BASE);

            break;
        }
        MAP_GPIO_toggleOutputOnPin(GPIO_PORT_P1, GPIO_PIN0);    // Debug - check that it's not hung up
        MAP_PCM_gotoLPM0();                                     // Go back to sleep until next interrupt
    }

#endif

    // Basic tests
    /*while(DAD_UART_NumCharsInBuffer(&(uartConfig->UART_Buffer) < bufferSize - 1)){
     // If an 'R' is received, toggle light
     if(DAD_UART_HasChar(&uartConfig) && DAD_UART_GetChar(&uartConfig) == 'R'){
         // MAP_GPIO_toggleOutputOnPin(GPIO_PORT_P1, GPIO_PIN0);
         // DAD_UART_Write_Test(&uartConfig, 'T'); // TODO bugs out if write immediately after read
     }
    }*/

    while(1);   // Trap CPU

    // TODO Time how long it takes to run through whole buffer


     // Done receive data through buffer
     // TODO test write thoroughly
     // TODO test timing
     // TODO test stop
     // TODO test different baud rates
     // TODO test longer messages
}
