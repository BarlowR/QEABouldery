void setupIMU(){
  Wire.begin();
  if (!imu.init())
  {
    while(true)
    {
      Serial.println("Failed to detect and initialize IMU!");
      delay(200);
    }
  }
  imu.enableDefault();
  imu.writeReg(LSM6::CTRL2_G, 0b01011000); // 208 Hz, 1000 deg/s
 
  // Wait for IMU readings to stabilize.
  delay(1000);
 
  // Calibrate the gyro.
  int32_t total = 0;
  for (int i = 0; i < CALIBRATION_ITERATIONS; i++)
  {
    imu.read();
    total += imu.g.y;
    delay(1);
  }

  gYZero = total / CALIBRATION_ITERATIONS;

 
}


void integrateGyro()
//call every 10ms

{
  if (abs(angle) < angleRange * 1000){
    angle = angle * 9999 / 10000.0;
  }
  // Convert from full-scale 1000 deg/s to deg/s.
  angleRate = (imu.g.y - gYZero) / 29;
 
  angle += angleRate * UPDATE_TIME_MS;
}



void updateTimeWait(int uptime){
  unsigned long timeIt = 1000.0*uptime*it;
  while(micros() < timeIt){
  }
  it++;
}

void encoderCalc(){
  vel = (encoders.getCountsAndResetLeft() + encoders.getCountsAndResetLeft())/2;
}

