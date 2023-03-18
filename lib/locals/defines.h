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
#define BUTTON 2

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

#define  HFUSE  0x747C
#define  LFUSE  0x646C
#define  EFUSE  0x666E

// ATTiny series signatures
#define  ATTINY13   0x9007  // L: 0x6A, H: 0xFF             8 pin
#define  ATTINY24   0x910B  // L: 0x62, H: 0xDF, E: 0xFF   14 pin
#define  ATTINY25   0x9108  // L: 0x62, H: 0xDF, E: 0xFF    8 pin
#define  ATTINY44   0x9207  // L: 0x62, H: 0xDF, E: 0xFFF  14 pin
#define  ATTINY45   0x9206  // L: 0x62, H: 0xDF, E: 0xFF    8 pin
#define  ATTINY84   0x930C  // L: 0x62, H: 0xDF, E: 0xFFF  14 pin
#define  ATTINY85   0x930B  // L: 0x62, H: 0xDF, E: 0xFF    8 pin