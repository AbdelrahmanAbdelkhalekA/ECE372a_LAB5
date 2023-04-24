
#ifndef SPI_H
#define SPI_H

void init_spi();
void SPI_MasterTransmit_REGISTER_then_DATA(unsigned char reg_code, unsigned char data);
void change_display_brightness(unsigned char brightness);
void change_display_scan_limit(unsigned char scanLimit);
void change_display_shutdown_mode(unsigned char shutdownMode);
void change_display_test_mode(unsigned char testMode);
void display_happy_face();
void display_sad_face();



#endif