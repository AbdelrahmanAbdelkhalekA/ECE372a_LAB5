#ifndef PMW_H
#define PMW_H
#include <avr/io.h>

void initTimer3();
void PWMoff();
void change_duty(float duty);
void turn_right();
void turn_left();


#endif
