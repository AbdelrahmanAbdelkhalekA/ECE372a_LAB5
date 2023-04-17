#include "switch.h"

/*
 * Initializes pull-up resistor on PB3 and sets it into input mode
 */
void initSwitchPB7()
{
    DDRB &= ~(1 << DDB7); //set the data direction for the pin of the switch to input
    PORTB |= (1 << PORTB7); //enable the pullup resistor for a stable reading
    PCICR |= (1 << PCIE0); //sets bit 0 in the pin change interrupt control register to 1 (enabling interrupts for PCINT bits 0-7)
    PCMSK0 |= (1 << PCINT7); //sets bit 3 in PCMSK0 to 1 in order to enable interrupts for PCINT bit 3
}