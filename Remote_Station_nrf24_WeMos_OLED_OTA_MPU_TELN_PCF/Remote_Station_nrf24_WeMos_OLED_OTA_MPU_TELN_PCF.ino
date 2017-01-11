/********************************************************************

This sketch is tested with following HW:
    - Wemos d1 mini, in OTA configuration ( can be upoloaded via OTA or Serial )
    - Oled display
    - myIMU board
    - myPCF board
    - NRF24 radio


  THIS ARDUINO SKETCH CAN COPMUNICATE WITH PROCESSIGN SKETCH:
  ..\thissketch\processing\MPUTeapot_MPU6050_HCML5883L_combined_remotestation.pde 
  ****  REMEMBER TO SET IN PROCESSING THE CORRECT COM PORT AND BAUD RATE
**********************************************************************
  FOR WEMOS DISPLAY ( I2C . 64x48 ) Adafruit_SSD1306.h + Adafruit_GFX.h libraries are OK
  I2C address is 0x3C with default DC jumper
*********************************************************************
  FOR ADAFRUIT DIPLAY ( I2C )
  Address for 128x64 is 0x3D (default) or 0x3C (if SA0 is grounded)
*********************************************************************/
  // in Adafruit_SSD1306.h you have tu uncomment the proper display definition according to the installed OLED
  //    #define SSD1306_64_48  // uncomment this for WeMos Oled 64x48 diplay
  //    #define SSD1306_128_64    // uncomment this for Adafruit Oled 64x48 diplay
/**********************************************************************
  FOR PCF8574P expander myShield ( I2C )
  Address with AD0/AD1/AD2 grounded is 0x20 (default) 
*********************************************************************/

/*
  WIRING On WeMosD1mini:

  WeMos ==> BaseBoard
  D3  pin, Flash Button ( for IoTAppstore )

  WeMos ==> PCF854P MyShield
  D1  pin, SCL ==> pin 14
  D2  pin, SDA ==> pin 15
 (D3)(*) pin VIA JUMPER ==> pin 13 INT ( active low, falling )  -- DO NOT USE - FLASH AT BOOT --
                   pin 1  ADD0 ==> to VDD via pull-up 1k and  to GND via jumper
                   pin 2  ADD1 ==> to VDD via pull-up 1k and  to GND via jumper
                   pin 3  ADD2 ==> to VDD via pull-up 1k and  to GND via jumper
                   pin 4  P0  ==> Green Led  --|>|--  to VCC via 100Ohm res. ( Low ==> active )
                   pin 5  P1  ==> Red Led    --|>|--  to VCC via 100Ohm res. ( Low ==> active )
                   pin 6  P2  ==> Green Led  --|>|--  to VCC via 100Ohm res. ( Low ==> active )
                   pin 7  P3  ==> Yellow Led --|>|--  to VCC via 100Ohm res. ( Low ==> active )
           GND ==> pin 8  Vss (GND)
                   pin 9  P4  ==> NC
                   pin 10 P5  ==> NC
                   pin 11 P6  ==> Buzzer     --|Bz|-- to VCC via 100Ohm res. ( active LOW )
                   pin 12 P7  <== Button  ( to GND via 1kOhm , to VCC via Button ( Active HIGH )
          +3/+5    pin 16 Vdd selectable to +3 or +5 via jumper
          
(*) in WeMos no pin are available if using together with other peripherials
(*) D4 to D8 used for NRL24
(*) D3 cannot used because flash at startup
(*) D0 is not enabled for INT
            
  WeMos ==> I2C OLED uses :
  D1  pin, GPIO5 ==> SCL 
  D2  pin, GPIO4 ==> SDA

  MPU6050 ==> WeMosD1mini             HMC5883L ==> WeMosD1mini
  Vcc ==> 3V3                         3V3 ==> 3V3
  GND ==> GND                         GND ==> GND
  SDA ==> D2 (GPIO4)                  SDA ==> D2 (GPIO4)  
  SCL ==> D1 (GPIO5)                  SCL ==> D1 (GPIO5) 
  INT ==> Unconnected (*)             DRDY ==> Unconnected  (*)
  AD0 ==> Unconnected
  XCL ==> Unconnected
  XDA ==> Unconnected

  Battery VCC ==> A0 Only if OTA or WiFi is not in USE

  WeMos     ==>   NRL24
  D4        ==>   CE    
  D5  CLK   ==>   CLK
  D6  MISO  ==>   MISO
  D7  MOSI  ==>   MOSI
  D8  SS    ==>   CSN
  




  

*/
/*****************        MPU 6050 CALIBRATION PROCEDURE    *****************
 *         
  - Put the MPU6050 in a flat and horizontal surface. Use an inclinometer to check that it is as horizontal as possible.
  - Modify the RAW program to put every offset to 0. ("setXGyroOffset/setYGyroOffset/setZGyroOffset/setZAccelOffset"  =0 ).
  - Upload the RAW program to your arduino and open serial monitor so you can see the values it is returning.
  - Leave it operating for a few minutes (5-10) so temperature gets stabilized.
  - Check the values in the serial monitor and write them down.
  - Now modify your program again updating your offsets and run the sketch, with updated offsets.
  - Repeat this process until your program is returning 0 for every gyro, 0 for X and Y accel, and +16384 for Z accel.

  or use the MPU_calibration sketch
*/
/*****  PREPROCESSOR DIRECTIVES   **********
 * #undef name
 * #error  emits tokens to standard error, and  prevents the program from being successfully assembled.:
 * #warning emits tokens to standard error
 * #message emits tokens to standard output
 *      Example: #error "Unsupported part:" __PART_NAME__ 
 *      
 * OPERATORS : see   http://www.atmel.com/webdoc/avrassembler/avrassembler.wb_expressions.html
 *      
 *******************************************/

