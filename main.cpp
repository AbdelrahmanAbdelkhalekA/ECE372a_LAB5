// Authors/Group Members: Abdelrahman Ahmed Abdelkhalek, Timothy Haines, Paz Davila, George Davis
// Net ID: abdelkhalek, timothyhaines, davilap1 ,gdavis3
// Date: 4/24/23
// Assignment: Lab 5
//----------------------------------------------------------------------//

#include <avr/io.h>
#include <Arduino.h>
#include "spi.h"
#include "i2c.h"
#include "pwm.h"
#include "timer.h"
#include "switch.h"
#include <avr/interrupt.h>
#include "util\delay.h"

  typedef enum mystates
  {
    read_xyz_values,//in this state we will just be reading/printing the values from the accelerometer
    happy,//in this state we will simply show the happy face
    sad,//in this state we will simply show the sad face
    wait_for_switch_press,//this state is used in the debounce state machine for waiting for a button press
    debounce_after_just_a_press,//this state is used to wait 1ms to debounce the button press
    wait_for_the_switch_to_be_released,//this state is used to wait for the button to be released
    debounce_after_the_switch_released//this state is used to wait for 1ms to debounce the button release
  }
  state;//new type
  
  volatile state current_LED_matrix_state = happy;// set my initial state of the led matrix to showing the happy face
  volatile state current_alarm_and_button_state = wait_for_switch_press;// set my initial state of the button/alarm to just waiting
  volatile int beep = 1;//this variable is used to keep track of whether we should be beeping or not.



int main()//main function
{
  Serial.begin(9600);//set baud rate for communication with the microcontroller
  sei();//enable global interrupts
  

  float x_coordinate = 0;//variable used to stor the x coordinate
  float y_coordinate = 0;//variable used to stor the y coordinate
  float z_coordinate = 0;//variable used to stor the z coordinate


  init_spi();//setup the spi module on the ATmega 2560
  change_display_brightness(0x09);//takes hex values from 0x00 to 0x0F
  change_display_scan_limit(0x07);//set to 0x07 for all the rows to be "scanned"
  change_display_shutdown_mode(0x01);//0x01 for normal mode
  change_display_test_mode(0x00);//0x00 for normal operation

  InitI2C();//setup the the I2C module on the ATmega 2560
  StartI2C_Trans(SLAVE_ADDRESS);//basically just gets the slave's attention
  write(PWR_MGMT);//loads data into the I2C data register and then sends it
  write(WAKEUP);//used to wakeup the device
  StopI2C_Trans();//stops the I2C transmission of data
  StartI2C_Trans(SLAVE_ADDRESS);//starts the I2C transmission again
  write(AFS_SEL);// sets the sensitivity of the acccelerometer
  write(0x00);//lowest sensitivity
  initTimer1();//used for any delays (such as debouncing)
  initSwitchPB7();//used for muting the buzzer
  StopI2C_Trans();//good idea to stop the I2C trnsmissions sometimes


  while(1)//loop forever
  {
    StopI2C_Trans();//good idea to stop the I2C trnsmissions sometimes

    Serial.flush();//flush out any data that may be waiting to be printed
    delayMs(100);//used to control the rate of the whole sequence of the lab

    switch(current_LED_matrix_state)//first state machine which controls the state of the LED matrix and the buzzer
    {
      case(read_xyz_values)://read the x, y, and z values state
        x_coordinate = get_x_coordinate();//get the x coordinate
        y_coordinate = get_y_coordinate();//get the y coordinate
        z_coordinate = get_z_coordinate();//get the z coordinate

        Serial.print("X = ");//print "X ="
        Serial.println((x_coordinate));//print the x coordinate we received
        Serial.println();//print a new line

        Serial.print("Y = ");//print "Y ="
        Serial.println((y_coordinate));//print the x coordinate we received
        Serial.println();//print a new line

        Serial.print("Z = ");//print "Z ="
        Serial.println((z_coordinate));//print the x coordinate we received
        Serial.println();//print a new line

        StopI2C_Trans();//good idea to stop the I2C trnsmissions sometimes

        if((z_coordinate) < 0.65)//0.65 is what I found to be about a 45 degree angle in any direction
        {
          current_LED_matrix_state = sad;//set the current state to "sad"
        }
        else if((z_coordinate) > 0.65)//0.65 is what I found to be about a 45 degree angle in any direction
        {
          current_LED_matrix_state = happy;//set the current state to happy
        }
        break;//end of this case

      case happy://state of happiness
        //Serial.println("in happy state");// used for debugging
        display_happy_face();// show the happy face on the led matrix
        PWMoff();//turn the buzzer off        
        current_LED_matrix_state = read_xyz_values;// we want to check the xyz values next
        beep = 1;//next time we have a z coordinate value below 0.65 allow beeping from the buzzer
        break;//end of happy state

      case sad://state of sadness
        //Serial.println("in sad state");// used for debugging
        display_sad_face();// display the sad face
        current_LED_matrix_state = read_xyz_values;//we want to check the xyz values again next
        if(beep == 1)// if we are allowed to beep
        {
          initTimer3();//start beeping
        }
        break;//end of sadness state
    }
      
    switch(current_alarm_and_button_state)//state machine for debouncing
      {
        case(wait_for_switch_press)://just keep counting state
        break;//end of case

        case(debounce_after_just_a_press):  //debounce the press
          delayMs(1); //debounce time for switch is 100 microseconds
          current_alarm_and_button_state = wait_for_the_switch_to_be_released; //advance to the next state
        break;//end of case

        case wait_for_the_switch_to_be_released: //wait for the button release
        break;//end of case

        case debounce_after_the_switch_released: //debounce the release
          delayMs(1);//debounce time for switch
          current_alarm_and_button_state = wait_for_switch_press; //go back to waiting for a switch press
          PWMoff();//turn off the beeping
        break;//end of case
      }
  }
}

ISR(PCINT0_vect) //interrupt service routine for pin change interrupt group 0
{
  // Serial.println("interrupt");// used for debugging
  // Serial.flush();//flush the buffer
  if(current_alarm_and_button_state == wait_for_switch_press)//if we are in the waiting for a press state
  {
    current_alarm_and_button_state = debounce_after_just_a_press;//advance to the debounce press state
  }
  if(current_alarm_and_button_state == wait_for_the_switch_to_be_released)//if we are in the waiting for a release state
  {
    current_alarm_and_button_state = debounce_after_the_switch_released;//advance to the debounce release state
    beep = 0;//disable beeping
  }
}