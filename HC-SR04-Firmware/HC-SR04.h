/*
 * HC-SR04.h
 *
 *  Created on: May 12, 2021
 *      Author: Abiria Placide
 */

#ifndef HC_SR04_H_
#define HC_SR04_H_

/**
 * PINS:
 * PORTB0-trigger(input)
 * PORTB1-echo (output)
 * ------------DEVICE PINS-------------------
   VCC = +5VDC
   Trig = Trigger input of Sensor
   Echo = Echo output of Sensor (150us to 25ms, 38ms if no obstacle)
   GND = GND

Time = Width of Echo pulse, in uS (micro second)
Distance in centimeters = Time(uS) / 58
Distance in inches = Time(uS) / 148

 *
 */

#include "gpio.h"
void sendTrigger(); //send pulse to be received
void initEchoDevice();
#endif /* HC_SR04_H_ */
