#include <Balboa32U4.h>
#include <Wire.h>
#include <LSM6.h>

LSM6 imu;


Balboa32U4Motors motors;
Balboa32U4ButtonA buttonA;
Balboa32U4Encoders encoders;

int setVel = 0;
int rotVel = 00;


int32_t initang = 91000;
int32_t angle=initang; // units: millidegrees
int32_t angleRate; // units: degrees/s (or millidegrees/ms)
int32_t setAng = 0;
int16_t lastenc = 0;

int CALIBRATION_ITERATIONS = 100;

bool pre = false;

float vel;

int it = 0;
float P = 0;
float I = 0;
float D = 0;
int pID = 0;
int32_t prev_error = 0;


float Pv = 0;
float Iv = 0;
float Dv = 0;
int pIDv = 0;

int angleRange = 30;
int32_t prev_errorv = 0;

const int iSamp = 400;

int32_t pastData[iSamp];


float gYZero;

float k_p = -30/1000.0;
float k_i = -235/1000.0;
float k_d = -85/1000.0;

/*30
 * 235
 * 125
 */


 float k_pv = 2; //1.0, 1.0
float k_iv = 0.5; //.1, 0.5
float k_dv = 30; //50, 30

const int UPDATE_TIME_MS = 10;




void setup(){
  // Initialize IMU.
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








void loop(){
  imu.read();
  updateTimeWait(5);
  integrateGyro();
  updateTimeWait(5);
  
  if (!pre){
      pre = buttonA.getSingleDebouncedPress();
  }
  else if (abs(angle) < angleRange * 1000){
    pID = calculate_PID();
    driveMotor(pID + rotVel, pID-rotVel);
    
    pIDv = calculate_PIDv();
    setAng += pIDv;
  }
  else{
    driveMotor(0, 0);
  }

  
  
  Serial.print(setAng);
  Serial.print(" ");
  Serial.print(angle);
  //Serial.print(" ");
  //Serial.print(angle/1000.0);
  Serial.print(" ");
  Serial.print(Pv*k_pv);
  Serial.print(" ");
  Serial.print(Iv*k_iv);
  Serial.print(" ");
  Serial.print(Dv*k_dv);
  Serial.print(" ");
  Serial.print(pIDv);
  
  Serial.println(" ");
  //Serial.print(angleRate);
  //Serial.print(" ");
 // Serial.println(angle);


}






 
// Call this every 10ms (UPDATE_TIME_MS)
void integrateGyro()

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

void driveMotor(int l, int r){

  
  if (1>300) l = 300;
  if (r>300) r = 300;
  if (1<-300) l = -300;
  if (1<-300) r = -300;

  motors.setLeftSpeed(l);
  motors.setRightSpeed(r);
 
}



float calculate_PID() {
  //run every 10 ms

  int32_t error = setAng-angle;

  P =  error;

  I +=  error * 0.010;

  D = (error-prev_error);

  float PID =  k_p *P + k_i *I + k_d * D;
  prev_error = error;

  return PID;
}

float calculate_PIDv() {
  //run every 10 ms

 int32_t errorv = setVel-vel;

  Pv =  errorv;

  Iv +=  errorv * 0.010;

  Dv = (errorv-prev_errorv);

  float PIDv = k_pv *Pv + k_iv *Iv + k_dv * Dv;

  prev_errorv = errorv;

  return PIDv;
}
 
