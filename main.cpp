
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
    read_xyz_values,
    happy,
    sad,
    wait_for_switch_press,
    debounce_after_just_a_press,
    wait_for_the_switch_to_be_released,
    debounce_after_the_switch_released
  }
  state;
  
  volatile state current_LED_matrix_state = happy;
  volatile state current_alarm_and_button_state = wait_for_switch_press;
  volatile int beep = 1;



int main()
{
  Serial.begin(9600);
  sei();
  

  float x_coordinate = 0;
  float y_coordinate = 0;
  float z_coordinate = 0;


  init_spi();
  change_display_brightness(0x09);//takes hex values from 0x00 to 0x0F
  change_display_scan_limit(0x07);//set to 0x07 for all the rows to be "scanned"
  change_display_shutdown_mode(0x01);//0x01 for normal mode
  change_display_test_mode(0x00);//0x00 for normal operation

  InitI2C();
  StartI2C_Trans(SLAVE_ADDRESS);
  write(PWR_MGMT);
  write(WAKEUP);
  StopI2C_Trans();
  StartI2C_Trans(SLAVE_ADDRESS);
  write(AFS_SEL);
  write(0x00);
  initTimer1();
  initSwitchPB7();


  while(1)
  {
    StopI2C_Trans();

    Serial.flush();
    delayMs(100);

    switch(current_LED_matrix_state)
    {
      case(read_xyz_values):
      {
        x_coordinate = get_x_coordinate();
        y_coordinate = get_y_coordinate();
        z_coordinate = get_z_coordinate();

        Serial.print("X = ");
        Serial.println((x_coordinate));
        Serial.println();

        Serial.print("Y = ");
        Serial.println((y_coordinate));
        Serial.println();    

        Serial.print("Z = ");
        Serial.println((z_coordinate));
        Serial.println();

        if((z_coordinate) < 0.65)
        {
          current_LED_matrix_state = sad;
          //Serial.println("I'm sad");
          //PWMoff();
        }
        else if((z_coordinate) > 0.65)
        {
          current_LED_matrix_state = happy;
          // PWMoff();
          //Serial.println("I'm happy");
        }
        break;
      }

      case happy:
        Serial.println("in happy state");
        display_happy_face();
        PWMoff();
        //change_duty(0);
        
        current_LED_matrix_state = read_xyz_values;
        beep = 1;
        break;

      case sad:
        Serial.println("in sad state");
        display_sad_face();
        current_LED_matrix_state = read_xyz_values;
        if(beep == 1)
        {
          initTimer3();
        }
        break;
    }
      
    switch(current_alarm_and_button_state)//state machine
      {
            case(wait_for_switch_press)://just keep counting state
            break;

            case(debounce_after_just_a_press):  //debounce the press
            delayMs(1); //debounce time for switch is 100 microseconds
            current_alarm_and_button_state = wait_for_the_switch_to_be_released; //advance to the next state
            break;

            case wait_for_the_switch_to_be_released: //wait for the button release
            break;

            case debounce_after_the_switch_released: //debounce the release
            delayMs(1);
            current_alarm_and_button_state = wait_for_switch_press; //go back to counting
            PWMoff();
            break;
      }



  }


  // sei();
  

  // while(1)
  // {
  //   Serial.println("starting timer");
  //   initTimer3();
  //   _delay_ms(1000);
  //   Serial.println("turning timer off");
  //   PWMoff();
  //   delayMs(1000);

  // }


//   while(1)
//   {
//     initTimer3();




//   while(1)
//   {
//     Serial.println("HEY");
//     _delay_ms(100);
//     Serial.flush();
//   }

  //}
}

ISR(PCINT0_vect) //interrupt service routine for pin change interrupt group 0
{
  Serial.println("interrupt");
  Serial.flush();
  if(current_alarm_and_button_state == wait_for_switch_press)
  {
    current_alarm_and_button_state = debounce_after_just_a_press;
  }
  if(current_alarm_and_button_state == wait_for_the_switch_to_be_released)
  {
    current_alarm_and_button_state = debounce_after_the_switch_released;
    beep = 0;

  }
}