#include <util/delay.h>
#include "include/IO328P.h"
#include <avr/eeprom.h>
#include "include/SERIAL328P.h"
#include "include/IR.h"
#include <stdio.h>


#define BLINK_DELAY_MS 100

// uint8_t num1EEM EEMEM = 1;
// const uint8_t num1 = 1;

int main (void)
{
    //#pragma GCC unroll 100 // might be nice
    // Initialize USART0
    USART0Init();
    initIR(3);
    _pinMode(13, mode::OUT);
    //#pragma GCC unroll 10
    IRKey sign;
    bool lastBit = 0;
    while (true)
    {
        sign = (IRKey)readIR(3);
        _digitalWrite(13, state::HI);
        _delay_ms(50);
        _digitalWrite(13, state::LO);
        print_IRKeyString(sign);

    }
}



// int main (void)
// {
//     uint8_t u8TempData;
//     //Initialize USART0
//     USART0Init();
//         while(1)
//         {
//             // Receive data
//             u8TempData = USART0ReceiveByte();
//             // Increment received data
//             u8TempData++;
//             //Send back to terminal
//             USART0SendByte(u8TempData);
//         }
// }