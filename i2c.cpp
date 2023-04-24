// Authors/Group Members: Abdelrahman Ahmed Abdelkhalek, Timothy Haines, Paz Davila, George Davis
// Net ID: abdelkhalek, timothyhaines, davilap1 ,gdavis3
// Date: 4/24/23
// Assignment: Lab 5
//----------------------------------------------------------------------//
//We were able to use Dr. Hetherington's video to help with this code

#include "i2c.h"
#include <avr/io.h>
#include "Arduino.h"




#define wait_for_completion while(!(TWCR & (1 << TWINT)));


void InitI2C()//initilizes the I2C module on the ATmega 2560
{
  PRR0 &= ~(1<<PRTWI);  // wake up I2C module on AT2560
  TWSR |= (1 << TWPS0);  // prescaler power = 1
  TWBR = 0xC6; // bit rate generator = 10k  (TWBR = 198)
  TWBR = 0xC6; // bit rate generator = 10k  (TWBR = 198)
  TWCR |= (1 << TWINT )| (1 << TWEN); // enable two wire interface

}

void StartI2C_Trans(unsigned char SLA)
{
// this function initiates a start condition and calls slave device with SLA
  TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN); // clear TWINT, intiate a start condition and enable
  wait_for_completion;//wait for completion
  TWDR = (SLA << 1); // slave address + write bit '0'
  TWCR = (1<<TWINT)|(1<<TWEN);  // trigger action:clear flag and enable TWI
  wait_for_completion;//wait for completion

}

void StopI2C_Trans()//stops the I2C Transmission
{
  TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO); //trigger action:  send stop condition
}

void Read_from(unsigned char SLA, unsigned char MEMADDRESS)//reads from the specified slave at the MEMADDRESS on the slave device
{
 StartI2C_Trans(SLA);//start the I2C transmission
 
  write(MEMADDRESS);//write the memory address that we want to read from
  
  TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN); // restart to switch to read mode
  wait_for_completion;//wait for completion
  TWDR = (SLA << 1) | 0x01; // 7 bit address for slave plus read bit
  TWCR = (1 << TWINT) | (1 << TWEN)| (1 << TWEA);// trigger with master sending ack
  wait_for_completion;// wait for completion
  TWCR = (1<< TWINT) | (1 << TWEN);  // master can send a nack now
  wait_for_completion;// wait for completion
  TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO); // Stop condition

}

unsigned char Read_data()//returns the data from the I2C data register
{
  return TWDR;//return the data in the data register
}

void write(unsigned char data)//loads data into the I2C data register and then sends it
{
  TWDR = data; //load data into TWDR register
  TWCR = (1<<TWINT)|(1<<TWEN);  // trigger action:  clear flag and enable TWI
  wait_for_completion;//wait for completion
}

float get_x_coordinate()//returns the x coordinate on the accelerometer
{
    float return_x_value;//value to return
    signed int x_coordinate = 1.0;//value used for intermediate calculation
    Read_from(SLAVE_ADDRESS, ACCEL_XOUT_HIGH);//read the upper byte of the x coordinate from the specified slave
    x_coordinate = Read_data();//assign the data we just got to the intermediate variable
    Read_from(SLAVE_ADDRESS, ACCEL_XOUT_LOW);//now read the lower byte of the x coordinate from the specified slave
    x_coordinate = (x_coordinate << 8);//shift the first byte of data in the intermediate variable to the left by 8 bits (1 byte)
    x_coordinate |= Read_data();//put the lower byte of the x coordinate into the lower byte of the intermediate variable
    return_x_value = (x_coordinate/16384.0);//divide by the value specified in the data sheet so the number makes sense and then assign 
                                            //this value to the variable that we will return
    // Serial.println(return_x_value);//used for debugging
    return return_x_value;//return the x coordinate information we just figured out
}

float get_y_coordinate()//returns the y coordinate on the accelerometer
{
    float return_y_value;//value to return
    signed int y_coordinate = 0;//value used for intermediate calculation
    Read_from(SLAVE_ADDRESS, ACCEL_YOUT_HIGH);//read the upper byte of the y coordinate from the specified slave
    y_coordinate = Read_data();//assign the data we just got to the intermediate variable
    Read_from(SLAVE_ADDRESS, ACCEL_YOUT_LOW);//now read the lower byte of the y coordinate from the specified slave
    y_coordinate = (y_coordinate << 8);//shift the first byte of data in the intermediate variable to the left by 8 bits (1 byte)
    y_coordinate |= Read_data();//put the lower byte of the y coordinate into the lower byte of the intermediate variable
    return_y_value = ((y_coordinate/16384.0));//divide by the value specified in the data sheet so the number makes sense and then assign 
                                              //this value to the variable that we will return
    // Serial.println(return_y_value);//used for debugging
    return return_y_value;//return the y coordinate information we just figured out
}

float get_z_coordinate()//returns the z coordinate on the accelerometer
{
    float return_z_value;//value to return
    signed int z_coordinate = 0;//value used for intermediate calculation
    Read_from(SLAVE_ADDRESS, ACCEL_ZOUT_HIGH);//read the upper byte of the z coordinate from the specified slave
    z_coordinate = Read_data();//assign the data we just got to the intermediate variable
    Read_from(SLAVE_ADDRESS, ACCEL_ZOUT_LOW);//now read the lower byte of the z coordinate from the specified slave
    z_coordinate = (z_coordinate << 8);//shift the first byte of data in the intermediate variable to the left by 8 bits (1 byte)
    z_coordinate |= Read_data();//put the lower byte of the z coordinate into the lower byte of the intermediate variable
    return_z_value = ((z_coordinate/16384.0));//divide by the value specified in the data sheet so the number makes sense and then assign 
                                              //this value to the variable that we will return
    // Serial.println(return_z_value);//used for debugging
    return return_z_value;//return the z coordinate information we just figured out
}