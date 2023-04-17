#ifndef I2C_H
#define I2C_H

void initI2C();
void StartI2C_Trans(unsigned char SLA);
void StopI2C_Trans();
void writeTo(unsigned char SLA, unsigned char REGADDRESS, unsigned char data);
void write(unsigned char data);
void Read_from(unsigned char SLA, unsigned char MEMADDRESS);
unsigned char Read_data();
#endif
