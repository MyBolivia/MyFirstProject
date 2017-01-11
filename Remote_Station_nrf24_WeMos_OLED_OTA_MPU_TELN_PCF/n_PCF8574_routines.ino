
/**********  Interrupt routine - tested but with D3 but NOT USED with WeMos ( no pins available )************/

#ifdef PCF8574_IN_USE
#ifdef PCF8574_INTERRUPT
void PCFInterrupt() {
  MyFlag = true;
  Serial.println("NOW interrupt !");
  detachInterrupt(digitalPinToInterrupt(PCF_INTERRUPT_PIN));
  myPcf.write(6, !myPcf.read(6));
  //myPcf.toggle(3);
  attachInterrupt(digitalPinToInterrupt(PCF_INTERRUPT_PIN), PCFInterrupt, FALLING);
}
#endif
#endif

/************************************************************************************/

#ifdef PCF8574_IN_USE
void expBlink (uint8_t expPin, int expBlinks, int expDelay) {
  for (int blinks = 0; blinks <= expBlinks; blinks++) {
    myPcf.write(expPin, LOW);
    delay(expDelay / 2);
    myPcf.write(expPin, HIGH);
    delay(expDelay);
  }
}
#endif

/************************************************************************************/

#ifdef PCF8574_IN_USE
void pcfSetup() {
#ifdef DEBUG
  Serial.print(F("PCF8574 inizializated at address ......"));
  Serial.print(PCF8574_ADDRESS);
  Serial.print(F("....... ......"));
#endif

#ifdef PCF8574_INTERRUPT
  pinMode(PCF_INTERRUPT_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(PCF_INTERRUPT_PIN), PCFInterrupt, FALLING);
  Serial.print(F("....ON INTERRUPT ...."));
#endif
  for (uint8_t pin = 0; pin <= 3; pin++) {
    myPcf.write(pin, HIGH);
  }
  myPcf.write(6, HIGH);
  Serial.println();

  for (uint8_t pin = 0; pin <= 3; pin++) {
    expBlink(pin, 3, 100);
  }
  expBlink(6, 2, 100);

  myPcf.write(expMode, LOW);
}
#endif

/************************************************************************************/

#ifdef PCF8574_IN_USE
void print_binary(int v, int num_places) {
  int mask = 0, n;
  for (n = 1; n <= num_places; n++)
  {
    mask = (mask << 1) | 0x0001;
  }
  v = v & mask;  // truncate v to specified number of places
  while (num_places)
  {
    if (v & (0x0001 << num_places - 1)) {
      Serial.print("1");
    }
    else {
      Serial.print("0");
    }
    --num_places;
    if (((num_places % 4) == 0) && (num_places != 0))
    {
      Serial.print("_");
    }
  }
}
#endif

/************************************************************************************/

#ifdef PCF8574_IN_USE
// Any pin you wish to use as input must be written HIGH and be pulled LOW to generate an interrupt.
void pcfReadLoop() {
  //Serial.println(digitalRead(PCF_INTERRUPT_PIN));
#ifdef PCF8574_INTERRUPT
  if (MyFlag) {
    MyFlag = false;
#ifdef DEBUG
    Serial.println(F("interr loop ==> "));
#endif
  }
  else {
#ifdef DEBUG
    Serial.print(F("normal loop ==> "));
#endif
  }
#endif
  uint8_t value = myPcf.read(EXPANDER_BUTTON_PIN);
  if (value != oldValue) {
//#ifdef DEBUG
//    Serial.print("PCF pin status changed :  ");
//    Serial.println(value);
//#endif
    //print_binary(value, 8);
    oldValue = value;

    //myPcf.toggle(1);
    if (value) {
      if ( expMode < 3 ) {
        expMode++;
      }
      else {
        expMode = 0;
      }
      for (uint8_t pin = 0; pin <= 3; pin++) {
        myPcf.write(pin, HIGH);
      }
      myPcf.write(expMode, LOW);
      #ifdef DEBUG
          expBlink(6, 0, 100);
          
    MY_SERIAL.print("MODE changed :  ");
    MY_SERIAL.println(expMode);

#endif
    }

  }
}
#endif

/************************************************************************************/





