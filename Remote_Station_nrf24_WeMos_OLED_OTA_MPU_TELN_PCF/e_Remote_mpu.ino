/*************************************************************** FileSystemSetup() **********************************************************************/

// Iniizialize Fyle System
#ifdef MPU6050_IN_USE
void FileSystemSetup() {
#ifdef MY_SETUP_DEBUG
  Serial.println(F("Mounting FS..."));
#endif
  if (!SPIFFS.begin()) {
#ifdef MY_SETUP_DEBUG
    Serial.println(F("Failed to mount FileSystem"));
#endif
  }
#ifdef MY_SETUP_DEBUG
  else {
    Serial.println(F("FileSystem successfully mounted :"));
    if (!getInfo()) {
      Serial.println(F("Failed to obtain FileSystem infos"));
    } else {
      Serial.println("FileSystem infos obtained");
    }
    Serial.println(F("Listing Files in the FileSystem :"));
    Serial.print(listFiles ());
    Serial.println(F("  Files exists"));
    Serial.println("");
  }
#endif
}
#endif

/*************************************************************** mpuSetup()  **********************************************************************/

#ifdef MPU6050_IN_USE
// initialize device MPU6050 and load configuration from JSON
void mpuSetup() {
#ifdef MY_SETUP_DEBUG
  Serial.println(F("Initializing MPU6050 I2C devices..."));
#endif
  mpu.initialize();
#ifdef INTERRUPT_PIN_MPU6050
  pinMode(INTERRUPT_PIN_MPU6050, INPUT);
#endif
#ifdef MY_SETUP_DEBUG
  Serial.println(F("Testing MPU6050 connections..."));
  Serial.println(mpu.testConnection() ? F("MPU6050 connection successful") : F("MPU6050 connection failed"));
  Serial.print(F("MPU6050 Internal Temperature...  "));
  intTemp = ~mpu.getTemperature();
  myTemp = intTemp + 1;
  myTemp = myTemp / 340 + 36.53;  /////////////////// **** DA RIVEDERE *** //////////////////////
  Serial.println(intTemp);
  Serial.println(F("/////////////////// **** DA RIVEDERE *** //////////////////////"));
#ifndef DEBUG
  Serial.println(F("Initializing DMP......."));
#endif
#ifdef DEBUG
  Serial.println(F("Initializing DMP. ( MPU6050 LIBRARY DEBUG MESSAGES ==> #define DEBUG uncommented) "));
#endif
#endif
  devStatus = mpu.dmpInitialize();
#ifdef MY_SETUP_DEBUG
#ifdef DEBUG
  Serial.println(F("****** end of  MPU6050 LIBRARY DEBUG MESSAGES ******"));
  Serial.println("");
#endif
#endif
  // make sure it worked (returns 0 if so)
  if (devStatus != 0) {
#ifdef MY_SETUP_DEBUG
    Serial.println(F("devStatus is not 0 ! ............."));
    // ERROR!
    // 1 = initial memory load failed
    // 2 = DMP configuration updates failed
    // (if it's going to break, usually the code will be 1)
    Serial.print(F("DMP Initialization failed (code "));
    Serial.print(devStatus);
    Serial.println(F(")"));
    delay(3000);
    #endif
    resetFunc(); //call reset
    // will never exit from here , will restart
  }
  // turn on the DMP, now that it's ready
  #ifdef MY_SETUP_DEBUG
    Serial.println(F("Enabling DMP..."));
  #endif
  mpu.setDMPEnabled(true);
  if (! mpuOffsetLoading()) {
  #ifdef MY_SETUP_DEBUG
      Serial.println("Failed to load mpuConfig");
  #endif
  }
  #ifdef MY_SETUP_DEBUG
    else {
      Serial.println("mpuConfig loaded");
    }
  #endif
}
#endif

/*************************************************************** mpuSetup()  **********************************************************************/

