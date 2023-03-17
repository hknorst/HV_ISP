/*
Code modified by Tyler Gerritsen
2020-04-13
td0g.ca

This code uses an Arduino-based High-Voltage Programmer as an ISP (In-Service Programmer)
Most 'High-Voltage Programmer' programs can only set fuses.  This program can upload sketches as well!

CREDITS + LICENSE

  Credit to Paul Willoughby 03/20/2010
  http://www.rickety.us/2010/03/arduino-avr-high-voltage-serial-programmer/
  
  Credit to Randall Bohn
  ArduinoISP (available in Arduino --> Examples --> Arduino As ISP)
  ArduinoISP
  Copyright (c) 2008-2011 Randall Bohn
  If you require a license, see
  http://www.opensource.org/licenses/bsd-license.php
  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
  IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, 
  OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; 
  OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


USE:
  ONLY TESTED WITH ATTINY85!!!
  Upload sketch to Arduino
  Connect to ATTINY microcontroller as shown in Paul Willoghby's website
  Programming Flash:
    Tools --> Programmer --> Arduino As ISP
  Setting Fuses:
    Open Serial Console (19200 baud)
    Type 'Z'
    Follow prompts

 CHANGELOG
  0.1
    2020-04-13 
    Initial Commit
    Only tested with ATTINY85!!!
    EEPROM not tested!!!
 */

//#include <stdint.h>
#include <Arduino.h>
#include <defines.h>
#include <_serial.h>
#include <_fuses.h>
#include <_misc.h>
#include <_pmode.h>
#include <_flash_read.h>
#include <_flash_write.h>
#include <main.h>

parameter param;
int error = 0;
int pmode = 0;
unsigned int here; // address for reading and writing, set by 'U' command
uint8_t buff[256]; // global block storage

// Configure the serial port to use.
//
// Prefer the USB virtual serial port (aka. native USB port), if the Arduino has one:
//   - it does not autoreset (except for the magic baud rate of 1200).
//   - it is more reliable because of USB handshaking.
//
// Leonardo and similar have an USB virtual serial port: 'Serial'.
// Due and Zero have an USB virtual serial port: 'SerialUSB'.
//
// On the Due and Zero, 'Serial' can be used too, provided you disable autoreset.
// To use 'Serial': #define SERIAL Serial





void setup() {
  SERIAL.begin(BAUDRATE);
  pinMode(RST,OUTPUT);
  digitalWrite(RST, 1);
  pinMode(SDI, OUTPUT);
  pinMode(SII, OUTPUT);
  pinMode(SDO, OUTPUT);
  pinMode(SCL, OUTPUT);
  pinMode(VCC, OUTPUT);
}

void loop(void) {
  if (SERIAL.available()) {
    uint8_t ch = getSerialChar();
    switch (ch) {
      case 'Z':  //Breaking the rules a little bit here... This is our special Fuse Setting / Debugging command.  Doesn't seem to affect programming!
        #ifdef saveDebugToEEPROM
          for (int i = 0; i < 511; i++){
            if (!(i & 0b00000011)) Serial.println();
            else Serial.print("  ");
            Serial.print(EEPROM.read(i), HEX);
          }
        #else
          burnFuses();
        #endif
       break;
      case '0': // signon
        error = 0;
        empty_reply();
        break;
      case '1':
        if (getSerialChar() == CRC_EOP) {
          SERIAL.print((char) STK_INSYNC);
          SERIAL.print("AVR ISP");
          SERIAL.print((char) STK_OK);
        }
        else {
          error++;
          SERIAL.print((char) STK_NOSYNC);
        }
        break;
      case 'A':
        get_version(getSerialChar());
        break;
      case 'B':
        fillBuff(20);
        set_parameters();
        empty_reply();
        break;
      case 'E': // extended parameters - ignore for now
        fillBuff(5);
        empty_reply();
        break;
      case 'P':
        if (!pmode) start_pmode();
        empty_reply();
        break;
      case 'U': // set address (word)
        here = getSerialChar();
        here = here +  (uint16_t)256 * getSerialChar();
        empty_reply();
        break;
  
      case 0x60: //STK_PROG_FLASH
        getSerialChar(); // low addr
        getSerialChar(); // high addr
        empty_reply();
        break;
      case 0x61: //STK_PROG_DATA
        getSerialChar(); // data
        empty_reply();
        break;
  
      case 0x64: //STK_PROG_PAGE
        program_page(); 
        break;
  
      case 0x74: //STK_READ_PAGE 't'
        read_page();
        break;
  
      case 'V': //0x56
        universal();
        break;
      case 'Q': //0x51
        error = 0;
        end_pmode();
        empty_reply();
        break;
  
      case 0x75: //STK_READ_SIGN 'u'
        read_signature();
        break;
  
      // expecting a command, not CRC_EOP
      // this is how we can get back in sync
      case CRC_EOP:
        error++;
        SERIAL.print((char) STK_NOSYNC);
        break;
  
      // anything else we will return STK_UNKNOWN
      default:
        error++;
        if (CRC_EOP == getSerialChar())
          SERIAL.print((char)STK_UNKNOWN);
        else
          SERIAL.print((char)STK_NOSYNC);
    }
  }
}
