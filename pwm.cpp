// Authors/Group Members: Abdelrahman Ahmed Abdelkhalek, Timothy Haines, Paz Davila, George Davis
// Net ID: abdelkhalek, timothyhaines, davilap1 ,gdavis3
// Date: 4/24/23
// Assignment: Lab 5
//----------------------------------------------------------------------//


// void initPWM_Timer4(){
//     //PWM mode 15 bit, OCR4A is the output
//     //Using timer4 for variable frequency
//     // WGM4[3:0] = 1 1 1 1
//     TCCR4A |= (1 << WGM40) | (1 << WGM41); 
//     TCCR4B |= (1 << WGM42) | (1 << WGM43); 

//     //set non-inverting mode - start high 
//     //COM4A[1:0] = 1 0
//     TCCR4A |=   (1 << COM4C1);
//     TCCR4A &=  ~(1 << COM4C0);
    
//     // set prescalar of 1
//     //CS4[2:0] = 0 0 1
//     TCCR4B |= (1 << CS40);
//     TCCR4B &= ~( (1 << CS41) | (1 << CS42));

//     //set header pin 8 to output
//     DDRH |= (1 << DDH5);

//  }

//  void turnOffBuzzer(){
//    OCR4C = 0;
//  }

//  void IncFreq(unsigned int frequency){
   
//     OCR4A = 16000000 / frequency;
//     OCR4AH = OCR4A >> 8;
//     OCR4AL = OCR4A;
//     OCR4CH = OCR4AH >> 1;
//     OCR4CL = OCR4AL >> 1;
   
//  } 

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

void IncFreq(unsigned int frequency){
   
    OCR4A = 16000000 / frequency;
    OCR4AH = OCR4A >> 8;
    OCR4AL = OCR4A;
    OCR4CH = OCR4AH >> 1;
    OCR4CL = OCR4AL >> 1;
   
 } 
