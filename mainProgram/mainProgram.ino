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
int start;
long beginScan;
const int totalScanLength = 10; // original 60 seconds
int waitBeforeReadTime = 3000;
int maxWaterLevel = 0;
int waterVal = 0;
int numberValuesRead = 0;
bool boolWait3Seconds = false;
bool checkPlacedInSoil = true;
int eventInterval = 1000; // initial setup will measure every second
int highestAverageMoisture[totalScanLength];
bool boolReadData = false;
bool boolFindHighestAverage = false;
int averageSum = 0;
int arrayLength = sizeof(highestAverageMoisture) / sizeof(highestAverageMoisture[0]);


int arrayIndex = 0;
int timePassed = 0;

// moisture levels
int fullSaturation = 0;
int threeFourthSaturation = 0;
int halfSaturation = 0;
int dryBoi = 0;



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
    waterVal = analogRead(waterInputPin);

    

    //motorControl();
      
} // end of loop



// find highest water value

/*
 * Later condense this by making each part of this function it's own method
 * That way you don't need to turn things on and off. You can just call the 
 * initialSetup() once in the loop or something (might not work so try it)
 */


int initialSetup()
{

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

      // see if need global variable
      

      for(int x = 0; x < arrayLength; x++)
      {
          averageSum += highestAverageMoisture[x];
      }
      
      Serial.print("Average : ");
      Serial.println(averageSum / arrayLength);

      createLevelsOfMoisture(averageSum / arrayLength);
      
      boolFindHighestAverage = false;
      initialWaterReading = false;
    }
   }// end of initialWaterReading
   
}// end of initialSetup

void createLevelsOfMoisture(int average)
{
    fullSaturation = average;
    threeFourthSaturation = average * 0.75;
    halfSaturation = average / 2;
    dryBoi = average / 5;

    // marks start of routine moisture check
    start = millis();
}

// during regular moisture check, this method will be called
// to see if water is necessary

//void checkMoisture()
//{
//  // for mat will be like this but times need to be fixed
//
//   long currentTime = millis();
//   
//   if(currentTime - start >= eventInterval && boolReadData && timePassed < totalScanLength)
//   {
//      // see if need to water
//      // waterNeeded = true;
//      start = currentTime;
//   }
//}
//




void motorControl()
{
  // if (waterNeeded) : motor move to allow water and turn off after x seconds
  
  Serial.println("clockwise full");
  stepper.step(stepsPerRev);
  delay(500);
  
}

//int readWaterVal()
//{
//    waterVal = analogRead(waterInputPin);  
//}