#ifdef MPU6050_IN_USE
void calibrateMPU6050() {
  // read raw accel/gyro measurements from device
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  // these methods (and a few others) are also available
  //accelgyro.getAcceleration(&ax, &ay, &az);
  //accelgyro.getRotation(&gx, &gy, &gz);


  // display tab-separated accel/gyro x/y/z values
  /*if (adjustType == 6) {
    Serial.print("a/g:\t");
    Serial.print(ax); Serial.print("\t");
    Serial.print(ay); Serial.print("\t");
    Serial.print(az); Serial.print("\t");
    Serial.print(gx); Serial.print("\t");
    Serial.print(gy); Serial.print("\t");
    Serial.println(gz);
    }*/

  count++;

  if (adjustType == 3) { // z accel
    long val = az - 16384 ;
    // add the reading to the total:
    sum = sum + val;
    // calculate the average:
    avg = sum / count;
    if (count == countMax) {
#ifdef MY_SETUP_DEBUG
      Serial.print("Average reading of "); Serial.print(avg); Serial.print(" with z accel offset of "); Serial.println(zAccelOff);
#endif
      if (avg > errorCheck && rough) {
        mpu.setZAccelOffset(zAccelOff -= 100);
        count = 0;
        sum = 0;
      } else if (avg < -errorCheck && rough || avg < -errorCheck && medium) {
        rough = false;
        medium = true;
        mpu.setZAccelOffset(zAccelOff += 10);
        count = 0;
        sum = 0;
      } else if (avg > errorCheck && medium || avg > errorCheck && fine) {
        medium = false;
        fine = true;
        mpu.setZAccelOffset(zAccelOff -= 1);
        count = 0;
        sum = 0;
      }
    } else if (count > countMax) {
#ifdef MY_SETUP_DEBUG
      Serial.print("---> Converged z accel at: "); Serial.print(zAccelOff); Serial.print(" with average reading of: "); Serial.println(avg);
#endif
      adjustType++;
      count = 0;
      sum = 0;
      rough = false;
      medium = true;
      fine = false;
    }
  } else if (adjustType == 1) { // x accel
    long val = ax ;
    // add the reading to the total:
    sum = sum + val;
    // calculate the average:
    avg = sum / count;
    if (count == countMax) {
#ifdef MY_SETUP_DEBUG
      Serial.print("Average reading of "); Serial.print(avg); Serial.print(" with x accel offset of "); Serial.println(xAccelOff);
#endif
      if (avg > errorCheck && rough) {
        mpu.setXAccelOffset(xAccelOff -= 100);
        count = 0;
        sum = 0;
      } else if (avg < -errorCheck && rough || avg < -errorCheck && medium) {
        rough = false;
        medium = true;
        mpu.setXAccelOffset(xAccelOff += 10);
        count = 0;
        sum = 0;
      } else if (avg > errorCheck && medium || avg > errorCheck && fine) {
        medium = false;
        fine = true;
        mpu.setXAccelOffset(xAccelOff -= 1);
        count = 0;
        sum = 0;
      }
    } else if (count > countMax) {
#ifdef MY_SETUP_DEBUG
      Serial.print("---> Converged x accel at: "); Serial.print(xAccelOff); Serial.print(" with average reading of: "); Serial.println(avg);
#endif
      adjustType++;
      count = 0;
      rough = true;
      medium = false;
      fine = false;
      sum = 0;
    }
  } else if (adjustType == 2) { // y accel
    long val = ay ;
    // add the reading to the total:
    sum = sum + val;
    // calculate the average:
    avg = sum / count;
    if (count == countMax) {
#ifdef MY_SETUP_DEBUG
      Serial.print("Average reading of "); Serial.print(avg); Serial.print(" with y accel offset of "); Serial.println(yAccelOff);
#endif
      if (avg > errorCheck && rough) {
        mpu.setYAccelOffset(yAccelOff -= 100);
        count = 0;
        sum = 0;
      } else if (avg < -errorCheck && rough || avg < -errorCheck && medium) {
        rough = false;
        medium = true;
        mpu.setYAccelOffset(yAccelOff += 10);
        count = 0;
        sum = 0;
      } else if (avg > errorCheck && medium || avg > errorCheck && fine) {
        medium = false;
        fine = true;
        mpu.setYAccelOffset(yAccelOff -= 1);
        count = 0;
        sum = 0;
      }
    } else if (count > countMax) {
#ifdef MY_SETUP_DEBUG
      Serial.print("---> Converged y accel at: "); Serial.print(yAccelOff); Serial.print(" with average reading of: "); Serial.println(avg);
#endif
      adjustType++;
      count = 0;
      sum = 0;
      rough = true;
      medium = false;
      fine = false;
    }
  } else if (adjustType == 6) { // z gyro
    long val = gz;
    // add the reading to the total:
    sum = sum + val;
    // calculate the average:
    avg = sum / count;
    if (count == countMax) {
#ifdef MY_SETUP_DEBUG
      Serial.print("Average reading of "); Serial.print(avg); Serial.print(" with z gyro offset of "); Serial.println(zGyroOff);
#endif
      if (avg > errorCheck && rough) {
        mpu.setZGyroOffset(zGyroOff -= 100);
        count = 0;
        sum = 0;
      } else if (avg < -errorCheck && rough || avg < -errorCheck && medium) {
        rough = false;
        medium = true;
        mpu.setZGyroOffset(zGyroOff += 10);
        count = 0;
        sum = 0;
      } else if (avg > errorCheck && medium || avg > errorCheck && fine) {
        medium = false;
        fine = true;
        mpu.setZGyroOffset(zGyroOff -= 1);
        count = 0;
        sum = 0;
      }
    } else if (count > countMax) {
#ifdef MY_SETUP_DEBUG
      Serial.print("---> Converged z gyro at: "); Serial.print(zGyroOff); Serial.print(" with average reading of: "); Serial.println(avg);
#endif
      adjustType++;
      count = 0;
      sum = 0;
    }
  } else if (adjustType == 4) { // x gyro
    long val = gx ;
    // add the reading to the total:
    sum = sum + val;
    // calculate the average:
    avg = sum / count;
    if (count == countMax) {
#ifdef MY_SETUP_DEBUG
      Serial.print("Average reading of "); Serial.print(avg); Serial.print(" with x gyro offset of "); Serial.println(xGyroOff);
#endif
      if (avg > errorCheck && rough) {
        mpu.setXGyroOffset(xGyroOff -= 100);
        count = 0;
        sum = 0;
      } else if (avg < -errorCheck && rough || avg < -errorCheck && medium) {
        rough = false;
        medium = true;
        mpu.setXGyroOffset(xGyroOff += 10);
        count = 0;
        sum = 0;
      } else if (avg > errorCheck && medium || avg > errorCheck && fine) {
        medium = false;
        fine = true;
        mpu.setXGyroOffset(xGyroOff -= 1);
        count = 0;
        sum = 0;
      }
    } else if (count > countMax) {
#ifdef MY_SETUP_DEBUG
      Serial.print("---> Converged x gyro at: "); Serial.print(xGyroOff); Serial.print(" with average reading of: "); Serial.println(avg);
#endif
      adjustType++;
      count = 0;
      sum = 0;
      rough = false;
      medium = true;
      fine = false;
    }
  } else if (adjustType == 5) { // y gyro
    long val = gy ;
    // add the reading to the total:
    sum = sum + val;
    // calculate the average:
    avg = sum / count;
    if (count == countMax) {
#ifdef MY_SETUP_DEBUG
      Serial.print("Average reading of "); Serial.print(avg); Serial.print(" with y gyro offset of "); Serial.println(yGyroOff);
#endif
      if (avg > errorCheck && rough) {
        mpu.setYGyroOffset(yGyroOff -= 100);
        count = 0;
        sum = 0;
      } else if (avg < -errorCheck && rough || avg < -errorCheck && medium) {
        rough = false;
        medium = true;
        mpu.setYGyroOffset(yGyroOff += 10);
        count = 0;
        sum = 0;
      } else if (avg > errorCheck && medium || avg > errorCheck && fine) {
        medium = false;
        fine = true;
        mpu.setYGyroOffset(yGyroOff -= 1);
        count = 0;
        sum = 0;
      }
    } else if (count > countMax) {
#ifdef MY_SETUP_DEBUG
      Serial.print("---> Converged y gyro at: "); Serial.print(yGyroOff); Serial.print(" with average reading of: "); Serial.println(avg);
#endif
      adjustType++;
      count = 0;
      rough = false;
      medium = true;
      fine = false;
    }
  } else if (adjustType == 7) {
#ifdef MY_SETUP_DEBUG
    Serial.println(" ");
    Serial.println("Calculated offsets:");
    Serial.print(" x accel: "); Serial.println(xAccelOff);
    Serial.print(" y accel: "); Serial.println(yAccelOff);
    Serial.print(" z accel: "); Serial.println(zAccelOff);
    Serial.print(" x gyro: "); Serial.println(xGyroOff);
    Serial.print(" y gyro: "); Serial.println(yGyroOff);
    Serial.print(" z gyro: "); Serial.println(zGyroOff);
#endif
    adjustType++;
  }
}
#endif

