/* DriverLib Includes */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>


// Import the UART driver definitions
#include <ti/drivers/UART.h>
#include <DAD_UART.h>

// Test mode - can be send or receive
#define TESTMODE_SEND


int main(void)
{
    //Stop WDT
    MAP_WDT_A_holdTimer();

    // Related to baudrate generation and sampling. Config struct also
    DAD_UART_Struct uartConfig;
    DAD_UART_Set_Config(9600, EUSCI_A0_BASE, &uartConfig);

    // initialize and enable EUSCI_A0
    size_t bufferSize = 1024;
    DAD_UART_Init(&uartConfig, bufferSize);

    //DAD_UART_Write_Char(&uartConfig, 't');
    //modifiedRingBuf_put(&(uartConfig.UART_Buffer), 'R'); // Put received data at end of buffer

    #ifdef TESTMODE_SEND
        // Generate test string
        int i;
        char msg[2049] = "t";
        for(i = 0; i < 2048; i++)
            strcat(msg, "t");

        // Send test string over UART
        DAD_UART_Write_Str(&uartConfig, msg);
    #endif

    #ifdef TESTMODE_RECEIVE
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
