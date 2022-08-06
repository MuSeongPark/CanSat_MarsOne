#include <Servo.h>
#include <TinyGPSPlus.h>
#include <Wire.h>
#include <SPI.h>

#include <Adafruit_SSD1306.h>
#include <splash.h>
#include <Adafruit_GFX.h>
#include <Adafruit_GrayOLED.h>
#include <Adafruit_SPITFT.h>
#include <Adafruit_SPITFT_Macros.h>
#include <gfxfont.h>
#include <Adafruit_CCS811.h>


#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>

#include "I2Cdev.h"
#include "MPU6050.h"

#define OUTPUT_READABLE_ACCELGYRO
//#define OUTPUT_BINARY_ACCELGYRO

Adafruit_CCS811 ccs;
Adafruit_BMP280 bmp;
MPU6050 accelgyro;
TinyGPSPlus gps;


float al;  //기준기압
int16_t ax, ay, az;  //가속도
float aofx, aofy, aofz;

String dio, acc, lati, longi, alti, cond;  //String sensor data for Xbee
Servo servo1;  //motor1
Servo servo2;  //motor2


void setup() {
  
  al = 0.0;
  cond = "0";
  lati = "n";
  longi = "n";
  
  Wire.begin();
  Serial.begin(9600);
  
  Serial1.begin(9600); //For Xbee
  Serial2.begin(9600); //For GPS
  
  //Serial.println("CCS TEST");
  if (!ccs.begin()) {
    //Serial.println("FAILED to START SENSOR! CCS PLZ CHECK");
    //while (1);
  }
    
  while (!ccs.available());
  delay(5000);
  
  float temp = ccs.calculateTemperature();  //ccs
  ccs.setTempOffset(temp-25.0);

  //Serial.println("BMP280 TEST");
  if(!bmp.begin(0x76)){
   // Serial.print(F("FAILED to START SENSOR! BMP PLZ CHECK"));
    //while(1);
  }

  // initialize device mpu6050
  //Serial.println("Initializing I2C devices...");
  accelgyro.initialize();
  accelgyro.setFullScaleAccelRange(MPU6050_ACCEL_FS_16);  //set scale

  // verify connection
  //Serial.println("Testing device connections...");
  //Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");

  //gps conn
  //Serial.print(F("Testing TinyGPSPlus library v. ")); Serial.println(TinyGPSPlus::libraryVersion());


  //servo init
  servo1.attach(6);
  servo2.attach(7);
  
  servo1.write(100); //closed
  servo2.write(100);

}

void loop() {
  
  //기준 고도
  if(al==0){
    al = (bmp.readPressure()/100);
  }
  //carbon diox data
  if (ccs.available()) {
    if (!ccs.readData()) {
      dio = String(ccs.geteCO2());
      
    }
  }
  
  //accel&altitude data
  accelgyro.getAcceleration(&ax, &ay, &az);
  aofz = az/2048.0;
  acc = String(aofz);

  alti = String(bmp.readAltitude(al));

  //Door open & close
  if((bmp.readAltitude(al)>30) && (bmp.readAltitude(al)<60)){
    if(aofz>1.0 && aofz<1.3){
      //opened
      servo1.write(0);
      servo2.write(0);
      cond = "1";
    }
  }
  
  if(bmp.readAltitude(al)<15){
    //closed
    servo1.write(100);
    servo2.write(100);
  }
  
  
  // GPS data
  while (Serial2.available() > 0)
    if (gps.encode(Serial2.read()))
      displayInfo();
  
  //Using Serial 1 (Tx1, rx1) 
  Serial1.print(dio+",");
  Serial1.print(acc+",");
  Serial1.print(lati+",");
  Serial1.print(longi+",");
  Serial1.print(alti+",");
  Serial1.println(cond);
  
  delay(300);
}

void displayInfo()    //receive latitude &longitude
{ 
  if (gps.location.isValid())
  {
    lati = String(gps.location.lat(), 6);
    longi = String(gps.location.lng(), 6);
  }
  else  //unable to receive data
  {
    lati = "n";
    longi = "n";
  }
}