/*************************************************************** mpuSetup()  **********************************************************************/

#ifdef MPU6050_IN_USE
void startCalibration() {
#ifdef MY_SETUP_DEBUG
  Serial.println(F("Calibrating MPU6050 ......"));
#endif
  while ( adjustType < 8 ) {
    calibrateMPU6050();
    // 1 = x accel, 2 = y accel, 3 = z accel, 4 = x gyro, 5 = y gyro, 6 = z gyro
  }
  if (! mpuOffsetSaving()) {
#ifdef MY_SETUP_DEBUG
    Serial.println(F("Failed to save mpuConfig"));
#endif
  }
#ifdef MY_SETUP_DEBUG
  else {
    Serial.println(F("mpuConfig saved"));
  }
#endif
  Serial.println(F("The system will restart in one second ( otherwise please restart manually )"));
  delay(1000);

  resetFunc(); //call reset
  // will never exit from here , will restart
}
#endif


/*************************************************************** mpuSetup()  **********************************************************************/

#ifdef INTERRUPT_PIN_MPU6050
void SetInterruptMPU6050() {
  // enable Arduino interrupt detection of MPU6050
#ifdef MY_SETUP_DEBUG
  Serial.println(F("Enabling interrupt detection of HCM5883L (Arduino external interrupt )..."));
#endif
  attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN_MPU6050), dmpDataReady, RISING);
  mpuIntStatus = mpu.getIntStatus();
  // set our DMP Ready flag so the main loop() function knows it's okay to use it
