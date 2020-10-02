#include <Arduino.h>
#include "MPU9250.h"
#include "LEDMatrixLibrary.h"

MPU9250 mpu;

float xVel = 0;
float yVel = 0;
float xBig = 0;
float yBig = 0;
float xAccel = 0;
float yAccel = 0;
int x = 3;
int y = 4;

float roll = 0;
float pitch = 0;

void setup() {
  Serial.begin(115200);
  Wire.begin();
  for(int i; i<2;i++){
    pinMode(dis[i], OUTPUT);
    pinMode(disClk[i], OUTPUT);
    digitalWrite(dis[i], LOW);
    digitalWrite(disClk[i], LOW);
    DisplaySendCmd(i, autoAddr);
    DisplaySendCmd(i, brightness); //lowest brightness
  }
  DisplayClearBoth();
  mpu.setup();
  //delay(5000);

  // calibrate anytime you want to
  //mpu.calibrateAccelGyro();
  //mpu.calibrateMag();
  mpu.printCalibration();
}

void loop() {
  static uint32_t prev_ms = millis();
  if ((millis() - prev_ms) > 16){
    mpu.update();
    //mpu.print();

    roll = mpu.getRoll();
    pitch = mpu.getPitch();
    /*
    xAccel = pitch / 5;
    yAccel = roll / 5;

    xVel = xVel + xAccel;
    yVel = yVel + yAccel;
    if(xVel >= 10){
      xVel = 10;
    }else if(xVel <= -10){
      xVel = -10;
    }
    if(yVel >= 10){
      yVel = 10;
    }else if(yVel <= -10){
      yVel = -10;
    }
    xBig = xBig + xVel;
    yBig = yBig + yVel;

    if(xBig >= 70){
      xBig = 70;
    }else if(xBig <= -70){
      xBig = -70;
    }
    if(yBig >= 70){
      yBig = 70;
    }else if(yBig <= -70){
      yBig = -70;
    }

    x = xBig / 10;
    y = yBig / 10;
    */
    x = roll+7;
    y = pitch+7;
    if(x >= 15){
      x = 15;
    }else if(x <= 0){
      x = 0;
    }

    if(y >= 15){
      y = 15;
    }else if(y <= 0){
      y = 0;
    }
    

    ClearTable();
    AddXY(x,y);
    DisplayTable();

    prev_ms = millis();
  }

  delay(20);
  Serial.print("xAccel = ");
  Serial.print(xAccel);
  Serial.print(" | yAccel = ");
  Serial.print(yAccel);
  Serial.print(" | yVel = ");
  Serial.print(yVel);
  Serial.print(" | xVel = ");
  Serial.print(xVel);
  Serial.print(" | xBig = ");
  Serial.print(xBig);
  Serial.print(" | yBig = ");
  Serial.print(yBig);
  Serial.print(" ||| Roll = ");
  Serial.print(roll);
  Serial.print(", X = ");
  Serial.print(x);
  Serial.print(" | Pitch = ");
  Serial.print(pitch);
  Serial.print(", Y = ");
  Serial.println(y);

/*   for(int i=0;i<16;i++){
    for(int j=0;j<16;j++){
      AddXY(i,j);
      DisplayTable();
    }
  }
  for(int i=0;i<16;i++){
    for(int j=0;j<16;j++){
      RemoveXY(i,j);
      DisplayTable();
    }
  } */

} //end main loop