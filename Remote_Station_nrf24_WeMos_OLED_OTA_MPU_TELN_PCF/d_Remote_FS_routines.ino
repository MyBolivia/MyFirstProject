
/**********************************  mpuOffsetLoading ***************************************/

#ifdef MPU6050_IN_USE
bool mpuOffsetLoading() {
  File configFile = SPIFFS.open("/mpuConfig.json", "r");
  if (!configFile) {
#ifdef MY_SETUP_DEBUG
    Serial.println(F("Failed to open config file"));
#endif
    return false;
  }
  size_t size = configFile.size();
  if (size > 1024) {
#ifdef MY_SETUP_DEBUG
    Serial.println(F("Config file size is too large"));
#endif
    return false;
  }
  // Allocate a buffer to store contents of the file.
  std::unique_ptr<char[]> buf(new char[size]);
  // We don't use String here because ArduinoJson library requires the input
  // buffer to be mutable. If you don't use ArduinoJson, you may as well
  // use configFile.readString instead.
  configFile.readBytes(buf.get(), size);
  StaticJsonBuffer<600> jsonBuffer;
  JsonObject& json = jsonBuffer.parseObject(buf.get());
  if (!json.success()) {
    Serial.println(F("Failed to parse config file"));
    return false;
  }
  xGyroOff =  json["CFGxGyroOff"];
  yGyroOff =   json["CFGyGyroOff"];
  zGyroOff = json["CFGzGyroOff"] ;
  xAccelOff = json["CFGxAccelOff"];
  yAccelOff =  json["CFGyAccelOff"] ;
  zAccelOff = json["CFGzAccelOff"]  ;
#ifdef MY_SETUP_DEBUG
  Serial.println(" ");
  Serial.println(F("Loaded offsets:"));
  Serial.print(F(" x accel: ")); Serial.println(xAccelOff);
  Serial.print(F(" y accel: ")); Serial.println(yAccelOff);
  Serial.print(F(" z accel: ")); Serial.println(zAccelOff);
  Serial.print(F(" x gyro: ")); Serial.println(xGyroOff);
  Serial.print(F(" y gyro: ")); Serial.println(yGyroOff);
  Serial.print(F(" z gyro: ")); Serial.println(zGyroOff);
#endif
  return true;
}
#endif

/**********************************  mpuOffsetSaving ***************************************/

#ifdef MPU6050_IN_USE
bool mpuOffsetSaving() {
#ifdef MY_SETUP_DEBUG
  Serial.println(F("saving calibration values......"));
#endif
  StaticJsonBuffer<600> jsonBuffer;
  JsonObject& json = jsonBuffer.createObject();
  json["CFGxGyroOff"] = xGyroOff;
  json["CFGyGyroOff"] = yGyroOff;
  json["CFGzGyroOff"] = zGyroOff;
  json["CFGxAccelOff"] = xAccelOff;
  json["CFGyAccelOff"] = yAccelOff;
  json["CFGzAccelOff"] = zAccelOff;

  File configFile = SPIFFS.open("/mpuConfig.json", "w");
  if (!configFile) {
#ifdef MY_SETUP_DEBUG
    Serial.println(F("Failed to open config file for writing"));
#endif
    return false;
  }
  json.printTo(configFile);
  return true;
}
#endif



/**********************************  getInfo ***************************************/

#ifdef MPU6050_IN_USE
bool getInfo() {
  FSInfo fs_info;
  SPIFFS.info(fs_info);
  Serial.print(F(" ==> totalBytes = ")); Serial.println(fs_info.totalBytes);
  Serial.print(F(" ==> usedBytes = ")); Serial.println(fs_info.usedBytes );
  Serial.print(F(" ==> blockSize = ")); Serial.println(fs_info.blockSize);
  Serial.print(F(" ==> pageSize = ")); Serial.println(fs_info.pageSize);
  Serial.print(F(" ==> maxOpenFiles = ")); Serial.println(fs_info.maxOpenFiles);
  Serial.print(F(" ==>maxPathLength = ")); Serial.println(fs_info.maxPathLength);
}
#endif

/**********************************  list Files ***************************************/

#ifdef MPU6050_IN_USE
int listFiles () {
  int FileCount = 0;
  Dir dir = SPIFFS.openDir("/");
  while (dir.next()) {
    Serial.print(F(" ==> File name : "));
    Serial.print(dir.fileName());
    Serial.print(F(" ==> File size : "));
    File f = dir.openFile("r");
    Serial.print(f.size());
    Serial.println(F(" bytes "));
    FileCount++;
  }
  return FileCount;
}
#endif
