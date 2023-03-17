#include <Arduino.h>
#include <defines.h>
#include <globals.h>

void program_page();
void write_flash(int length);
uint8_t write_flash_pages(int length);
void flash(unsigned int addr, uint8_t high, uint8_t low);