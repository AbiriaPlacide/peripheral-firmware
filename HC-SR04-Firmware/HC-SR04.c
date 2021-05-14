/*
 * HC-SR04.c
 *
 *  Created on: May 12, 2021
 *      Author: abiria Placide
 */

#include <stdio.h>
#include "HC-SR04.h"
#include "timer1.h"
#include "uart0.h"



#define TRIGGER          PORTB,0
#define ECHO             PORTB,1
#define Us10             400
#define READ8PULSE       8000
#define INTERUPT_OFFSET  16

char buffer[20];

void initEchoDevice()
{
    //set up a an rising edge and falling edge interrupt on PORB
    //once detected. start timer. turn off timer on falling edge interrupt. return value
    //8 pulses at 40 khz == 5000 CLK cycles. at @ 40mhz == 8000 CLK cycles
    enablePort(PORTB);
    selectPinDigitalInput(ECHO); //make it an input
    selectPinInterruptBothEdges(ECHO);
    enablePinInterrupt(ECHO);
    //turn on interrupt in interrupt for PORTB in controller CHECK PG. 104 & PG. 142 of DOCS
    NVIC_EN0_R |= 1 << (INT_GPIOB-INTERUPT_OFFSET); //enable interrupt to NVIC for GPIO port B
}

void sendTrigger()
{
    static bool xor = 1;
    xor ^=1;
    //maybe init timer to zero
    enablePort(PORTB);
    selectPinPushPullOutput(TRIGGER);//set pin as output
    setPinValue(TRIGGER, 1);
    setPinValue(TRIGGER,0); //off 5us
    _delay_cycles(Us10/2);
    //delay 10us
    setPinValue(TRIGGER,1); //on 10us
    _delay_cycles(Us10);
    setPinValue(TRIGGER,0);
    turnOnLED(BLUE,xor); //to make sure its sending a trigger
}

void receiveEcho()
{
    static uint8_t FallingRising  = 0;
    static bool timer1On = 0;
    volatile float timer1value = 0;
    if(timer1On == 0)
    {
        initTimer1();        //start timer on first rising edge
        timer1On = 1;
    }


    if(FallingRising ==0)
    {
        TIMER1_TAV_R =0; //load timer with 0
        timer1value = TIMER1_TAV_R;
        sprintf(buffer,"%f inches 0\r\n",((timer1value/(40e6))/2)/29); //148 FOR INCHES, 58 FOR CENTIMENTERS
        putsUart0(buffer);
        clearPinInterrupt(ECHO);
        turnOnLED(RED,1);
        FallingRising = 1;
    }

    else if(FallingRising == 1)
    {
        //get value of of timer, turn off timer then return value
        timer1value = TIMER1_TAV_R;
        sprintf(buffer,"%f inches 1\r\n",((timer1value/(40e6))/2)/29);
        putsUart0(buffer);
        clearPinInterrupt(ECHO);
        turnOnLED(GREEN,1);
        FallingRising = 0;
    }
}

