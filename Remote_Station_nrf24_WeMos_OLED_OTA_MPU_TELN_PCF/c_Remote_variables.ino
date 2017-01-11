/*************************** WiFi  **************************/
#if defined (OTA_IN_USE) || defined (TELNET_DATA_MODE)       
// if you are using OTA, connect PC to Serial Monitor via USB port using RTTY ( Serial ) instead of Arduino IDE
const char* remote_host = "192.168.1.1"; //"www.google.com";
const char* ssid = "Telecom-23831509";
const char* password = "BolZxxN415B#";
uint8_t MAC_array[6];
char MAC_char[18];
#endif

/*************************** PCF 8574 **************************/

#ifdef PCF8574_IN_USE
// Wire.h already defines "Wire" which the PCF8574-class would use by default, but for the sakes of an example let's define our own instance of it and use that instead!
TwoWire testWire;
// class initializer PCF8574(uint8_t address, int sda = SDA, int scl = SCL, TwoWire UseWire = Wire)
PCF857x myPcf(0x20, &testWire);
volatile bool MyFlag = false;
uint8_t oldValue;
int expMode = 0;
#endif

/*************************** Telnet **************************/

#ifdef TELNET_DATA_MODE
// provide text for the WiFi status
const char *str_status[]= {
  "WL_IDLE_STATUS",
  "WL_NO_SSID_AVAIL",
  "WL_SCAN_COMPLETED",
  "WL_CONNECTED",
  "WL_CONNECT_FAILED",
  "WL_CONNECTION_LOST",
  "WL_DISCONNECTED"
};

// provide text for the WiFi mode
const char *str_mode[]= { "WIFI_OFF", "WIFI_STA", "WIFI_AP", "WIFI_AP_STA" };

// declare telnet server (do NOT put in setup())
WiFiServer telnetServer(23);
WiFiClient serverClient;
int telnetInterval = 500;
unsigned long telnetStartTime = millis();

#endif
/*************************** Vcc measure **************************/
/*
  ESP.getVcc() may be used to measure supply voltage. ESP needs to reconfigure the ADC
  at startup in order for this feature to be available. Add the following line to the top of
  your sketch to use getVcc:
  ADC_MODE(ADC_VCC);
  TOUT pin has to be disconnected in this mode.
  Note that by default ADC is configured to read from TOUT pin using analogRead(A0),
  and ESP.getVCC() is not available."
*/
#ifdef OLED_IN_USE
#ifndef ANALOG_IN_USE
ADC_MODE(ADC_VCC);  // if you uncomment this command, the ADC input (A0) is connected to the internal Vcc source, instead of phisical A0 pin
#endif

/*************************** OLED **************************/
#define OLED_RESET 30  // for WeMos use 30 (change OLED_RESET to different pin if using default I2C pins D4/D5 with ESP8266.
Adafruit_SSD1306 display(OLED_RESET);