/*http://www.atmel.com/webdoc/avrassembler/avrassembler.wb_preprocessor.Pre-defined_macros.html
 __AVRASM_VERSION__   Integer   Built-in    Assembler version, encoded as (1000*major + minor)
__CORE_VERSION__    file:///C:/Users/PJM03/Google%20Drive/9-PROGETTI/Arduino_Projects/libraries/PCF8574_ESP/README.md  String    #pragma     AVR core version.
__DATE__              String    built-in    Build date. Format: "Jun 28 2004", see -FDformat.
__TIME__              String    built-in    Build time. Format: "HH:MM:SS". see -FDformat
__CENTURY__           Integer   built-in    Build time century (typically 20)
__YEAR__              Integer   built-in    Build time year, less century (0-99)
__MONTH__             Integer   built-in    Build time month (1-12)
__DAY__               Integer   built-in    Build time day (1-31)
_HOUR__               Integer   built-in    Build time hour (0-23)
__MINUTE__            Integer   built-in    Build time minute (0-59)
__SECOND__            Integer   built-in    Build time second (0-59)
__FILE__              String    built-in    Source file name.
__LINE__              Integer   built-in    Current line number in source file.
__PART_NAME__         String    #pragma     AVR part name.
__partname__          Integer   #pragma     partname corresponds to the value of __PART_NAME__. Example: #ifdef __ATmega8__
__CORE_coreversion__  Integer   #pragma     coreversioncorresponds to the value of __CORE_VERSION__. Example: #ifdef __CORE_V2__
*/

/********************************************************************************************

pcf8574_esp.h  library  usage

PCF857x(uint8_t address, TwoWire *UseWire, is8575 = false)
uint8_t read8() -- Read all 8 pins' status at once as a bitmask with a pin being HIGH if the corresponding bit is set, and vice versa.
uint8_t read(uint8_t pin) -- Returns a single pin's status.
void write8(uint8_t value) -- Set all 8 pins' status at once.
void write(uint8_t pin, uint8_t value) -- Set a single pin's status.
void toggle(uint8_t pin) -- Reverses the corresponding pin's status, HIGH to LOW or vice versa.
void toggleAll() -- Reverses all the pins' statuses, from HIGH to LOW and vice versa.
void shiftLeft(uint8_t n=1) -- Shift the pins' states left, with pin 1's state going into pin 2 and so on.
void shiftRight(uint8_t n=1) -- Like above, but to the right instead.
void rotateLeft(uint8_t n=1) -- Rotate the pins' status instead of just shifting them, with pin 7's status going to pin 0, ie. wrapping around.
void rotateRight(uint8_t n=1) -- Like above.
int lastError()
uint16_t read16() -- Returns all the 16 pins' status at once if you set is8575 as true, otherwise doesn't do anything and returns zero.
void write16() -- Sets all the 16 pins' statuses at once if you set is8575 as true, otherwise doesn't do anything.

    // DO NOTE: When you write LOW to a pin on a PCF8574 it becomes an OUTPUT.
    // It wouldn't generate an interrupt if you were to connect a button to it that pulls it HIGH when you press the button.
    // Any pin you wish to use as input must be written HIGH and be pulled LOW to generate an interrupt. 
 
 
 */



