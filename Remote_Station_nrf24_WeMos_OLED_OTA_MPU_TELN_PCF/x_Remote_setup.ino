
void setup() {
  // join I2C bus (I2Cdev library doesn't do this automatically)
  //    #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
  Serial.begin(74880); // default baudrate to see the boot messages
  //Wire.setClock(400000); // 400kHz I2C clock. Comment this line if having compilation difficulties
  //    #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
  //        Fastwire::setup(400, true);
  //    #endif
  Wire.begin();
#ifdef OTA_IN_USE
  WeMosOTAsetup();
#endif

#ifdef TELNET_DATA_MODE
telnetSetup();
Serial.println(F("PLEASE START TELNET TERMINAL"));
delay(5000);
serverClient.println(F("PLEASE START TELNET TERMINAL"));
#endif

#ifdef DEBUG
  ESP_INFO();
  I2Cscan();
#endif

#ifdef PCF8574_IN_USE    
  pcfSetup();
#endif


#ifdef OLED_IN_USE
  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 64x48 WeMos with default DC jumper)
  // init done
  display.display();
  delay(10);
  // Clear the buffer.
  display.clearDisplay();
  // Load the buffer with a new image. To convert bmp image in HEX image use
  // "LCFassistant" program, setting "vertical" , 64x48 , 8 pixels per byte
  //display.drawBitmap(Xpos, Ypos, imagevector, imageHeight, imageWidth, WHITEorBLACK);
  display.drawBitmap(0, 0, mylogo_glcd_bmp, MYLOGO_GLCD_HEIGHT, MYLOGO_GLCD_WIDTH, WHITE);
  //display.drawBitmap(0, 0, logo16_glcd_bmp, LOGO16_GLCD_HEIGHT, LOGO16_GLCD_WIDTH, WHITE);
  // Show image buffer on the display hardware.
  display.display();
  delay(200);
  // invert the display
  display.invertDisplay(true);
  delay(200);
  display.invertDisplay(false);
  delay(200);
  display.clearDisplay();
  display.display();
  delay(500);
  TextDisplay("0");
#endif


#ifdef DEBUG
  compilationInfo();
#endif

#ifdef RADIO_IN_USE
  Serial.println(F("This station is acting as receiving station, sending back informations as feedback"));
  Serial.println(F("*** PRESS 'T--' to begin transmitting to the other node"));

  radio.begin();

  // Set the PA Level low to prevent power supply related issues since this is a
  // getting_started sketch, and the likelihood of close proximity of the devices. RF24_PA_MAX is default.
  radio.setPALevel(RF24_PA_LOW);

  // Open a writing and reading pipe on each radio, with opposite addresses
  if (radioNumber) {
    // RADIO 1 <========================================
    radio.openWritingPipe(addresses[1]);
    radio.openReadingPipe(1, addresses[0]);
  } else {
    // RADIO 0
    radio.openWritingPipe(addresses[0]);
    radio.openReadingPipe(1, addresses[1]);
  }

  // Start the radio listening for data
  radio.startListening();
#endif

#ifdef MPU6050_IN_USE
  FileSystemSetup();
  mpuSetup();
#endif
#ifdef HCM5883L_IN_USE
  hclSetup() ;
#endif

#ifdef MPU6050_IN_USE
#ifndef AUTOSTART // start ifndef AUTOSTART
  byte inByte;
  // wait for ready
  unsigned long NowMillis = millis();
  Serial.println(F("\nSend any character to begin  ( \"c\" for calibrate MPU6050, otherwise json file saved value or default value will be loaded and set) : "));
  while (Serial.available() && Serial.read()); // empty buffer
  //while (!Serial.available());
  while (!Serial.available() && (millis() < (NowMillis + StartTimeout)));                 // wait for data or timeout
  inByte = Serial.read();
#ifdef MY_SETUP_DEBUG
  Serial.println (inByte);
#endif
  while (Serial.available() && Serial.read()); // empty buffer again
  if (inByte == 99 ) {
    startCalibration();
  }
#endif // end of "#ifdef AUTOSTART "
  mpuOffsetSetting();


#ifdef MY_SETUP_DEBUG
  Serial.println(F("START"));
#endif

#ifdef INTERRUPT_PIN_MPU6050
  SetInterruptMPU6050();
#endif
  dmpReady = true;

  // get expected DMP packet size for later comparison
  packetSize = mpu.dmpGetFIFOPacketSize();
#endif



}
