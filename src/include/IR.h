#ifndef IR_H
#define IR_H
#include "IO328P.h"
#include <util/delay.h>
#include "SERIAL328P.h"

uint16_t inline readSigLen(char pin, state state);

enum IRKey{
    OnOff = 41565,
    VolUp = 25245,
    StopFunc = 57885,
    Prev = 8925,
    PausePlay = 765,
    Next = 49725,
    Down = 57375,
    VolDown = 43095,
    Up = 36975,
    Zero = 26775,
    EQ = 39015,
    StRept = 45135,
    One = 12495,
    Two = 6375,
    Three = 31365,
    Four = 4335,
    Five = 14535,
    Six = 23205,
    Seven = 17085,
    Eight = 19125,
    Nine = 21165
};
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wwrite-strings"
void print_IRKeyString(IRKey key)
{
    switch (key)
    {
        case OnOff: 
            serialPrintStr("OnOff\n\r");
            break;  
        case VolUp:
            serialPrintStr("VolUp\n\r");
            break;  
        case StopFunc: 
            serialPrintStr("StopFunc\n\r");
            break;  
        case Prev: 
            serialPrintStr("Prev\n\r");
            break;  
        case PausePlay:
            serialPrintStr("PausePlay\n\r");
            break;   
        case Next: 
            serialPrintStr("Next\n\r");
            break;  
        case Down: 
            serialPrintStr("Down\n\r");
            break;  
        case VolDown:
            serialPrintStr("VolDown\n\r");
            break;   
        case Up: 
            serialPrintStr("Up\n\r");
            break;  
        case Zero: 
            serialPrintStr("0\n\r");
            break;  
        case EQ: 
            serialPrintStr("EQ\n\r");
            break;  
        case StRept:
            serialPrintStr("StRept\n\r");
            break;   
        case One: 
            serialPrintStr("1\n\r");
            break;  
        case Two: 
            serialPrintStr("2\n\r");
            break;  
        case Three: 
            serialPrintStr("3\n\r");
            break;  
        case Four: 
            serialPrintStr("4\n\r");
            break;  
        case Five: 
            serialPrintStr("5\n\r");
            break;  
        case Six: 
            serialPrintStr("6\n\r");
            break;  
        case Seven: 
            serialPrintStr("7\n\r");
            break;  
        case Eight: 
            serialPrintStr("8\n\r");
            break;  
        case Nine: 
            serialPrintStr("9\n\r");
            break;  
        default: break;
    }
}
#pragma GCC diagnostic pop


void initIR(char pin)
{
    if (14 > pin && pin >= 0)
    {
        _pinMode(pin, mode::IN);
        _digitalWrite(pin, state::HI);
    }
}

uint16_t readIR(char pin) 
{
    const int ARRSIZE = 70;
    uint16_t res = 0;
    uint16_t signals[ARRSIZE];
    //shift cause reading in loop takes almost exactly 2 times longer
    signals[0] = readSigLen(pin, state::HI);
    const uint16_t sigLen = signals[0] >> 5;
    uint16_t tmp_lastSigLen = 0;

    uint8_t i = 1;
    for (; i < ARRSIZE; i++)
    {
        signals[i] = readSigLen(pin, (state)((i+1)%2));
        if (signals[0] < signals[i])
        {
            break;
        }
    }

    if (i == 67)
    {
        for (uint8_t j = 3+(16*2); j < i-1; j+=2)
        {
            if (signals[j] > sigLen << 1)
            {
                res = 1 + (res << 1);
                // USART0SendByte('1');
            } else {
                // USART0SendByte('0');
                res = (res << 1);
            }
            
        }
        //serialPrintStr("\n\r");
        return res;
    }
    for (uint8_t j = 0; j < i-1; j++)
    {
        serialPrintStr("ON: ");
        serialPrintInt(signals[j]);
        j++;
        serialPrintStr("  OFF: ");
        serialPrintInt(signals[j]);
        serialPrintStr("\n\r");
    }
    return 0;
    
    
    //return readSigLen(pin, state::HI);
}

uint16_t inline readSigLen(char pin, state state) 
{
    uint16_t len = 0;
    bool last_sig = (state+1)%2;
    while (true)
    {
        if(_digitalRead(3) != state) 
        {
            last_sig = !state;
            len++;
        } 
        else
        {
            last_sig = state;
            if (len || len > 0xfffe)
            {
                return len;
            }
        }
    }
}


#endif // IR_H
