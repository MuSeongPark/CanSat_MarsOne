#include <TinyGPSPlus.h>
#include <Wire.h>
#include <SPI.h>
#include <SoftwareSerial.h>

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
#define OUTPUT_BINARY_ACCELGYRO

Adafruit_CCS811 ccs;
Adafruit_BMP280 bmp;
MPU6050 accelgyro;
TinyGPSPlus gps;

SoftwareSerial ss(11, 10);


float al;  //기준기압
int16_t ax, ay, az;
float aofx, aofy, aofz;

String dio, acc, lati, longi, grat, cond;


void setup() {

  al = 0.0;
  cond = "0";
  lati = "0";
  longi = "0";
  
  Wire.begin();
  Serial.begin(9600);
  
  ss.begin(9600);
  Serial1.begin(9600); //For Xbee
  
  //Serial.println("CCS TEST");
  if (!ccs.begin()) {
    Serial.println("FAILED to START SENSOR! CCS PLZ CHECK");
    while (1);
  }
  while (!ccs.available());
  delay(5000);
  float temp = ccs.calculateTemperature();
  ccs.setTempOffset(temp-25.0);

  //Serial.println("BMP280 TEST");
  if(!bmp.begin(0x76)){
    Serial.print(F("FAILED to START SENSOR! BMP PLZ CHECK"));
    while(1);
  }

  // initialize device
  //Serial.println("Initializing I2C devices...");
  accelgyro.initialize();

  // verify connection
  //Serial.println("Testing device connections...");
  //Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");
  //delay(1000);

  //gps conn
  //Serial.print(F("Testing TinyGPSPlus library v. ")); Serial.println(TinyGPSPlus::libraryVersion());

}

void loop() {
  //gps
  
  //해면고도
  if(al==0){
    al = (bmp.readPressure()/100);
  }
  //dio dataw
  if (ccs.available()) {
    if (!ccs.readData()) {  //이 부분을 수정 -> 통신으로 보내야함.
      dio = String(ccs.geteCO2());
      
    }
  }
  //delay(500);
  
  //accel data , 고도

  accelgyro.getAcceleration(&ax, &ay, &az);
  aofz = az/16384.0;
  acc = String(aofz);

  if((bmp.readAltitude(al)>100) && (bmp.readAltitude(al)<200)){
    if(aofz>1 && aofz<2){
      cond = "1";
      //Serial.println(cond);
    }
  }
  
  grat = String(bmp.readAltitude(al));
  

  // GPS data
  // This sketch displays information every time a new sentence is correctly encoded.
  if (ss.available() > 0)
  {
    if (gps.encode(ss.read()))
      displayInfo();
  }
  
  //Serial.print(dio+",");
  //Serial.print(acc+",");
  //Serial.print(lati+",");
  //Serial.print(longi+",");
  //Serial.print(grat+",");
  //Serial.println(cond);
  //delay(500);
  
  //Using Serial 1 (Tx1, rx1)
  Serial1.print(dio+",");
  Serial1.print(acc+",");
  Serial1.print(lati+",");
  Serial1.print(longi+",");
  Serial1.print(grat+",");
  Serial1.println(cond);

  delay(500);
}

void displayInfo()
{
  //Serial.print(F("Location: ")); 
  if (gps.location.isValid())
  {
    lati = String(gps.location.lat(), 6);
    longi = String(gps.location.lng(), 6);
  }
  else
  {
    lati = "0";
    longi = "0";
  }
}
