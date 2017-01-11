
/*************************************************************** TextDisplay(String MyMessage) **********************************************************************/

#ifdef OLED_IN_USE
void TextDisplay(String MyMessage) {
  // text display tests
  display.setTextSize(1);
  blinker = !blinker;
  if (blinker) {
    display.setTextColor(BLACK, WHITE); // 'inverted' textdisplay.setTextColor(WHITE);
  }
  else {
    display.setTextColor(WHITE);
  }
  display.setCursor(5, 0);
  display.println("ENCODER:");
  display.setTextColor(WHITE);
  display.setTextSize(2);
  Align = ( 64 - ( MyMessage.length() * 12)) / 2;
  display.setCursor(Align, 14);
  display.println(MyMessage);
  //display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 30);
#ifndef ANALOG_IN_USE
  display.print("int V:");
  float getVcc = ESP.getVcc(); // to activate this function uncomment ADC_MODE(ADC_VCC) in the declarations
  float intVcc = getVcc / 1000;
  display.print(intVcc);
#endif
#ifdef ANALOG_IN_USE
  display.print("bat V:");
  display.print(ExtVcc); // ADC input doesn't works if you uncomment ADC_MODE(ADC_VCC) in the declarations
#endif
  IPAddress  MyIP = WiFi.localIP();
  display.setCursor(0, 40);
  display.print("IP : ");
  display.print(MyIP[3]);
  display.display();
  display.clearDisplay();
}
#endif



/*************************************************************** hclSetup()  **********************************************************************/

#ifdef HCM5883L_IN_USE
void hclSetup() {
#ifdef MY_SETUP_DEBUG
  Serial.println(F("Initializing HCM5883L I2C devices..."));
#endif
  mag.initialize();
#ifdef INTERRUPT_PIN_HCM5883L
  pinMode(INTERRUPT_PIN_HCM5883L, INPUT);
#endif
#ifdef MY_SETUP_DEBUG
  Serial.println(F("Testing HMC5883L connections..."));
  Serial.println(mag.testConnection() ? "HMC5883L connection successful" : "HMC5883L connection failed");
#endif
}
#endif



/*************************************************************** hclSetup()  **********************************************************************/

#ifdef INTERRUPT_PIN_HCM5883L
void SetInterruptHCL588L(){
    // enable Arduino interrupt detection of HCM5883L
#ifdef MY_SETUP_DEBUG
  Serial.println(F("Enabling interrupt detection (Arduino external interrupt 0)..."));
#endif
  attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN_HCM5883L), cmpDataReady, FALLING);
}
#endif


/*****************************************************************************/

//Funzione per il calcolo degli angoli Pitch e Roll
double FunctionsPitchRoll(double A, double B, double C) {
  double DatoA, DatoB, Value;
  DatoA = A;
  DatoB = (B * B) + (C * C);
  DatoB = sqrt(DatoB);

  Value = atan2(DatoA, DatoB);
  //Value = Value * 180/3.14;
  //return (int)Value;
  return Value;
}

/*****************************************************************************/

float Bytes2float( byte* bytes_array ) {    //,float val

  union {
    byte b[4];
    float fval;
  } u;
  u.b[0] = bytes_array[0];
  u.b[1] = bytes_array[1];
  u.b[2] = bytes_array[2];
  u.b[3] = bytes_array[3];
  float val = u.fval;
  return val;

}
/*****************************************************************************/

void float2Bytes(float val, byte* bytes_array) {
  // Create union of shared memory space
  union {
    float float_variable;
    byte temp_array[4];
  } u;
  // Overite bytes of union with float variable
  u.float_variable = val;
  // Assign bytes to input array
  memcpy(bytes_array, u.temp_array, 4);
}

/*****************************************************************************/

