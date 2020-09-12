


#include "MPU9250.h"

#include "HyperDisplay_UG2856KLBAG01.h"
#include <math.h>



#define AHRS false         // Set to false for basic data read
#define SerialDebug true  // Set to true to get Serial output for debugging



#define I2Cclock 400000
#define I2Cport Wire


UG2856KLBAG01_I2C myTOLED;
UG2856KLBAG01_I2C myTOLED2;
float ax; 
float ay; 
float az; 

float gx; 
float gy; 
float gz; 
MPU9250 myIMU(I2Cport, 0x68);
int xval[5];
int yval[5];
int xval2[5];
int yval2[5];
void setup()
{
  Wire.begin();
  Wire.setClock(I2Cclock);
  // TWBR = 12;  // 400 kbit/sec I2C speed
  Serial.begin(115200);

  while(!Serial){};


  
  

  
  
  int status = myIMU.begin();
  Serial.println(status);
  if (status > 0) {
    Serial.println(F("MPU9250 is online..."));
    myTOLED.begin(I2Cport, false, SSD1309_ARD_UNUSED_PIN);
    myTOLED.setScroll(false);
    myTOLED.setVScrollArea(8, 56);
    myTOLED.setContrastControl(0);
    myTOLED2.begin(I2Cport, true, 0x00);
    myTOLED2.setScroll(false);
    myTOLED2.setVScrollArea(8, 56);
    myTOLED2.setContrastControl(0);
    initDisplay();
  }
   
}

void loop()
{
   
   readIMU();
   
   updateDisplay();
}
void initDisplay(){
  obscureDisplay();
  readIMU();
  float a = sqrt(pow(ax,2) + pow(ay,2) + pow((az),2));
  float g = sqrt(pow(gx,2) + pow(gy,2) + pow(gz,2));
 
  
  uniqueRandom(xval, 4);
  uniqueRandom(xval2, 4);
  for(int i = 0; i<4; i++){
    int col = xval[i]%3;
    int row = round((xval[i]-col)/3);
    xval[i] = col*21 + round(random(0, 2));
    yval[i] = row*24 + round(random(0, 2)) + 8;
    col = xval2[i]%3;
    row = round((xval2[i]-col)/3);
    xval2[i] = col*21 + round(random(0, 2))+64;
    yval2[i] = row*24 + round(random(0, 2)) + 8;

    myTOLED.rectangleClear(xval[i], yval[i], xval[i]+24, yval[i]+23, true);
    myTOLED2.rectangleClear(xval2[i], yval2[i], xval2[i]+23, yval2[i]+23, true);
  }
 
}
void readIMU(){
   myIMU.readSensor();
  // display the data
  Serial.print(myIMU.getAccelX_mss(),6);
  ax = myIMU.getAccelX_mss();
  Serial.print("\t");
  Serial.print(myIMU.getAccelY_mss(),6);
  Serial.print("\t");
  ay = myIMU.getAccelY_mss();
  Serial.print(myIMU.getAccelZ_mss(),6);
  Serial.print("\t");
  az = myIMU.getAccelZ_mss();
  Serial.print(myIMU.getGyroX_rads(),6);
  Serial.print("\t");
  gx = myIMU.getGyroX_rads();
  Serial.print(myIMU.getGyroY_rads(),6);
  Serial.print("\t");
  gy = myIMU.getGyroY_rads();
  Serial.print(myIMU.getGyroZ_rads(),6);
  Serial.print("\t");
  gz = myIMU.getGyroZ_rads();
 
  Serial.println(myIMU.getTemperature_C(),6);
  delay(100);
  
}
void obscureDisplay(){
  Serial.print("obscuring");

  int i = 33;
  int j = 32;

  while (i<56 && j>8){
    myTOLED.lineSet(0, i, 63, i, 1);
   myTOLED2.lineSet(64, i, 127, i, 1);
    myTOLED.lineSet(0, j, 63, j, 1);
   myTOLED2.lineSet(64, j, 127, j, 1);
    i = i+3;
    j = j-3;
  }
  
   i = 34;
   j = 31;

  while (i<56 && j>8){
    myTOLED.lineSet(0, i, 63, i, 1);
   myTOLED2.lineSet(64, i, 127, i, 1);
    myTOLED.lineSet(0, j, 63, j, 1);
   myTOLED2.lineSet(64, j, 127, j, 1);
    i = i+3;
    j = j-3;
  }
   i = 35;
   j = 30;

  while (i<56 && j>8){
    myTOLED.lineSet(0, i, 63, i, 1);
   myTOLED2.lineSet(64, i, 127, i, 1);
    myTOLED.lineSet(0, j, 63, j, 1);
   myTOLED2.lineSet(64, j, 127, j, 1);
    i = i+3;
    j = j-3;
  }
  //myTOLED.rectangleSet(0, 8, 63, 56, true);
  //myTOLED2.rectangleSet(64, 8, 127, 56, true);
  delay(1000);
  //yield;
}
void updateDisplay(){
  float a = sqrt(pow(ax,2) + pow(ay,2) + pow((az),2));
  float g = sqrt(pow(gx,2) + pow(gy,2) + pow(gz,2));
  Serial.print("a"); Serial.print(a);
  Serial.print("g"); Serial.print(g);
  
  if(a>11 || g>1.75){
    initDisplay();
  }
  else if ((g<1.75 && g>.5)){
    float transX = gy-gz;
    float transY = -gx;
//    float mag = sqrt(pow(dirX,2) + pow(dirY,2));
//    int transX = round(dirX*15/mag);
//    int transY = round(dirY*15/mag);
//    int count = 0;
  

  if (abs(transX)>=abs(transY)){
    if (transX>0){
      
      myTOLED.contHScrollSetupRight(0x00, 0x07, 0x07, 0x00, 0x3F);
      myTOLED.setScroll(true);
      myTOLED2.contHScrollSetupRight(0x00, 0x07, 0x07, 0x3F, 0x7F);
      myTOLED2.setScroll(true);
      delay(500);
    }
    else if(transX<0){
      
      myTOLED.contHScrollSetupLeft(0x00, 0x07, 0x07, 0x00, 0x3F);
      myTOLED.setScroll(true);
      myTOLED2.contHScrollSetupLeft(0x00, 0x07, 0x07, 0x3F, 0x7F);
      myTOLED2.setScroll(true);
      delay(500);
    }
  }
  else if (abs(transX)<abs(transY)){
    
    myTOLED.contVHScrollSetupLeft(false, 0x00, 0x07, 0x00, 0x00, 0x7F);
    myTOLED.setScroll(true);
    myTOLED2.contVHScrollSetupLeft(false, 0x00, 0x07, 0x00, 0x00, 0x7F);
    myTOLED2.setScroll(true);
    delay(1100);
    
  }


  myTOLED.setScroll(false);
  
  myTOLED2.setScroll(false);
  } 

  
  
}
void uniqueRandom(int* arr, int length){
  randomSeed(millis());
  for(int i = 0; i<length; i++){
    int val = round(random(0, 8));
    arr[i] = val;
    for(int j = 0; j<i; j++){
      if(arr[j] == val){
        i = i-1;
      }
    }
  } 
}
