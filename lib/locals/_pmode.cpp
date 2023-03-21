// Functions to enter / exit HV programming mode

#include <_pmode.h>

extern int pmode;

void start_pmode()
{
    if (!pmode)
    {
        digitalWrite(SDI, LOW);
        digitalWrite(SII, LOW);
        digitalWrite(SDO, LOW);

        pinMode(SDO, OUTPUT);
        digitalWrite(VCC, 1);
        delayMicroseconds(30);
        digitalWrite(RST, 0);
        delayMicroseconds(30);
        pinMode(SDO, INPUT);
        delayMicroseconds(300);
        // HVP mode entered.  Now command Flash Writing
        pmode = 1;
        Serial.println("Programming mode on.");
    }
}

void end_pmode()
{
    if (pmode)
    {
        digitalWrite(RST, 1);
        digitalWrite(VCC, 0);
        pinMode(SDO, OUTPUT);
        pmode = 0;
        Serial.println("Programming mode off.");
    }
}
