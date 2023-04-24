// Authors/Group Members: Abdelrahman Ahmed Abdelkhalek, Timothy Haines, Paz Davila, George Davis
// Net ID: abdelkhalek, timothyhaines, davilap1 ,gdavis3
// Date: 4/24/23
// Assignment: Lab 5
//----------------------------------------------------------------------//
#ifndef I2C_H
#define I2C_H

#define SLAVE_ADDRESS 0x68
#define PWR_MGMT 0x6B //power management address
#define WAKEUP 0x00 //wakeup mode
#define ACCEL_XOUT_HIGH 0x3B
#define ACCEL_XOUT_LOW 0x3C
#define ACCEL_YOUT_HIGH 0x3D
#define ACCEL_YOUT_LOW 0x3E
#define ACCEL_ZOUT_HIGH 0x3F
#define ACCEL_ZOUT_LOW 0x40
#define AFS_SEL 0x1C

void InitI2C();
void StartI2C_Trans(unsigned char SLA);
void StopI2C_Trans();
void Read_from(unsigned char SLA, unsigned char MEMADDRESS);
unsigned char Read_data();
void write(unsigned char data);

float get_x_coordinate();
float get_y_coordinate();
float get_z_coordinate();


#endif