/****************************************************************************************************
                                 OTA SECTION
 ****************************************************************************************************/

#ifdef OTA_IN_USE
void WeMosOTAsetup() {
#ifdef DEBUG
  delay(500);
  Serial.println();
  Serial.println(F("*********  INFO FROM OTA SETUP ************* "));
  Serial.print(F("Connecting as wifi client to SSID: "));
  Serial.println(ssid);
#endif
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
#ifdef DEBUG
  WiFi.printDiag(Serial);
  WiFi.macAddress(MAC_array);
  Serial.print (F("MAC ADDRESS  : "));
  for (int i = 0; i < sizeof(MAC_array); ++i) {
    sprintf(MAC_char, "%s%02x:", MAC_char, MAC_array[i]);
  }
  Serial.println(MAC_char);
#endif
  /*
    // E' L'OPPOSTO DI QUELLA SOTTO
    while (WiFi.status() != WL_CONNECTED) {
      delay(100);
      Serial.print(".");
    }
  */



  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println(F("Connection Failed! Rebooting..."));
    delay(5000);
    ESP.restart();
  }

  Serial.print("Pinging host ");
  Serial.println(remote_host);

  // First parameter host name ( const char* ) or IP address ( IPAddress )
  if (Ping.ping(remote_host, 2)) {
    Serial.print (F("Success!!    Time:"));
    Serial.println(Ping.averageTime());
  } else {
    Serial.println(F("Error :("));
  }

  delay(4000);
  //Port defaults to 8266
  //ArduinoOTA.setPort(8266);

  //Hostname defaults to esp8266-[ChipID]
  //ArduinoOTA.setHostname("myesp8266");

  //No authentication by default
  ArduinoOTA.setPassword((const char *)"123");

  ArduinoOTA.onStart([]() {
    Serial.println("Start");
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.print("Progress: ");
    Serial.println(progress / (total / 100));
    //   Serial.printf("Progress: %u%%\r", (progress / (total / 100)));  //Serial.printf is not compatible with RF24 library
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.print("Error: ");
    Serial.println(error);
    //  Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}
#endif

/****************************************************************************************************
                                 TELNET SECTION
 ****************************************************************************************************/



#ifdef TELNET_DATA_MODE
void telnetSetup() {
  telnetServer.begin();
  telnetServer.setNoDelay(true);
  Serial.println("Please connect Telnet Client, exit with ^] and 'quit'");
  Serial.println("Use 'b' to beep the buzer");
}
#endif



#ifdef TELNET_DATA_MODE
void telnetLoop() {
  // look for Client connect trial
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
  while (serverClient.available()) { // get data from Client
    byte MyByte = serverClient.read();
#ifdef DEBUG
    Serial.write(MyByte);
#endif
    if ( MyByte == 98 ) // "b"
    {
#ifndef PCF8574_IN_USE
      digitalWrite(BUILTIN_LED2, !digitalRead(BUILTIN_LED2));
#endif
#ifdef PCF8574_IN_USE
      expBlink(6, 4, 100);
      serverClient.println("Received telnet command 'b' ");
#endif
    }
  }
  //   if (millis() - telnetStartTime > telnetInterval) { // run every xxx ms
  //     if (serverClient && serverClient.connected()) {  // send data to Client
  //      serverClient.print("Telnet Test, millis from last sending: ");
  //      serverClient.println(millis() - telnetStartTime);
  //      //serverClient.print("Free Heap RAM: ");
  //      //serverClient.println(ESP.getFreeHeap());
  //    }
  //    telnetStartTime = millis();
  //  }
  delay(10); // to avoid strange characters left in buffer
}
#endif




