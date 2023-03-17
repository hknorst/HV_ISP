#include <Arduino.h>
#include <defines.h>

uint8_t write_eeprom(unsigned int length);
uint8_t write_eeprom_chunk(unsigned int start, unsigned int length);
char eeprom_read_page(int length);
