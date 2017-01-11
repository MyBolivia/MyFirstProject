
#ifdef RADIO_IN_USE
#include <SPI.h>
#include "RF24.h"
#endif

/*************************   OLED Setup  *************************/
#ifdef OLED_IN_USE
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#endif

/************************** OTA setup **************************/

#ifdef OTA_IN_USE       // if you are using OTA, connect PC to Serial Monitor via USB port using RTTY ( Serial ) instead of Arduino IDE
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <ESP8266Ping.h>
#endif

/************************** PCF 8574 setup **************************/

#ifdef PCF8574_IN_USE
#include <pcf8574_esp.h>
#endif

/********************** DATA COMUNICATION Setup ****************/
#ifndef OTA_IN_USE
#ifdef TELNET_DATA_MODE
#include <ESP8266WiFi.h>
#endif
#endif
/********************** MPU - HCM *******************************/

#ifdef MPU6050_IN_USE
// I2Cdev and MPU6050 must be installed as libraries, or else the .cpp/.h files
// for both classes must be in the include path of your project
//#define DEBUG // this enable the Serial MOnitor debug of MPU6050 configuration library routine ONLY
#include "MPU6050_6Axis_MotionApps20.h"  //   DOC ==> http://www.i2cdevlib.com/docs/html/class_m_p_u6050.html
//#include "MPU6050.h" // not necessary if using MotionApps include file
#endif
/**************************************************************************************/
// ================================================================
// ===         CONFIG / FILESYSTEM  INITIAL DECLARATIONS        ===
// ================================================================
#ifdef MPU6050_IN_USE
#include <ArduinoJson.h>
#include "FS.h"
#endif
