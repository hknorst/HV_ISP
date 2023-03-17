#include <Arduino.h>
#include <defines.h>
#include <globals.h>

uint8_t getSerialChar();
void fillBuff(int n);
void empty_reply();
void breply(uint8_t b);
void get_version(uint8_t c);
void universal();