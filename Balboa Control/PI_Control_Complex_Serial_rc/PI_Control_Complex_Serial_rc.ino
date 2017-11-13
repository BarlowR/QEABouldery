#include <Balboa32U4.h>
#include <Wire.h>
#include <LSM6.h>
#include <Wire.h>

LSM6 imu;


Balboa32U4Motors motors;
Balboa32U4ButtonA buttonA;
Balboa32U4Encoders encoders;


int setVel = 0;
int rotVel = 0;

int forw;
int side;

int32_t initang = 90000;
int32_t angle=initang; // units: millidegrees
int32_t angleRate; // units: degrees/s (or millidegrees/ms)
int32_t setAng = 0;
long lastenc = 0;

int CALIBRATION_ITERATIONS = 100;

bool pre = false;

float vel;

int it = 0;
int I2CIteration = 0;
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

float k_p = -25/1000.0;
float k_i = -235/1000.0;
float k_d = -120/1000.0;

/* 30
 * 235
 * 125
 */


 float k_pv = 1.0; //1.0, 1.0
float k_iv = 0.1 ; //.1, 0.5
float k_dv = 60; //50, 30

const int UPDATE_TIME_MS = 10;

String serialCommand = "";




void setup(){
  Serial.begin(9600);

  
  // Initialize IMU.
  setupIMU();
  comSetup();
  

  Serial.println("Waiting for Button Press");

}

void loop(){
  
  
  readI2C();
  imu.read();
  updateTimeWait(5);
  integrateGyro();
  updateTimeWait(5);
  encoderCalc();
  balanceRobot();
  setDesVel();
  
  Serial.print(setAng);
  Serial.print(" ");
  Serial.print(angle);
  Serial.print(" ");
  Serial.print(vel);
  Serial.print(" ");
  Serial.println((encoders.getCountsRight()+encoders.getCountsLeft())/2);
  
}