#define MYLOGO_GLCD_HEIGHT 64
#define MYLOGO_GLCD_WIDTH  48
static  const unsigned char PROGMEM mylogo_glcd_bmp [] = {
  0x00, 0x0A, 0xA0, 0x00, 0x00, 0x0A, 0xA0, 0x00, 0x00, 0x5F, 0xFC, 0x00, 0x00, 0x7F, 0xF4, 0x14,
  0x00, 0xFF, 0xFF, 0x80, 0x03, 0xFF, 0xFE, 0x3C, 0x01, 0xFF, 0xFF, 0xC0, 0x07, 0xFF, 0xFF, 0x10,
  0x07, 0xFF, 0xFF, 0xE0, 0x0F, 0xFF, 0xFF, 0xC0, 0x0F, 0xFF, 0xFF, 0xF0, 0x1F, 0xFF, 0xFF, 0xE0,
  0x1F, 0xF8, 0x3F, 0xF8, 0x3F, 0xF0, 0x3F, 0xF0, 0x1F, 0xC0, 0x07, 0xFC, 0x7F, 0xC0, 0x07, 0xF0,
  0x3F, 0x80, 0x03, 0xFE, 0xFF, 0x80, 0x03, 0xF8, 0x7F, 0x00, 0x00, 0xFF, 0xFE, 0x00, 0x01, 0xFC,
  0x7E, 0x00, 0x00, 0x7F, 0xFC, 0x07, 0x00, 0xFC, 0x7E, 0x00, 0x00, 0x7F, 0xF8, 0x07, 0x00, 0xFC,
  0xFE, 0x00, 0x00, 0x3F, 0xF8, 0x07, 0x00, 0xFE, 0x7C, 0x04, 0x40, 0x1F, 0xF0, 0x3F, 0xE0, 0x7C,
  0xFC, 0x3F, 0xF8, 0x0F, 0xE0, 0x3F, 0xE0, 0x7E, 0x7C, 0x1F, 0xF8, 0x07, 0xE0, 0x3F, 0xE0, 0x7C,
  0xFC, 0x08, 0x88, 0x1F, 0xF0, 0x3F, 0xE0, 0x7E, 0x7C, 0x00, 0x00, 0x1F, 0xF0, 0x07, 0x00, 0x7C,
  0xFE, 0x00, 0x00, 0x3F, 0xF8, 0x07, 0x00, 0xFE, 0x7E, 0x00, 0x00, 0x7F, 0xFC, 0x00, 0x00, 0xFC,
  0x7F, 0x00, 0x00, 0xFF, 0xFE, 0x00, 0x01, 0xFC, 0x7F, 0x00, 0x01, 0xFC, 0xFE, 0x00, 0x01, 0xF8,
  0x3F, 0x80, 0x03, 0xFC, 0x7F, 0x80, 0x07, 0xF8, 0x1F, 0xE0, 0x07, 0xF8, 0x3F, 0xC0, 0x0F, 0xF0,
  0x0F, 0xFE, 0xFF, 0xF8, 0x3F, 0xFE, 0xFF, 0xE0, 0x0F, 0xFF, 0xFF, 0xF0, 0x1F, 0xFF, 0xFF, 0xC0,
  0x03, 0xFF, 0xFF, 0xE0, 0x0F, 0xFF, 0xFF, 0x80, 0x01, 0xFF, 0xFF, 0x00, 0x03, 0xFF, 0xFF, 0x00,
  0x00, 0xFF, 0xFE, 0x00, 0x00, 0xFF, 0xFE, 0x00, 0x00, 0x1F, 0xFC, 0x00, 0x00, 0x7F, 0xF0, 0x00,
  0x00, 0x02, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x0E, 0x3E, 0x9F, 0x8E, 0x77, 0xFB, 0x98, 0xF8, 0x0F, 0x1F, 0xDF, 0xCC, 0x77, 0xF3, 0x9D, 0xFC,
  0x1F, 0x3F, 0xDF, 0xEE, 0x33, 0xF3, 0x9B, 0xFE, 0x1F, 0x19, 0xDC, 0x7C, 0x71, 0xC3, 0xDB, 0x9C,
  0x3F, 0xBF, 0xDC, 0x6E, 0x70, 0xC3, 0xFB, 0x8E, 0x1B, 0x9F, 0x9C, 0x6C, 0x71, 0xC7, 0x7F, 0x0C,
  0x3B, 0xBF, 0x9C, 0x6E, 0x30, 0xC3, 0x7B, 0x8E, 0x3F, 0x99, 0x9C, 0x7C, 0x71, 0xC3, 0x7B, 0x9C,
  0x7F, 0xF9, 0xDF, 0xEE, 0xF3, 0xFB, 0x3B, 0xFC, 0x71, 0xDD, 0xDF, 0xC7, 0xE7, 0xF3, 0x3D, 0xFC,
  0x70, 0xF8, 0xFF, 0x83, 0x87, 0xF3, 0x38, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

#if (SSD1306_LCDHEIGHT != 48)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif
#endif

/****************** DISPLAY User Config ***************************/
int Align;
unsigned long previousMillis = 0;        // will store last time OLED was updated
// constants won't change :
const long interval = 3000;           // interval at which to update (milliseconds)
bool blinker = false;
float ExtVcc = 0;
int16_t got_time;
// Generally, you should use "unsigned long" for variables that hold time
// The value will quickly become too large for an int to store



#ifdef RADIO_IN_USE
/****************** RADIO User Config ***************************/
/***      Set this radio as radio number 0 or 1         ***/
bool radioNumber = 1;
/* Hardware configuration: Set up nRF24L01 radio on SPI bus plus pins CE and CSN*/
RF24 radio(D4, D8); //CE - CSN   ( D4, D8 for Wemos )
byte addresses[][6] = {"1Node", "2Node", "2Node"};
// Used to control whether this node is sending or receiving
bool role = 0; // 1=sending  0=receiving
/**********************************************************/
#endif


#ifdef MPU6050_IN_USE
/****************** MPU ***************************/
// MPU6050 class default I2C address is 0x68
// specific I2C addresses may be passed as a parameter here
// AD0 low = 0x68 (default for SparkFun breakout and InvenSense evaluation board)
// AD0 high = 0x69
MPU6050 mpu; // <-- use for AD0 low or unconnected
//MPU6050 mpu(0x69); // <-- use for AD0 high

unsigned long StartTimeout = 3000;           // Timout waiting for instruction (milliseconds)
// MPU control / status vars
bool dmpReady = false;  // set true if DMP init was successful
uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
uint16_t fifoCount;     // count of all bytes currently in FIFO
uint8_t fifoBuffer[64]; // FIFO storage buffer
uint16_t intTemp;       // internal temperature
float myTemp; // internal temperature in CÂ°

// orientation/motion vars
Quaternion q;           // [w, x, y, z]         quaternion container
VectorInt16 aa;         // [x, y, z]            accel sensor measurements
VectorInt16 aaReal;     // [x, y, z]            gravity-free accel sensor measurements
VectorInt16 aaWorld;    // [x, y, z]            world-frame accel sensor measurements
VectorFloat gravity;    // [x, y, z]            gravity vector
float euler[3];         // [psi, theta, phi]    Euler angle container
float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector

// packet structure for InvenSense teapot demo
uint8_t teapotPacket[14] = { '$', 0x02, 0, 0, 0, 0, 0, 0, 0, 0, 0x00, 0x00, '\r', '\n' };
uint8_t compassPacket[14] = { '£', 0x02, 0, 0, 0, 0, 0, 0, 0, 0, 0x00, 0x00, '\r', '\n' };
float heading = 0;
float compHeading = 0;
float prev_heading = 0;
float prev_compHeading = 0;
float prev_prev_heading = 0;
float prev_prev_compHeading = 0;

/*************************************** MPU CALIBRARTION *******************************************/
// default for offsets if json file is not available in FileSystem
long xAccelOff = -1000;
long yAccelOff = -800;
long zAccelOff = 1700;
long xGyroOff = 100;
long yGyroOff = -10;
long zGyroOff = 10;

int16_t ax, ay, az;
int16_t gx, gy, gz;
int count = 0;

boolean rough = true;
boolean medium = false;
boolean fine = false;
int adjustType = 1; // 1 = x accel, 2 = y accel, 3 = z accel, 4 = x gyro, 5 = y gyro, 6 = z gyro
const int countMax = 200; // number of iterations to check for settling of axes
int errorCheck = 5; // this value gets checked against the average of countMax number of samples, should be as close to 0 as possib

long readings[countMax];
long sum = 0;
long avg = 0;

/***************************************  HCL ***************************************************/
#ifdef HCM5883L_IN_USE
// I2Cdev and HMC5883L must be installed as libraries, or else the .cpp/.h files
// for both classes must be in the include path of your project
//#include "I2Cdev.h"
#include "HMC5883L.h"   // DOC ==> http://www.i2cdevlib.com/docs/html/class_h_m_c5883_l.html

// class default I2C address is 0x1E
// specific I2C addresses may be passed as a parameter here
// this device only supports one I2C address (0x1E)
HMC5883L mag;
#endif

int16_t mx, my, mz;
int counter1 = 0;
int counter2 = 0;

#endif

volatile bool mpuInterrupt = false;     // indicates whether MPU interrupt pin has gone high
volatile bool cmpInterrupt = false;     // indicates whether CMPASS interrupt pin has gone high


/***************************************  SOFTWARE RESET FUNZION DECLARATION **************************************/
void(* resetFunc) (void) = 0;//declare reset function at address 0

