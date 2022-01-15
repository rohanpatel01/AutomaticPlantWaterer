#include <Array.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Stepper.h>



//set up pins
int waterInputPin = A0;

// array to hold values for average

// initialization and variables related to it
const int stepsPerRev = 2038;
int motorSpeed = 10;
int in1 = 8, in2 = 9, in3 = 10, in4 = 11;
Stepper stepper (stepsPerRev, in1, in2, in3, in4);
LiquidCrystal_I2C lcd (0x27, 16, 2);



// variables for program
bool initialWaterReading = true;
bool boolInitialWaitOnUser = false;
int waitBeforeReadTime = 3000;
int start;
long beginScan;
int maxWaterLevel = 0;
int waterVal = 0;
int numberValuesRead = 0;
bool boolWait3Seconds = false;
bool checkPlacedInSoil = true;
int eventInterval = 1000; // initial setup will measure every second
const int totalScanLength = 5; // original 60 seconds
int highestAverageMoisture[totalScanLength];
bool boolReadData = false;
bool boolFindHighestAverage = false;

int arrayIndex = 0;
int timePassed = 0;


void setup() 
{

    pinMode(waterInputPin, INPUT); // The Water Sensor is an Input
    Serial.begin(9600);
    Serial.println("Starting System");
   
    lcd.begin(16, 2);
    lcd.backlight();

    stepper.setSpeed(motorSpeed);
    
    //Serial.println("Vertex info");    

    
   
} // end of setup

void loop() 
{
  
    initialSetup();
    readWaterVal();

    //motorControl();


    
      
} // end of loop



// find highest water value

int initialSetup()
{
  
  // if a water level is detected and it's the first time program is being started
  // start process to wait 3 seconds and then read values for highest average

  if(initialWaterReading)
  {

    if(waterVal > 30 && checkPlacedInSoil)
    {
      Serial.println("Placed in soil");
      start = millis();
      checkPlacedInSoil = false;
      boolWait3Seconds =  true;
    }
    
    
    //wait 3 seconds till reading average
    if(millis() - start >= waitBeforeReadTime && boolWait3Seconds)
    {
        Serial.println("3 seconds have passed - starting soil reading");
        boolWait3Seconds = false;
        boolReadData = true;
        beginScan = millis();
    }

    // start reading initial values. Every second for 60 seconds
    // will need 2 start times then

    long currentTime = millis();

    //  && timePassed < totalScanLength
    if(currentTime - start >= eventInterval && boolReadData && timePassed < totalScanLength)
    {

      /*
         We need totalScanLength - 1 b/c we want to find average when time is over
         but we can't turn this off and turn next on b/c parameter to end scan 
         is with other parameters. So we end scan early then get the last value
         before we leave this loop
       */
       
       if(timePassed < totalScanLength - 1)
       {
          Serial.println(waterVal);
          highestAverageMoisture[arrayIndex] = waterVal;
          arrayIndex++;
       
          start = millis();  
          timePassed++; 

         // when scan is over
       } else {
          // get the last value
          Serial.println(waterVal);
          highestAverageMoisture[arrayIndex] = waterVal;
          arrayIndex++;
       
          start = millis();  
          timePassed++; 

          // end scan and find average
          boolReadData = false;
          boolFindHighestAverage = true;
       }
    }

    
    // if scan is complete. find average
    if(boolFindHighestAverage)
    {
      Serial.println("Find average");

      
//      for(int x = 0; x < sizeof(highestAverageMoisture); x++)
//      {
//        Serial.print(highestAverageMoisture[x]);
//        Serial.println(", ");
//      }
      
      boolFindHighestAverage = false;
      initialWaterReading = false;


    }
//    if(boolFindHighestAverage)
//    {
//      
//      for(int x = 0; x < sizeof(highestAverageMoisture); x ++)
//      {
//        Serial.print(highestAverageMoisture[x]);
//        Serial.println(", ");
//      }
//      
//      boolFindHighestAverage = false;
//    }

    
    

   
   }// end of initialWaterReading


    
    
}// end of method
       
  



void motorControl()
{
  Serial.println("clockwise full");
  stepper.step(stepsPerRev);
  delay(500);
  
}

int readWaterVal()
{
    waterVal = analogRead(waterInputPin);
    // can put code that reads value every hour here
  
}

int setMoistureLevelsFromAverage(int average)
{
  return 0;
}
