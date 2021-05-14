
#include <stdbool.h>
#include <stdint.h>
#include "uart0.h"
#include "clock.h"
#include "HC-SR04.h"

int main(void)
{
	//set up clocks and ports
    initSystemClockTo40Mhz();
    //turn on uart0
    initUart0();
    setUart0BaudRate(115200,40e6);
    //setup HC-SR04 Echo Device
    initEchoDevice();

    while(1)
    {
        sendTrigger(); //sends a 10us trigger. response is handled with interrupts
    }
}
