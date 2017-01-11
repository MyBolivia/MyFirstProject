
/******************************************************************************************************************************/

#define DEBUG             // uncomment to print in Serial Monitor the debug message of this sketch
//#define DEBUG_TELNET
//#define IN_LOOP_DEBUG        // uncomment to monitor the IP connection and other function inside the main loop
#define OTA_IN_USE        // uncomment if you want to keep the OTA function after first upload, to upload sketches via WiFi
#define RADIO_IN_USE      // uncomment to activate the RFn24 radio comunication to the base station ( Via SPI )
#define OLED_IN_USE       // uncomment to activate OLED display ( via I2C )
//#define ANALOG_IN_USE   // uncomment to activate the A0 analog input ---   NON COMPATIBILE CON WIFY e quindi OTA ---

#ifdef DEBUG_TELNET
#define MY_SERIAL serverClient
#endif
#ifndef DEBUG_TELNET
#define MY_SERIAL Serial
#endif


// ================================================================
// ===                      PCF 8574 INITIAL DECLARATIONS       ===
// ================================================================

#define PCF8574_IN_USE    // uncomment to activate PCF8574 expander MySHield
//#define PCF8574_INTERRUPT // (* do not use with ESP8266) uncomment to activate INT callback from expander to test buttons
//#define PCF_INTERRUPT_PIN D3  // do not use D0(GPIO16), not allowed in ESP8266 as INT
#define PCF8574_ADDRESS 0x20 //( 0x20 ==> all Ad in LOW )
#define EXPANDER_BUZZER_PIN 6
#define EXPANDER_BUTTON_PIN 7
// pins 4 and 5 unconnected

// ================================================================
// ===                      HCM5883L INITIAL DECLARATIONS       ===
// ================================================================


/////////////////////////////////////////////////////#define HCM5883L_IN_USE //uncomment if HCM5883L is in use
//#define HCM5883_HDG_ONLY //uncomment to send to serial HEADING IN DEGREE ONLY ( to processing ..)
#define COMPASS_PACKET  //uncomment to send the compass data in packet for processing
//#define COMPASS_READABLE_HEADING
#define COMPASS_TILT_COMPENSATED
#define AVERAGED_HEADING


// ================================================================
// ===                      MPU6050 INITIAL DECLARATIONS        ===
// ================================================================

//#define AUTOSTART //uncomment if loop has to start automatically
//#define MY_SETUP_DEBUG //uncomment to show debug information in loop
//#define MY_DEBUG //uncomment to show debug information in loop
//#define SHOW_EMPTY //uncomment to show the empty information cycle from MPU
////////////////////////////////////////////////////////////////#define MPU6050_IN_USE //uncomment if MPU6050 is in use
//#define INTERRUPT_PIN_MPU6050 D3  ///// Da rivedere per ora NON USARE CON WeMos condizionano il BOOT
//#define INTERRUPT_PIN_HCM5883L D4 ///// Da rivedere per ora NON USARE CON WeMos condizionano il BOOT
// ================================================================
// ===              DATA COMUNICATION MODES DECLARATIONS        ===
// ================================================================
//#define SERIAL_DATA_MODE  // uncomment for sending data via USB Serial Monitor
#define TELNET_DATA_MODE  // uncomment for sending data via Telnet ( WiFi )
//#define RADIO_DATA_MODE   // uncomment for sending data via nRF24


// ================================================================
// ===              SERIAL MONITOR PROTOCOL DECLARATIONS        ===
// ================================================================
//#define OUTPUT_READABLE_QUATERNION
//#define OUTPUT_READABLE_EULER
//#define OUTPUT_READABLE_YAWPITCHROLL
//#define OUTPUT_READABLE_REALACCEL
//#define OUTPUT_READABLE_WORLDACCEL
#define OUTPUT_TEAPOT // for PROCESSING
//#define OUTPUT_MPU_TEMPERATURE //uncomment to diplay the MCU temperature

// uncomment "OUTPUT_READABLE_QUATERNION" if you want to see the actual
// quaternion components in a [w, x, y, z] format (not best for parsing
// on a remote host such as Processing or something though)

// uncomment "OUTPUT_READABLE_EULER" if you want to see Euler angles
// (in degrees) calculated from the quaternions coming from the FIFO.
// Note that Euler angles suffer from gimbal lock (for more info, see
// http://en.wikipedia.org/wiki/Gimbal_lock)

// uncomment "OUTPUT_READABLE_YAWPITCHROLL" if you want to see the yaw/
// pitch/roll angles (in degrees) calculated from the quaternions coming
// from the FIFO. Note this also requires gravity vector calculations.
// Also note that yaw/pitch/roll angles suffer from gimbal lock (for
// more info, see: http://en.wikipedia.org/wiki/Gimbal_lock)

// uncomment "OUTPUT_READABLE_REALACCEL" if you want to see acceleration
// components with gravity removed. This acceleration reference frame is
// not compensated for orientation, so +X is always +X according to the
// sensor, just without the effects of gravity. If you want acceleration
// compensated for orientation, us OUTPUT_READABLE_WORLDACCEL instead.

// uncomment "OUTPUT_READABLE_WORLDACCEL" if you want to see acceleration
// components with gravity removed and adjusted for the world frame of
// reference (yaw is relative to initial orientation, since no magnetometer
// is present in this case). Could be quite handy in some cases.

// uncomment "OUTPUT_TEAPOT" if you want output that matches the
// format used for the InvenSense teapot demo


