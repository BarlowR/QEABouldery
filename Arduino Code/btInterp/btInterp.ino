#include <SoftwareSerial.h>
#include <Wire.h>

SoftwareSerial blueTooth(6, 7); // RX, TX

String serialCommand = "";
byte forw = 0;
byte side = 0;


void setup() {
  // put your setup code here, to run once:
  Wire.begin(8);
  Wire.onRequest(requestEvent);
  Serial.begin(9600);
  blueTooth.begin(9600);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:

  blueToothRead();

  /*
  Serial.print(forw);
  Serial.print(" ");
  Serial.println(side); 
  */
}


void requestEvent() {
  Wire.write(forw +40); // respond with message of 6 bytes
  Wire.write(side + 40);
  Wire.write('!');
  // as expected by master
}











void blueToothRead(){
  if (blueTooth.available() > 0) {
    //Serial.println("SerialAvail");
    // read the incoming byte:
    char serVal = (char)blueTooth.read();

    serialCommand += serVal;
    //Serial.println(serVal);

    if (serialCommand.indexOf(']')>0){
      //Serial.println(serialCommand);
      forw = ((byte)serialCommand.charAt(2));
      side = ((byte)serialCommand.charAt(1));
  
      
      serialCommand = "";

      
    }
    else if (serialCommand.length() > 10){
      Serial.println(serialCommand);

      serialCommand = "";
    
    }
  }
}

