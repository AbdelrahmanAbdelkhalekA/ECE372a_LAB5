
#include "i2c.h"
#include <avr/io.h>
#include "Arduino.h"




#define wait_for_completion while(!(TWCR & (1 << TWINT)));


void InitI2C()
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
  wait_for_completion;
  TWDR = (SLA << 1); // slave address + write bit '0'
  TWCR = (1<<TWINT)|(1<<TWEN);  // trigger action:clear flag and enable TWI
  wait_for_completion;

}

void StopI2C_Trans()
{
  TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO); //trigger action:  send stop condition

}

void Read_from(unsigned char SLA, unsigned char MEMADDRESS)
{
 StartI2C_Trans(SLA);
 
  write(MEMADDRESS);
  
  TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN); // restart to switch to read mode
  wait_for_completion;
  TWDR = (SLA << 1) | 0x01; // 7 bit address for slave plus read bit
  TWCR = (1 << TWINT) | (1 << TWEN)| (1 << TWEA);// trigger with master sending ack
  wait_for_completion;
  TWCR = (1<< TWINT) | (1 << TWEN);  // master can send a nack now
  wait_for_completion;
  TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO); // Stop condition

}

unsigned char Read_data()
{
  return TWDR;
}

void write(unsigned char data)
{
  TWDR = data; //load data into TWDR register
  TWCR = (1<<TWINT)|(1<<TWEN);  // trigger action:  clear flag and enable TWI
  wait_for_completion;
}

float get_x_coordinate()
{
    float return_x_value;
    signed int x_coordinate = 1.0;
    Read_from(SLAVE_ADDRESS, ACCEL_XOUT_HIGH);
    x_coordinate = Read_data();
    Read_from(SLAVE_ADDRESS, ACCEL_XOUT_LOW);
    x_coordinate = (x_coordinate << 8);
    x_coordinate |= Read_data();
    return_x_value = (x_coordinate/16384.0);
    // Serial.println(return_x_value);
    return return_x_value;
}

float get_y_coordinate()
{
    float return_y_value;
    signed int y_coordinate = 0;
    Read_from(SLAVE_ADDRESS, ACCEL_YOUT_HIGH);
    y_coordinate = Read_data();
    Read_from(SLAVE_ADDRESS, ACCEL_YOUT_LOW);
    y_coordinate = (y_coordinate << 8);
    y_coordinate |= Read_data();
    return_y_value = ((y_coordinate/16384.0));
    return return_y_value;
}

float get_z_coordinate()
{
    float return_z_value;
    signed int z_coordinate = 0;
    Read_from(SLAVE_ADDRESS, ACCEL_ZOUT_HIGH);
    z_coordinate = Read_data();
    Read_from(SLAVE_ADDRESS, ACCEL_ZOUT_LOW);
    z_coordinate = (z_coordinate << 8);
    z_coordinate |= Read_data();
    return_z_value = ((z_coordinate/16384.0));
    return return_z_value;
}