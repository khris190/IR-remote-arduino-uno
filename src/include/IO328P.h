#ifndef __COFFEFE_AVR_ATMEGA328P_IO__
#define __COFFEFE_AVR_ATMEGA328P_IO__

#include <avr/io.h>
#ifdef __AVR_ATmega328P__

//yep, SET_LO saves 2 bytes, that's amazing
#define SET_HI(bit) (1 << (bit))
#define SET_LO(bit) (0 << (bit))

enum mode{
	IN,
	OUT 
};
enum state{
	LO,
	HI
};

/**
 * @brief write state to one of DDRD or DDRB.
 * 
 * Compiler should optimize this function to "DDRD |= (1 << pin);" form 
 * and then some more it should take 2 bytes of memory, the same as macro version.
 * 
 * @param pin pin number 0 - 13
 * @param m mode of pin operation
 */
void inline _pinMode(uint8_t pin, mode m) {
    if (pin >= 0)
    {
        if (( pin < 8 ) && (m == mode::OUT))
        { 
            DDRD |= (1 << pin); 
        }
        else if (( pin < 8 ) && (m == mode::IN))
        { 
            DDRD &= ~(1 << pin); 
        }
        else if (( pin < 14 ) && (m == mode::OUT))
        {
            DDRB |= (1 << (pin - 8));  
        }
        else if (( pin<14 ) && (m == mode::IN))
        {
            DDRB &= ~(1 << (pin - 8));  
        }
    }
}
/**
 * @brief write state to one of PORTB or PORTD pins.
 * 
 * Compiler should optimize this function to "DDRD |= (1 << pin);" form 
 * and then some more it should take 2 bytes of memory, the same as macro version.
 * 
 * @param pin pin number 0 - 13
 * @param s state to set the pin to
 */
void inline _digitalWrite(uint8_t pin,state s)
{ 
    if (pin >= 0)
    {
        if (( pin < 8 ) && (s == state::HI))
        { 
            PORTD |= (1 << pin); 
        }
        else if (( pin < 8 ) && (s == state::LO))
        { 
            PORTD &= ~(1 << pin); 
        }
        else if (( pin < 14 ) && (s == state::HI))
        {
            PORTB |= (1 << (pin - 8));  
        }
        else if (( pin<14 ) && (s == state::LO))
        {
            PORTB &= ~(1 << (pin - 8));  
        }
    }
}

uint8_t _digitalRead(uint8_t pin) {
    if (pin < 8 && pin >= 0) {
        return ((PIND & (1 << pin)) >> pin);
    } else if (pin < 14) {
        return ((PINB & (1 << (pin - 8))) >> (pin - 8));
    }
}

#endif
#endif