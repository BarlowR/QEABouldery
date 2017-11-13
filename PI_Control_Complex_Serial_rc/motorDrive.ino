void balanceRobot(){
  if (!pre){
        pre = buttonA.getSingleDebouncedPress();
        if (pre){
          Serial.println("Button Pressed");
        }
    }
    else if (abs(angle) < angleRange * 1000){
      pID = calculate_PID();
      driveMotor(pID + rotVel, pID-rotVel);
      
      pIDv = calculate_PIDv();
      //Serial.println(pIDv);
      setAng += pIDv;
    }
    else{
      driveMotor(0, 0);
  }
}



void driveMotor(int l, int r){

  
  if (1>300) l = 300;
  if (r>300) r = 300;
  if (1<-300) l = -300;
  if (1<-300) r = -300;

  motors.setLeftSpeed(l);
  motors.setRightSpeed(r);
 
}

