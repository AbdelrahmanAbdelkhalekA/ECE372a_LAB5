// Example code using SPI transmission to write to 8x8 LED array through MAxim 7219 chip
// The 8x8 LED array is part number 1038AS and it is interfaced with the MAX7219CNG chip

#include <Arduino.h>
#include "SPI.h"
#include "i2c.h"
#include <avr/interrupt.h>
#include <avr/io.h>

#define SLA               0x68  // MPU_6050 address with PIN AD0 grounded
#define PWR_MGMT          0x6B
#define WAKEUP            0x00
#define ACCEL_CONFIG      0x1C
#define AFS_SEL_2         0x00
#define SL_MEMA_XAX_HIGH  0x3B
#define SL_MEMA_XAX_LOW   0x3C
#define SL_MEMA_YAX_HIGH  0x3D
#define SL_MEMA_YAX_LOW   0x3E
#define SL_MEMA_ZAX_HIGH  0x3F
#define SL_MEMA_ZAX_LOW   0x40
#define SL_TEMP_HIGH      0x41
#define SL_TEMP_LOW       0x42
#define SL_WHO_AM_I       0X75


int main () {

  Serial.begin(9600);  // use serial port for print in debug
  sei();
  SPI_MASTER_Init(); // initialize SPI module and set the data rate
  initI2C();  // initialize I2C and set bit rate
  
  signed int T_val = 0;
  signed int x_val = 0;
  signed int y_val = 0;
  signed int z_val = 0;
  signed int mode = 2;
  float x_accel = 0;	
  float y_accel = 0;
  float z_accel = 0;

  // initialize 8 x 8 LED array (info from MAX7219 datasheet)
  write_execute(0x0A, 0x08);  // brightness control
  write_execute(0x0B, 0x07); // scanning all rows and columns
  write_execute(0x0C,0x01); // set shutdown register to normal operation (0x01)
  write_execute(0x0F, 0x00); // display test register - set to normal operation (0x01)
  
  write(PWR_MGMT);// address on SLA for Power Management
  write(WAKEUP); // send data to Wake up from sleep mode
  StopI2C_Trans();
  writeTo(SLA, ACCEL_CONFIG, AFS_SEL_2);

  while(1){

    // write "HI" to LED array.  Must address all 8 rows.  
    //Each row has data for individual LED:   "1" LED is on, "0" LED is off

    write_execute(0x01, 0b00000000); // all LEDS in Row 1 are off
    write_execute(0x02, 0b01000010); // row 2 LEDS 
    write_execute(0x03, 0b10100101); // row 3 LEDS
    write_execute(0x04, 0b01000010); // row 4 LEDS
    write_execute(0x05, 0b00000000); // row 5 LEDS
    write_execute(0x06, 0b10000001); // row 6 LEDS
    write_execute(0x07, 0b01111110); // row 7 LEDS
    write_execute(0x08, 0b00000000); // row 8 LEDS

    _delay_ms(1000);  // delay for 1 s to display "HI"

    /// write "ALL" to LED array.  Must address all 8 rows.  
    //Each row has data for individual LED:   "1" LED is on, "0" LED is off

    write_execute(0x01, 0b00000000);
    write_execute(0x02, 0b01000010);
    write_execute(0x03, 0b00000000);
    write_execute(0x04, 0b00000000);
    write_execute(0x05, 0b00111100);
    write_execute(0x06, 0b01000010);
    write_execute(0x07, 0b10000001);
    write_execute(0x08, 0b00000000);

    _delay_ms(2000); // delay for 2 sec to display "ALL"
    
    // status = TWSR & 0xF8;
    Read_from(SLA,SL_MEMA_XAX_HIGH);
    x_val= Read_data(); // read upper value
 
    Read_from(SLA,SL_MEMA_XAX_LOW);
    x_val = (x_val << 8 )| Read_data(); // append lower value
    x_accel = x_val / 16384.0;

    Read_from(SLA,SL_MEMA_YAX_HIGH);
    y_val= Read_data(); // read upper value
 
    Read_from(SLA,SL_MEMA_YAX_LOW);
    y_val = (y_val << 8 )| Read_data(); // append lower value
    y_accel = y_val / 16384.0;

    Read_from(SLA,SL_MEMA_ZAX_HIGH);
    z_val= Read_data(); // read upper value
 
    Read_from(SLA,SL_MEMA_ZAX_LOW);
    z_val = (z_val << 8 )| Read_data(); // append lower value
    z_accel = z_val / 16384.0;

    Read_from(SLA, ACCEL_CONFIG);
    mode = Read_data();

  
    Serial.println(mode);
    Serial.print("X-Axis =  ");
    Serial.print(x_accel);
    Serial.print(",  Y-Axis = ");
    Serial.print(y_accel);
    Serial.print(",  Z-Axis = ");
    Serial.println(z_accel);
    StopI2C_Trans();
    
    
}
return 0;
}
