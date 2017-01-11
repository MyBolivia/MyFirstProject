
void loop() {
#ifdef OTA_IN_USE
  ArduinoOTA.handle(); // this allow OTA sketch uploads from WiFi
#endif

#ifdef ANALOG_IN_USE  // Analog Input is not compatible with OTA !!!!!
  ExtVcc = map(analogRead(A0), 0, 1024, 0, 3100);
  ExtVcc =  ExtVcc / 1000 * 2;
#endif

  // ================================================================
  // ===                     IN LOOP DEBUG                        ===
  // ================================================================
#ifdef IN_LOOP_DEBUG
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;
  #ifdef ANALOG_IN_USE
      Serial.print ("Analog Read : ");
      Serial.print (analogRead(A0));
      Serial.print ("  ExtVcc: ");
      Serial.println(ExtVcc);
  #endif
  #ifndef ANALOG_IN_USE
      Serial.print("int V: ");
      float My_getVcc = ESP.getVcc(); // to activate this function uncomment ADC_MODE(ADC_VCC) in the declarations
      float My_intVcc = My_getVcc / 1000;
      Serial.println(My_intVcc);
  #endif
  #ifdef ANALOG_IN_USE
      Serial.print("Time Stamp: ");
      Serial.print (currentMillis);
      Serial.print (" My Current IP: ");
      Serial.println(WiFi.localIP());
  #endif

#ifdef OLED_IN_USE
    TextDisplay(String(got_time));
#endif
  }
#endif

#ifdef MPU6050_IN_USE
#ifdef HCM5883L_IN_USE
  // ================================================================
  // ===                     NAVIGATION SECTION                   ===
  // ================================================================

  IMUtestConnection();
  IMUscan();
#endif
#endif
  // ================================================================
  // ===                     RADIO   SECTION                      ===
  // ================================================================
#ifdef RADIO_IN_USE
  if (role == 1)  {           // =============>>>     Ping Out Role
    RadioPingRole();
  }
  if ( role == 0 )            // =============>>>     Pong Back Role - DEFAULT FOR REMOTE STATION
  {
    RadioPongRole();
  }

  if ( Serial.available() )  // =============>>>      Change Roles via Serial Commands
  {
    char c = toupper(Serial.read()); // riconverte tutto al maiuscolo
    if ( c == 'T' && role == 0 ) {
      Serial.println(F("*** CHANGING TO TRANSMIT ROLE -- PRESS 'R' TO SWITCH BACK"));
      role = 1;                  // Become the primary transmitter (ping out)
    } else if ( c == 'R' && role == 1 ) {
      Serial.println(F("*** CHANGING TO RECEIVE ROLE -- PRESS 'T' TO SWITCH BACK"));
      role = 0;                // Become the primary receiver (pong back)
      radio.startListening();
    }
  }
#endif
  // ================================================================
  // ===                     PCF8574  SECTION                      ===
  // ================================================================
#ifdef PCF8574_IN_USE  
pcfReadLoop();
#endif

  // ================================================================
  // ===                     TELNET  SECTION                      ===
  // ================================================================
#ifdef TELNET_DATA_MODE
telnetLoop();
#endif

}