#ifdef MY_SETUP_DEBUG
  Serial.println(F("DMP ready! Waiting for first interrupt..."));
#endif
}
#endif

/**********************************  mpuOffsetSetting ***************************************/

#ifdef MPU6050_IN_USE
void mpuOffsetSetting() {
#ifdef MY_SETUP_DEBUG
  Serial.println(F("Setting calibration values......"));
#endif
  mpu.setXGyroOffset(xGyroOff);
  mpu.setYGyroOffset(yGyroOff);
  mpu.setZGyroOffset(zGyroOff);
  mpu.setXAccelOffset(xAccelOff);
  mpu.setYAccelOffset(yAccelOff);
  mpu.setZAccelOffset(zAccelOff);
}
#endif

/**********************************  myMPU ***************************************/
#ifdef MPU6050_IN_USE
void myMPU() {
  // reset interrupt flag and get INT_STATUS byte
  mpuInterrupt = false;
  mpuIntStatus = mpu.getIntStatus();

  // get current FIFO count
  fifoCount = mpu.getFIFOCount();

  // check for overflow (this should never happen unless our code is too inefficient)
  if ((mpuIntStatus & 0x10) || fifoCount == 1024) {
    // reset so we can continue cleanly
    mpu.resetFIFO();
    Serial.println(F("FIFO overflow!"));

    // otherwise, check for DMP data ready interrupt (this should happen frequently)
  } else if (mpuIntStatus & 0x02) {
    // wait for correct available data length, should be a VERY short wait
    while (fifoCount < packetSize) fifoCount = mpu.getFIFOCount();

    // read a packet from FIFO
    mpu.getFIFOBytes(fifoBuffer, packetSize);

    // track FIFO count here in case there is > 1 packet available
    // (this lets us immediately read more without waiting for an interrupt)
    fifoCount -= packetSize;

#ifdef OUTPUT_READABLE_QUATERNION
    // display quaternion values in easy matrix form: w x y z
    mpu.dmpGetQuaternion(&q, fifoBuffer);
    Serial.print("quat\t");
    Serial.print(q.w);
    Serial.print("\t");
    Serial.print(q.x);
    Serial.print("\t");
    Serial.print(q.y);
    Serial.print("\t");
    Serial.print(q.z);
#endif
#ifdef OUTPUT_READABLE_EULER
    // display Euler angles in degrees
    mpu.dmpGetQuaternion(&q, fifoBuffer);
    mpu.dmpGetEuler(euler, &q);
    Serial.print("euler\t");
    Serial.print(euler[0] * 180 / M_PI);
    Serial.print("\t");
    Serial.print(euler[1] * 180 / M_PI);
    Serial.print("\t");
    Serial.print(euler[2] * 180 / M_PI);
#endif
#ifdef OUTPUT_READABLE_YAWPITCHROLL
    // display Euler angles in degrees
    mpu.dmpGetQuaternion(&q, fifoBuffer);
    mpu.dmpGetGravity(&gravity, &q);
    mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
    Serial.print("ypr\t");
    Serial.print(ypr[0] * 180 / M_PI);
    Serial.print("\t");
    Serial.print(ypr[1] * 180 / M_PI);
    Serial.print("\t");
    Serial.print(ypr[2] * 180 / M_PI);
    Serial.print("\t");
#endif
#ifdef OUTPUT_MPU_TEMPERATURE
    Serial.print("\t Temp.");
    intTemp = ~mpu.getTemperature();
    myTemp = intTemp + 1;
    myTemp = myTemp / 340 + 36.53;
    Serial.print(myTemp);
#endif
#ifdef OUTPUT_READABLE_REALACCEL
    // display real acceleration, adjusted to remove gravity
    mpu.dmpGetQuaternion(&q, fifoBuffer);
    mpu.dmpGetAccel(&aa, fifoBuffer);
    mpu.dmpGetGravity(&gravity, &q);
    mpu.dmpGetLinearAccel(&aaReal, &aa, &gravity);
    Serial.print("areal\t");
    Serial.print(aaReal.x);
    Serial.print("\t");
    Serial.print(aaReal.y);
    Serial.print("\t");
    Serial.print(aaReal.z);
    Serial.print("\t");
#endif
#ifdef OUTPUT_READABLE_WORLDACCEL
    // display initial world-frame acceleration, adjusted to remove gravity
    // and rotated based on known orientation from quaternion
    mpu.dmpGetQuaternion(&q, fifoBuffer);
    mpu.dmpGetAccel(&aa, fifoBuffer);
    mpu.dmpGetGravity(&gravity, &q);
    mpu.dmpGetLinearAccel(&aaReal, &aa, &gravity);
    mpu.dmpGetLinearAccelInWorld(&aaWorld, &aaReal, &q);
    Serial.print("aworld\t");
    Serial.print(aaWorld.x);
    Serial.print("\t");
    Serial.print(aaWorld.y);
    Serial.print("\t");
    Serial.print(aaWorld.z);
    Serial.print("\t");
#endif
#ifdef OUTPUT_TEAPOT
    // display quaternion values in InvenSense Teapot demo format:
    teapotPacket[2] = fifoBuffer[0];
    teapotPacket[3] = fifoBuffer[1];
    teapotPacket[4] = fifoBuffer[4];
    teapotPacket[5] = fifoBuffer[5];
    teapotPacket[6] = fifoBuffer[8];
    teapotPacket[7] = fifoBuffer[9];
    teapotPacket[8] = fifoBuffer[12];
    teapotPacket[9] = fifoBuffer[13];
    Serial.write(teapotPacket, 14);
    teapotPacket[11]++; // packetCount, loops at 0xFF on purpose
#endif
#ifndef OUTPUT_TEAPOT
    Serial.println("");
#endif
  }
#ifdef SHOW_EMPTY
  else {
    Serial.println("********  EMPTY  ********");
  }
#endif
}
#endif


// ================================================================
// ===         MPU/HCL INTERRUPT DETECTION ROUTINE             ===
// ================================================================

/**********  NOT TESTED *************/



#ifdef INTERRUPT_PIN_MPU6050
void dmpDataReady() {
  mpuInterrupt = true;
}
#endif

#ifdef INTERRUPT_PIN_HCM5883L
void cmpDataReady() {
  cmpInterrupt = true;
}
#endif













