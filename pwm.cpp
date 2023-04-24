// Authors/Group Members: Abdelrahman Ahmed Abdelkhalek, Timothy Haines, Paz Davila, George Davis
// Net ID: abdelkhalek, timothyhaines, davilap1 ,gdavis3
// Date: 4/24/23
// Assignment: Lab 5
//----------------------------------------------------------------------//


#include "PWM.h"
#include <Arduino.h>

void initTimer3()
{
  DDRH |= (1 << PORTH5);
  DDRH |= (1 << PORTH6);

  PORTH |= (1 << PORTH5);
  PORTH &= ~(1 << PORTH6);

  DDRE |= (1 << PORTE4); //PORTE4 is pin 2 on the board //OC3B

  TCCR3A |= (1 << WGM30);
  TCCR3A |= (1 << WGM31);
  TCCR3B |= (1 << WGM32);
  TCCR3B |= (1 << WGM33);

  TCCR3A &= ~(1 << COM3C0); //non-inverting mode
  TCCR3A |= (1 << COM3C1);
  TCCR3A &= ~(1 << COM3B0);
  TCCR3A |= (1 << COM3B1);
  TCCR3A &= ~(1 << COM3A0);
  TCCR3A |= (1 << COM3A1);

  TCCR3B |= (1 << CS30); //prescaler
  TCCR3B &= ~(1 << CS31);
  TCCR3B |= (1 << CS32);

  //PWM frequency
  OCR3A = 5999;



  //duty cycle
  OCR3B = 5999*0.1;
  

}

void change_duty(float duty)
{
  OCR3B = 5999*duty;
   //Serial.println(OCR3B);
   //Serial.flush();
}

void PWMoff()
{
  DDRE  &= ~(1 << DDE4);
}
