// #define saveDebugToEEPROM //Uncomment to begin saving 'UNIVERSAL' commands sent over Serial.  Review these commands by typing 'Z' in the Serial console.  Comment out to use the fuse setting function instead.
#undef SERIAL
#ifdef SERIAL_PORT_USBVIRTUAL
#define SERIAL SERIAL_PORT_USBVIRTUAL
#else
#define SERIAL Serial
#endif

#ifdef saveDebugToEEPROM
#include <EEPROM.h>
int EEPROMaddress = 0;
#endif

#define SDI 9
#define SII 10
#define SDO 11
#define SCL 12
#define RST 13
#define VCC 8

// Configure the baud rate:

#define BAUDRATE 19200
// #define BAUDRATE 115200
// #define BAUDRATE 1000000

#define HWVER 2
#define SWMAJ 1
#define SWMIN 18

// STK Definitions
#define STK_OK 0x10
#define STK_FAILED 0x11
#define STK_UNKNOWN 0x12
#define STK_INSYNC 0x14
#define STK_NOSYNC 0x15
#define CRC_EOP 0x20 // ok it is a space...

#define beget16(addr) (*addr * 256 + *(addr+1) )