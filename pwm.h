// Authors/Group Members: Abdelrahman Ahmed Abdelkhalek, Timothy Haines, Paz Davila, George Davis
// Net ID: abdelkhalek, timothyhaines, davilap1 ,gdavis3
// Date: 4/24/23
// Assignment: Lab 5
//----------------------------------------------------------------------//

#ifndef PMW_H
#define PMW_H
#include <avr/io.h>

void initTimer3();//initilizes and starts the PWM
void PWMoff();// sets the data direction register of the PWM to input (essentially disabling it from outputing anything)
void change_duty(float duty);//changes the duty cycle of the PWM (the length of time the buzzer is on)

#endif
