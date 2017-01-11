/********************************************************************* I2Cscan() ************************************************************************/

#ifdef DEBUG
void I2Cscan()
{
  byte error, address;
  int nDevices;

  if (telnetServer.hasClient()) {
    if (!serverClient || !serverClient.connected()) {
      if (serverClient) {
        serverClient.stop();
        Serial.println("Telnet Client Stop");
      }
      serverClient = telnetServer.available();
      Serial.println("New Telnet client");
      serverClient.flush();  // clear input buffer, else you get strange characters
    }
  }
  
  MY_SERIAL.println(F("********* INFO FROM I2C SCANNER *********"));
  MY_SERIAL.println(F("Scanning..."));
  nDevices = 0;
  for (address = 1; address < 127; address++ )
  {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0)
    {
      MY_SERIAL.print("I2C device found at address 0x");
      if (address < 16)
        Serial.print("0");
      MY_SERIAL.print(address, HEX);
      MY_SERIAL.println("  !");
      nDevices++;
    }
    else if (error == 4)
    {
      MY_SERIAL.print("Unknow error at address 0x");
      if (address < 16)
        MY_SERIAL.print("0");
      MY_SERIAL.println(address, HEX);
    }
  }
  if (nDevices == 0)
    MY_SERIAL.println("No I2C devices found\n");
  else
    MY_SERIAL.println("done\n");
}
#endif

/******************************************************************* ESP_INFO() **************************************************************************/

#ifdef DEBUG
void ESP_INFO() {
  Serial.println();
  Serial.println("********  INFO FROM ESP_INFO  *******");
  Serial.println();
  Serial.print( "ESP.getVcc() :");
  Serial.println(        ESP.getVcc());
  Serial.print("ESP.getFreeHeap() :");
  Serial.println(        ESP.getFreeHeap());
  Serial.print("ESP.getChipId() :");
  Serial.println(        ESP.getChipId());
  Serial.print("ESP.getSdkVersion() :");
  Serial.println(        ESP.getSdkVersion());
  Serial.print("ESP.getBootVersion() :");
  Serial.println(       ESP.getBootVersion());
  Serial.print("ESP.getBootMode() :");
  Serial.println(       ESP.getBootMode());
  Serial.print("ESP.getCpuFreqMHz() :" );
  Serial.println(        ESP.getCpuFreqMHz());
  Serial.print ("ESP.getFlashChipId() :");
  Serial.println(       ESP.getFlashChipId());
  Serial.print ("ESP.getFlashChipSize() :");
  Serial.println(        ESP.getFlashChipSize());
  Serial.print ("ESP.getFlashChipSpeed() :");
  Serial.println(        ESP.getFlashChipSpeed());
  Serial.print ("ESP.getFlashChipMode() :");
  Serial.println(        ESP.getFlashChipMode());
  Serial.print ("ESP.getFlashChipSizeByChipId() :");
  Serial.println(        ESP.getFlashChipSizeByChipId());
  Serial.print ("ESP.getCycleCount() :");
  Serial.println(        ESP.getCycleCount());
  Serial.println();

}
#endif

#ifdef DEBUG
#define LINE(name,val) Serial.print(name); Serial.print("\t"); Serial.println(val);
void compilationInfo()
{
  Serial.println(F("******* INFO FROM COMPILER **********"));
  // Compile file path (internal file of compilation proccess)
  LINE("__FILE__", __FILE__);
 // Current line (but in __FILE__)
  LINE("__LINE__", __LINE__);
  // Name of current function   
  LINE("__func__", __func__);
   // Date of compliation
  LINE("__DATE__", __DATE__);
  // Time of compilation
  LINE("__TIME__", __TIME__);
   // Version of comilator (avr-gcc) 
  LINE("__VERSION__", __VERSION__);
  // Counter
  LINE("__COUNTER__",__COUNTER__); // 0 
  LINE("__COUNTER__",__COUNTER__); // 1
  LINE("__COUNTER__",__COUNTER__); // 2
  Serial.print("CPU :  ");
   #if defined(__AVR_ATmega168__)
    // Arduino Diecimila and older
    LINE("__AVR_ATmega168__", __AVR_ATmega168__);
  #endif
  #if defined(__AVR_ATmega328P__)
    // Arduino Duemilanove and Uno
    LINE("__AVR_ATmega328P__", __AVR_ATmega328P__);
  #endif
  #if defined(__AVR_ATmega2560__)
    // Arduino Mega 2560
    LINE("__AVR_ATmega2560__", __AVR_ATmega2560__);
  #endif
   #if defined(__AVR_ATmega1280__)
    // Arduino Mega
    LINE("__AVR_ATmega1280__", __AVR_ATmega1280__);
  #endif
  #if defined(__AVR_ATmega32U4__)
    // Arduino Leonardo
    LINE("__AVR_ATmega32U4__", __AVR_ATmega32U4__);
  #endif
  #if defined(__SAM3X8E__)
    // Arduino Due
    LINE("__SAM3X8E__", __SAM3X8E__);
  #endif
   // Determine board type
  #if defined(__AVR_ATmega168__) || defined(__AVR_ATmega328P__)
    Serial.println("Regular Arduino");
  #elif defined(__AVR_ATmega2560__) || defined(__AVR_ATmega1280__)
    Serial.println("Arduino Mega"); 
  #elif defined(__AVR_ATmega32U4__)
    Serial.println("Arduino Leonardo");
  #elif defined(__SAM3X8E__)
    Serial.println("Arduino Due");
      #elif defined(__esp8266__)
    Serial.println("esp8266");
  #else
    Serial.println("Unknown");
  #endif
   Serial.println("Arduino IDE version : ");
    // Arduino SW version
  LINE("ARDUINO", ARDUINO);
  Serial.println("Oscillator Frequency : ");
    // Oscillator frequency
  LINE("F_CPU", F_CPU);
    /*
    Serial.print(F("part name :"));Serial.println(__PART_NAME__); Serial.print(F("  asm version :"));Serial.println(__AVRASM_VERSION__);
    Serial.print(F("  core version :"));Serial.println(__CORE_VERSION__);
    Serial.print(F("AVR32_PM_OSCCTRL0_MODE_EXT_CLOCK :"));Serial.print(AVR32_PM_OSCCTRL0_MODE_EXT_CLOCK);
  */
}
#endif

