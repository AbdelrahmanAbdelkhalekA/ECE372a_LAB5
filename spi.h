// Authors/Group Members: Abdelrahman Ahmed Abdelkhalek, Timothy Haines, Paz Davila, George Davis
// Net ID: abdelkhalek, timothyhaines, davilap1 ,gdavis3
// Date: 4/24/23
// Assignment: Lab 5
//----------------------------------------------------------------------//

#ifndef SPI_H
#define SPI_H

void init_spi();
void SPI_MasterTransmit_REGISTER_then_DATA(unsigned char reg_code, unsigned char data);//sends a register and then data
void change_display_brightness(unsigned char brightness);//sets the brightness of the led matrix
void change_display_scan_limit(unsigned char scanLimit);//sets the scan limit to "scan" the led matrix
void change_display_shutdown_mode(unsigned char shutdownMode);//sets the power mode of the module driving the led matrix
void change_display_test_mode(unsigned char testMode);//sets the mode to test or normal mode
void display_happy_face();//displays a happy face on the led matrix
void display_sad_face();//displays a sad face on the led matrix



#endif