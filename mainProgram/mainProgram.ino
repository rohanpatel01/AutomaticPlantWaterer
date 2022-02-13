#include <Array.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Stepper.h>

<<<<<<< HEAD
=======
/*
 * Use a photo resistor to measure when the user puts the water sensor 
 * in the soil. This way we don't have to wait 3 seconds and all that.
 * All we have to do is wait until the sensor tells us that it's dark
 * and then we can find the average like normal.
 */

//set up pins
>>>>>>> 286aa520b927477db692ade2297c966dc0f7940c
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

<<<<<<< HEAD
int timeTillNextScan = 5; 
bool wait = false;
=======
int timeTillNextScan = 5; // 5 seconds for testing. 3600 for real (1 hour)
bool wait = false;

>>>>>>> 286aa520b927477db692ade2297c966dc0f7940c
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
<<<<<<< HEAD
   waterVal = analogRead(waterInputPin);
=======
    waterVal = analogRead(waterInputPin);
>>>>>>> 286aa520b927477db692ade2297c966dc0f7940c
    
    initialSetup();
    
    // will be routine water checker
    checkMoisture();
    // after scan is complete program will wait until it scans again
<<<<<<< HEAD
    waitBeforeNextScan();
    
    
    //motorControl();
}

void checkMoisture()
{
  long currentTime = millis();
=======
//    waitBeforeNextScan();
    
    
    //motorControl();
    
} // end of loop


// during regular moisture check, this method will be called
// to see if water is necessary
void checkMoisture()
{ 
   // every 60 mins run scan that lasts 1 min
   
   long currentTime = millis();

>>>>>>> 286aa520b927477db692ade2297c966dc0f7940c
   // took out button command cuz not necessary    totalScanLength
   if(currentTime - start >= eventInterval && timePassed < totalScanLength && shouldScan)
   {
    
      if(timePassed < totalScanLength - 1)
      {
<<<<<<< HEAD
         timePassed++;
        
         highestAverageMoisture[arrayIndex] = waterVal;
         arrayIndex++;
      
         Serial.println(highestAverageMoisture[arrayIndex]);
      
=======
        timePassed++;
        Serial.println(timePassed);
>>>>>>> 286aa520b927477db692ade2297c966dc0f7940c
        start = currentTime;
      }else
      {
        // get last value
        timePassed++;
<<<<<<< HEAD

        highestAverageMoisture[arrayIndex] = waterVal;
        arrayIndex++;

        Serial.println(highestAverageMoisture[arrayIndex]);

        Serial.println("Scan is over");

        
        timePassed = 0;
        shouldScan = false;
=======
        Serial.println(timePassed);
        Serial.println("Scan is over");
        timePassed = 0;
        shouldScan = false;

>>>>>>> 286aa520b927477db692ade2297c966dc0f7940c
        // settings for next method
        timePassed = 0;
        start = millis();
        
        wait = true;

<<<<<<< HEAD
      }
   }
}


void waitBeforeNextScan()
{
   long currentTime = millis();
   if(currentTime - start >= eventInterval && wait)
   {

    // should be printing the timePassed

=======
      }  
   }
}

<<<<<<< HEAD
void waitBeforeNextScan()
{
   long currentTime = millis();

   if(currentTime - start >= eventInterval && wait)
   {

//      highestAverageMoisture[arrayIndex] = waterVal;
//      arrayIndex++;
    
>>>>>>> 286aa520b927477db692ade2297c966dc0f7940c
      timePassed++;
      Serial.println(timePassed);
      
      
      if(timePassed >= timeTillNextScan)
      {
        shouldScan = true;
        wait = false;
        
        // settings for next method
        timePassed = 0;  
        start = millis();
<<<<<<< HEAD
        Serial.println("Routine scan reoccuring");
      }
      start = currentTime;
=======


        Serial.println("Routine scan reoccuring");
      }

      start = currentTime;

>>>>>>> 286aa520b927477db692ade2297c966dc0f7940c
      
   }
}
=======
//void waitBeforeNextScan()
//{
//   long currentTime = millis();
//
//   if(currentTime - start >= eventInterval && wait)
//   {
//
////      highestAverageMoisture[arrayIndex] = waterVal;
////      arrayIndex++;
//    
//      timePassed++;
//      Serial.println(timePassed);
//      
//      
//      if(timePassed >= timeTillNextScan)
//      {
//        shouldScan = true;
//        wait = false;
//        
//        // settings for next method
//        timePassed = 0;  
//        start = millis();
//
//
//        Serial.println("Routine scan reoccuring");
//      }
//
//      start = currentTime;
//
//      
//   }
//}
>>>>>>> parent of d33ed5b (saving to go to spammingIssue)

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
<<<<<<< HEAD
    
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






=======

    
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
>>>>>>> 286aa520b927477db692ade2297c966dc0f7940c

void clearArray()
{
  
    for(int x = 0; x < arrayLength; x++)
      highestAverageMoisture[x] = 0;
      averageSum = 0;
<<<<<<< HEAD
    Serial.println("array cleared");
=======

    Serial.println("array cleared");

>>>>>>> 286aa520b927477db692ade2297c966dc0f7940c
     
}


void motorControl()
{
  // if (waterNeeded) : motor move to allow water and turn off after x seconds
  
  Serial.println("clockwise full");
  stepper.step(stepsPerRev);
  delay(500);
  
}
