#include <Array.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Stepper.h>

int waterInputPin = A0;
int buttonInputPin = 9;
// initialization and variables related to it


const int stepsPerRev = 2038;
int motorSpeed = 10;
int in1 = 8, in2 = 9, in3 = 10, in4 = 11;
Stepper stepper (stepsPerRev, in1, in2, in3, in4);
LiquidCrystal_I2C lcd (0x27, 16, 2);
// variables for program
int start;
int waterVal = 0;
int eventInterval = 1000;

const int totalScanLength = 3;
int highestAverageMoisture[totalScanLength];
int averageSum = 0;
int arrayLength = sizeof(highestAverageMoisture) / sizeof(highestAverageMoisture[0]);

int timeTillNextScan = 5; 
bool wait = false;
bool shouldScan = false;

int initialSetupStep = 0;
int arrayIndex = 0;
int timePassed = 0;

int fullSaturation = 0;
int threeFourthSaturation = 0;
int halfSaturation = 0;
int dryBoi = 0;

void setup() 
{
    Serial.begin(9600);
    lcd.begin(16, 2);
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("Hello Aadi");
    stepper.setSpeed(motorSpeed);
    Serial.println("Starting System");
   
} // end of setup

void loop()
{
   waterVal = analogRead(waterInputPin);
    
    initialSetup();
    
    // will be routine water checker
    checkMoisture();
    // after scan is complete program will wait until it scans again
    waitBeforeNextScan();
    
    
    //motorControl();
}

void checkMoisture()
{
  long currentTime = millis();
   // took out button command cuz not necessary    totalScanLength
   if(currentTime - start >= eventInterval && timePassed < totalScanLength && shouldScan)
   {
    
      if(timePassed < totalScanLength - 1)
      {
         timePassed++;
        
         highestAverageMoisture[arrayIndex] = waterVal;
         arrayIndex++;
      
         Serial.println(highestAverageMoisture[arrayIndex]);
      
        start = currentTime;
      }else
      {
        // get last value
        timePassed++;

        highestAverageMoisture[arrayIndex] = waterVal;
        arrayIndex++;

        Serial.println(highestAverageMoisture[arrayIndex]);

        Serial.println("Scan is over");

        
        timePassed = 0;
        shouldScan = false;
        // settings for next method
        timePassed = 0;
        start = millis();
        
        wait = true;

      }
   }
}


void waitBeforeNextScan()
{
   long currentTime = millis();
   if(currentTime - start >= eventInterval && wait)
   {

    // should be printing the timePassed

      timePassed++;
      Serial.println(timePassed);
      
      
      if(timePassed >= timeTillNextScan)
      {
        shouldScan = true;
        wait = false;
        
        // settings for next method
        timePassed = 0;  
        start = millis();
        Serial.println("Routine scan reoccuring");
      }
      start = currentTime;
      
   }
}

void createLevelsOfMoisture(int average)
{
    fullSaturation = average;
    threeFourthSaturation = average * 0.75;
    halfSaturation = average / 2;
    dryBoi = average / 5;
    // print this out to user ^
    
    // marks start of routine moisture check
    // variables to control routine check
    start = millis();
    timePassed = 0; 
    shouldScan = true;
    
}





  
int initialSetup()
{
  if(digitalRead(buttonInputPin) == HIGH && initialSetupStep == 0 )
  {
    while(digitalRead(buttonInputPin) == HIGH) {}
    Serial.println("Placed in soil");
    initialSetupStep++;
    start = millis();
    
  }
  
  long currentTime = millis();
  // every second, for totalScanLength : get moisture of water
  if(currentTime - start >= eventInterval && timePassed < totalScanLength && initialSetupStep == 1)
  {
     // this will pass data from water sensor into array
     // if time is almost up we will continue getting the values
     // when time is up we will get the final value in the else statement
     
     if(timePassed < totalScanLength - 1)
     {
        Serial.println(waterVal);
        highestAverageMoisture[arrayIndex] = waterVal;
        arrayIndex++;
     
        start = millis();  
        timePassed++; 
     } else {
        // get the last value
        Serial.println(waterVal);
        highestAverageMoisture[arrayIndex] = waterVal;
        arrayIndex++;
        start = millis();  
        timePassed++; 
        // move onto next part of code: finding average
        initialSetupStep++;


        
     }
  }

   // if scan is complete. find average
  if(initialSetupStep == 2)
  {
    Serial.println("Find average");
    for(int x = 0; x < arrayLength; x++)
        averageSum += highestAverageMoisture[x];
    
   
    Serial.print("Average : ");
    Serial.println(averageSum / arrayLength);
    createLevelsOfMoisture(averageSum / arrayLength);
    clearArray();
    
    initialSetupStep++;
    
  }
}// end of initialWaterReading







void clearArray()
{
  
    for(int x = 0; x < arrayLength; x++)
      highestAverageMoisture[x] = 0;
      averageSum = 0;
    Serial.println("array cleared");
     
}


void motorControl()
{
  // if (waterNeeded) : motor move to allow water and turn off after x seconds
  
  Serial.println("clockwise full");
  stepper.step(stepsPerRev);
  delay(500);
  
}
