#include <Arduino.h>
#include <defines.h>
#include <globals.h>

uint16_t flash_read(unsigned int addr);
char flash_read_page(int length);
void read_page();
void read_signature();