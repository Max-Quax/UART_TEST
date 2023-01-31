/* DriverLib Includes */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>


// Import the UART driver definitions
#include <ti/drivers/UART.h>
#include <DAD_UART.h>


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

    DAD_UART_Write_Test(&uartConfig, 'T');
    modifiedRingBuf_put(&(uartConfig.UART_Buffer), 'R'); // Put received data at end of buffer
    while(DAD_UART_NumCharsInBuffer(&(uartConfig->UART_Buffer) < bufferSize - 1)){
     // If an 'R' is received, toggle light
     if(DAD_UART_HasChar(&uartConfig) && DAD_UART_GetChar(&uartConfig) == 'R'){
         MAP_GPIO_toggleOutputOnPin(GPIO_PORT_P1, GPIO_PIN0);
         //DAD_UART_Write_Test(&uartConfig, 'T'); // TODO bugs out if write immediately after read
     }
    }

    // TODO Time how long it takes to run through whole buffer


     // Done receive data through buffer
     // TODO test write thoroughly
     // TODO test timing
     // TODO test stop
     // TODO test different baud rates
     // TODO test longer messages
}
