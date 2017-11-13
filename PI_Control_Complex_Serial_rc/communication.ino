void comSetup(){
  Wire.begin();
}

void requestI2C(){
  Wire.requestFrom(8, 3);    // request 3 bytes from slave device #8
}


void readI2C(){
  I2CIteration ++;
  if (I2CIteration > 20){
    requestI2C();
    I2CIteration = 0;
  }
  if(Wire.available()) { // slave may send less than requested
    forw = Wire.read(); // receive a byte as character
    side = Wire.read(); // receive a byte as character
    char endchar = Wire.read(); // receive a byte as character

    
    
    /*
    Serial.print(forw);
    Serial.print(" ");
    Serial.println(side);
    */
    
  }
}


void setDesVel(){
  setVel = (forw - 80)/1.5;
  rotVel = (side - 80)*1.5;
}

