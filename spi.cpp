// Authors/Group Members: Abdelrahman Ahmed Abdelkhalek, Timothy Haines, Paz Davila, George Davis
// Net ID: abdelkhalek, timothyhaines, davilap1 ,gdavis3
// Date: 4/24/23
// Assignment: Lab 5
//----------------------------------------------------------------------//

#include <Arduino.h>
#include "spi.h"
#include <avr/interrupt.h>
#include <avr/io.h>


#define DDR_SPI DDRB//data direction register for SPI
#define DD_SS DDB0//bit in data direction register for the SS signal
#define DD_SCK DDB1//bit in data direction register for the SCK signal
#define DD_MOSI DDB2//bit in data direction register for the MOSI signal
#define DD_MISO DDB3//bit in data direction register for the MISO signal
#define SPI_PORT PORTB//port for our spi output
#define SPI_SS_BIT PORTB0//pin in  our spi output register what we will be using to output data



void init_spi()//initialize the spi module on the ATmega 2560
{
  DDR_SPI |= (1 << DD_SS); //SS
  DDR_SPI |= (1 << DD_SCK); //SCK (CLOCK)
  DDR_SPI |= (1 << DD_MOSI); //MOSI

  SPI_PORT |= (1 << SPI_SS_BIT);//set the SS pin as output

  SPCR |= (1 << SPE);//enable SPI operations
  SPCR |= (1 << MSTR);//ATMEGA is the master
  SPCR |= (1 << CPOL); //high when idle
  SPCR |= (1 << CPHA); // setup on the leading edge
  
  //set fosc/128 (I don't think it really matters for our case)
  SPCR |= (1 << SPR0);
  SPCR |= (1 << SPR1);
}

void SPI_MasterTransmit_REGISTER_then_DATA(unsigned char reg_code, unsigned char data)
{
  SPI_PORT &= ~(1 << SPI_SS_BIT); //SS PIN low for data transfer
  SPDR = reg_code;//transmit the register code
  while(!(SPSR & (1<<SPIF)));//wait for the transmission to complete
  SPDR = data;
  while(!(SPSR & (1<<SPIF)));//wait for the transmission to complete
  SPI_PORT |= (1 << SPI_SS_BIT); //put the high (end of transmission)
}

void change_display_brightness(unsigned char brightness)
{
  SPI_MasterTransmit_REGISTER_then_DATA(0x0A, brightness);//intensity
}

void change_display_scan_limit(unsigned char scanLimit)
{
  SPI_MasterTransmit_REGISTER_then_DATA(0x0B, scanLimit);//scan limit
}

void change_display_shutdown_mode(unsigned char shutdownMode)
{
    SPI_MasterTransmit_REGISTER_then_DATA(0x0C, shutdownMode);//shutdown
}

void change_display_test_mode(unsigned char testMode)
{
  SPI_MasterTransmit_REGISTER_then_DATA(0x0F, testMode);//display test
}

void display_happy_face()//this is the function that will display a happy face
{
  SPI_MasterTransmit_REGISTER_then_DATA(0x01, 0b00000000);
  SPI_MasterTransmit_REGISTER_then_DATA(0x02, 0b01100110);
  SPI_MasterTransmit_REGISTER_then_DATA(0x03, 0b01100110);
  SPI_MasterTransmit_REGISTER_then_DATA(0x04, 0b00000000);
  SPI_MasterTransmit_REGISTER_then_DATA(0x05, 0b11000011);
  SPI_MasterTransmit_REGISTER_then_DATA(0x06, 0b11111111);
  SPI_MasterTransmit_REGISTER_then_DATA(0x07, 0b01111110);
  SPI_MasterTransmit_REGISTER_then_DATA(0x08, 0b00111100);
}

void display_sad_face()//this is the function that will display a sad face
{
  SPI_MasterTransmit_REGISTER_then_DATA(0x01, 0b00000000);
  SPI_MasterTransmit_REGISTER_then_DATA(0x02, 0b01100110);
  SPI_MasterTransmit_REGISTER_then_DATA(0x03, 0b01100110);
  SPI_MasterTransmit_REGISTER_then_DATA(0x04, 0b00000000);
  SPI_MasterTransmit_REGISTER_then_DATA(0x05, 0b00111100);
  SPI_MasterTransmit_REGISTER_then_DATA(0x06, 0b01111110);
  SPI_MasterTransmit_REGISTER_then_DATA(0x07, 0b11111111);
  SPI_MasterTransmit_REGISTER_then_DATA(0x08, 0b11000011);
}