#ifdef HCM5883L_IN_USE
void myCompass() {
  // read raw heading measurements from device
  mag.getHeading(&mx, &my, &mz);
  // To calculate heading in degrees. 0 degree indicates North
  heading = atan2(my, mx);
  if (heading < 0) heading += 2 * M_PI;
  heading = heading * 180 / M_PI;
#ifdef COMPASS_READABLE_HEADING
  Serial.print("  raw   mag:\t mx= ");
  Serial.print(mx); Serial.print("\t my= ");
  Serial.print(my); Serial.print("\t mz= ");
  Serial.print(mz); Serial.print("\t");
  Serial.print("raw  hdg (y,x): ");
  Serial.print(heading );
  Serial.print("    ");
#endif
#ifdef COMPASS_TILT_COMPENSATED
  // to compensate tilt using Accellerometers
  int16_t ax, ay, az;
  mpu.getAcceleration(&ax, &ay, &az);
  int16_t tx = ax;
  int16_t ty = ay;
  int16_t tz = az;
  float roll = FunctionsPitchRoll(tx, ty, tz);
  float pitch = FunctionsPitchRoll(ty, tx, tz);
  float Xh = 0;
  float Yh = 0;
  Xh = mx * cos(pitch) + my * sin(roll ) * sin(pitch) - mz * cos(roll ) * sin(pitch);
  Yh = my * cos(roll ) + mz * sin(roll );
  // To calculate heading in degrees. 0 degree indicates North
  compHeading = atan2(Yh, Xh);
  if (compHeading < 0) compHeading += 2 * M_PI;
  compHeading = compHeading * 180 / M_PI;

#ifdef COMPASS_READABLE_HEADING
  Serial.print("acc:\t");
  Serial.print(tx); Serial.print("\t");
  Serial.print(ty); Serial.print("\t");
  Serial.print(tz); Serial.print("\t");
  Serial.print("pitch : "); Serial.print(pitch);
  Serial.print("     roll : "); Serial.println(roll);
  Serial.print("  comp. mag:\t Xh= ");
  Serial.print(int(Xh)); Serial.print("\t Yh= ");
  Serial.print(int(Yh)); Serial.print("\t\t\t");
  Serial.print("comp.Hdg (y,x): ");
  Serial.print(compHeading);
  Serial.println("");
#endif
#endif

#ifdef AVERAGED_HEADING
int ave_heading = (heading + prev_heading + prev_prev_heading ) /3;
int ave_compHeading = (compHeading + prev_compHeading + prev_prev_compHeading ) /3;
prev_prev_heading = prev_heading;
heading= ave_heading;
prev_heading = heading;
prev_prev_compHeading = prev_compHeading;
prev_compHeading = compHeading;
compHeading = ave_compHeading;
#endif
#ifdef COMPASS_PACKET
  byte bytes[4];
  float2Bytes(heading, &bytes[0]);
  compassPacket[2] = bytes[0];
  compassPacket[3] = bytes[1];
  compassPacket[4] = bytes[2];
  compassPacket[5] = bytes[3];
  /*
    #ifdef COMPASS_READABLE_HEADING 
    Serial.print("compHeading: ");
    Serial.print(compHeading);
    Serial.print("\t");
    Serial.print(bytes[3], HEX);
    Serial.print("\t");
    Serial.print(bytes[2], HEX);
    Serial.print("\t");
    Serial.print(bytes[1], HEX);
    Serial.print("\t");
    Serial.print(bytes[0], HEX);
    Serial.print("that means... ");
    Serial.print(Bytes2float(&bytes[0]));
    Serial.println("");
    #endif
  */

#ifdef COMPASS_TILT_COMPENSATED
  float2Bytes(compHeading, &bytes[0]);
  compassPacket[6] = bytes[0];
  compassPacket[7] = bytes[1];
  compassPacket[8] = bytes[2];
  compassPacket[9] = bytes[3];
#endif
  /*
    #ifdef COMPASS_READABLE_HEADING
    Serial.print("heading: ");
    Serial.print(heading);
    Serial.print("\t");
    Serial.print(bytes[3], HEX);
    Serial.print("\t");
    Serial.print(bytes[2], HEX);
    Serial.print("\t");
    Serial.print(bytes[1], HEX);
    Serial.print("\t");
    Serial.print(bytes[0], HEX);
    Serial.print("that means... ");
    Serial.print(Bytes2float(&bytes[0]));
    Serial.println("\t");
    #endif
  */
  Serial.write(compassPacket, 14);
  compassPacket[11]++; // packetCount, loops at 0xFF on purpose
#endif
}
#endif

/**********************************  IMUtestConnection ***************************************/

void IMUtestConnection(){
  // if programming failed, don't try to do anything
#ifdef MPU6050_IN_USE
  if (!dmpReady) {
    Serial.println("RETURN BECAUSE OF MPU6050.......................");
    delay(5000);
    return;
  }
#endif
#ifdef HCM5883L_IN_USE
  if (!mag.testConnection()) {
    Serial.println("RETURN BECAUSE OF HCM5883L.......................");
    delay(5000);
    return;
  }
#endif
}

/**********************************  IMUscan ***************************************/

void IMUscan(){
#ifdef MY_DEBUG
  Serial.print(counter1);
  Serial.print("< 1 > ");
#endif
  // wait for MPU interrupt or extra packet(s) available
#ifdef INTERRUPT_PIN_MPU6050
  while ((!mpuInterrupt && fifoCount < packetSize)) {
#endif
    // other program behavior stuff here
    // .
    // .
    // .
    // if you are really paranoid you can frequently test in between other
    // stuff to see if mpuInterrupt is true, and if so, "break;" from the
    // while() loop to immediately process the MPU data
    // .
    // .
    // .
#ifdef INTERRUPT_PIN_MPU6050
  }
#endif

#ifdef INTERRUPT_PIN_MPU6050
  if (mpuInterrupt) {
#endif
#ifdef MY_DEBUG
    Serial.print(counter2);
    Serial.print("< 2 > ");
#endif
#ifdef HCM5883L_IN_USE
    myCompass();
#endif
#ifdef MPU6050_IN_USE
    myMPU();
#endif
    //Serial.println("*******");
    //mpuInterrupt = false;
#ifdef MY_DEBUG
    counter2++;
#endif
#ifdef INTERRUPT_PIN_MPU6050
  }
#endif
#ifdef MY_DEBUG
  counter1++;
#endif
}



