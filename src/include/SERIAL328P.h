#ifndef SERIAL328P_H
#define SERIAL328P_H
#include "IO328P.h"
#include <stdlib.h>


#define CLOCK_RATE 16000000UL;

void serialInit(uint16_t baud)
{
    long clockRate = CLOCK_RATE;
    long UBRR_Val = (clockRate /16/baud-1);

    // Set-up Baudrate
    UBRR0 = UBRR_Val;

    // Enable Reception and Transmission
    UCSR0B |= (0 << RXCIE0) | (0 << TXCIE0) | (1 << RXEN0) |
        (1 << TXEN0);

    // Set-up
    UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00); // 8-bit
    UCSR0B |= (0 << UCSZ02);
	UCSR0B |= (1<<RXEN0)|(1<<RXCIE0);

    UCSR0C |= (0 << UPM01) | (0 << UPM00); // No-Parity

    UCSR0C |= (0 << USBS0); // 1 stop bit

    UDR0 = 0;
}

void serialWrite(uint8_t data)
{
    //while((UCSR0A & (1 << TXC0)) == 0);
    UDR0 = data;
    while( ( UCSR0A & ( 1 << UDRE0 ) ) == 0 ){}
}



void setup()
{
	
    _pinMode(13, OUT);
    _pinMode(12, OUT);

    //pinMode(2,INPUT_PULLUP);
    _pinMode(2, IN);
    _digitalWrite(2, HI);
    
    serialInit(9600);
    //Serial.begin(9600);
	  DDRD &= ~(1 << PD0);
	  PORTD |= (1 << PD0);
	  EICRA |= (1 << 1);    // set INT0 to trigger on falling edge logic change
    EIMSK |= (1 << 0);
    //sei();
    _digitalWrite(12, state::HI);
}


#include <avr/io.h>
#ifndef USART_BAUDRATE
    #define USART_BAUDRATE 9600
#endif
#define UBRR_VALUE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)
void USART0Init(unsigned long baudrate = USART_BAUDRATE)
{
    const int UBRR_val = (((F_CPU / (USART_BAUDRATE * 16UL))) - 1);
    // Set baud rate
    UBRR0L = (uint8_t)UBRR_val;
    UBRR0H = (uint8_t)(UBRR_val>>8);
    // Set frame format to 8 data bits, no parity, 1 stop bit
    UCSR0C |= (1<<UCSZ01)|(1<<UCSZ00);
    //enable transmission and reception
    UCSR0B |= (1<<RXEN0)|(1<<TXEN0);
}
void USART0SendByte(uint8_t u8Data)
{
    //wait while previous byte is completed
    while(!(UCSR0A&(1<<UDRE0))){};
    // Transmit data
    UDR0 = u8Data;
}
uint8_t USART0ReceiveByte()
{
    // Wait for byte to be received
    while(!(UCSR0A&(1<<RXC0))){};
    // Return received data
    return UDR0;
}

void serialPrintStr(char *text)
{
    while(*text) USART0SendByte(*text++);
}

void serialPrintInt(uint16_t value)
{
    char buffer[6];     
    itoa(value, buffer, 10);
    serialPrintStr(buffer);
}

#endif // SERIAL328P_H
