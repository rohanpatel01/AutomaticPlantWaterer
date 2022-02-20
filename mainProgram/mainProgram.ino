/*
 * Issue log:
 * CheckMoisture is not logging the right values from water sensor
 * this also causes system to crash. See what is going on here.
 * 
 * 
 * Solution log:
 * I changed the print statement to come before the array++
 * This way I didn't log a value that didn't exist b/c we're
 * trying to access something ahead of what we have
 * 
 * ^ So far initialScan and checkMoisture work perfectly, 
 * now need to test wait feature
 * 
 * ***** NEED TO FIX SOMETHING WITH WAIT FEATURE
 * Wait feature works fine until 5+ iterations then it spams
 * 
 * 
 * Moved start = currentTime in wait method to inside the if statement
 * from outside the if statement. I think this will fix the spamming issue 
 * cuz that is a timing issue
 * 
 * 
 * 
 */

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

// LEDs for testing spammingIssue
int leftLED = 11;
int middleLED = 12;
int rightLED = 13;

void setup() 
{
    pinMode(leftLED, OUTPUT);
    pinMode(middleLED, OUTPUT);
    pinMode(rightLED, OUTPUT);

    Serial.begin(9600);
    lcd.begin(16, 2);
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("Hello There!");
    stepper.setSpeed(motorSpeed);
    Serial.println("--------------------------------------------");
    Serial.println("Starting System");
   
} // end of setup

void loop()
{
    waterVal = analogRead(waterInputPin);
   
    initialSetup();
    
    checkMoisture();
    
    waitBeforeNextScan();
    
    //motorControl();
}

void checkMoisture()
{
   if(millis() - start >= eventInterval && timePassed < totalScanLength && shouldScan)
   {
      digitalWrite(leftLED, LOW);
      
      // turn on current LED
      digitalWrite(middleLED, HIGH);

      digitalWrite(rightLED, LOW);
      
      if(timePassed < totalScanLength - 1)
      {
         Serial.println("Check Moisture values");
         Serial.println("Should Scan");
         Serial.println(shouldScan);

         Serial.println("wait");
         Serial.println(wait);
        
         timePassed++;
         
         highestAverageMoisture[arrayIndex] = waterVal;
         Serial.println(highestAverageMoisture[arrayIndex]);
         arrayIndex++;
         
         start = millis();
      }else
      {
        // get last value
        //timePassed++;
        
        highestAverageMoisture[arrayIndex] = waterVal;
        Serial.println(highestAverageMoisture[arrayIndex]);

        // this might be the issue
        //arrayIndex++;

        // find average
        Serial.println("Find average");
            for(int x = 0; x < arrayLength; x++)
                averageSum += highestAverageMoisture[x];

        Serial.print("Average : ");
        Serial.println(averageSum / arrayLength);

        // determine if water is needed
        if ((averageSum / arrayLength) <= halfSaturation)
        {
          Serial.println("Needs water");
          
        }else
        {
          Serial.println("Still moist");
        }
        
        clearArray();
        
        // settings for next method
        timePassed = 0;
        shouldScan = false;
        wait = true;

        start = millis();
        
      }
   }
}

void waitBeforeNextScan()
{
   if(millis() - start >= eventInterval && timePassed < timeTillNextScan && wait)
   {
      // turn off previous LED
      digitalWrite(middleLED, LOW);
    
      // turn on current LED
      digitalWrite(rightLED, HIGH);

      // routine before last value
      if(timePassed < timeTillNextScan - 1)
      {
         Serial.println("waitBeforeNextScan values");
         Serial.println("Should Scan");
         Serial.println(shouldScan);

         Serial.println("wait");
         Serial.println(wait);
        
        timePassed++;
        Serial.println(timePassed);
        start = millis();
        
      }else   // the last value and when scan is over
      {
        // last value
        timePassed++;
        Serial.println(timePassed);
        
        // settings for next method
        shouldScan = true;
        wait = false;
        timePassed = 0;  
        Serial.println("Wait Complete");
        start = millis();
      }
   }
}

void createLevelsOfMoisture(int average)
{
    fullSaturation = average;
    threeFourthSaturation = average * 0.75;
    halfSaturation = average / 2;
    dryBoi = average / 5;    
 
    timePassed = 0; 
    shouldScan = true;
    start = millis();
    
}

int initialSetup()
{
  
  if(digitalRead(buttonInputPin) == HIGH && initialSetupStep == 0)
  {
    digitalWrite(leftLED, HIGH);

    
    while(digitalRead(buttonInputPin) == HIGH) {}
    Serial.println("Placed in soil");
    initialSetupStep++;
    start = millis();
    
  }
  
  //long currentTime = millis();
  // every second, for totalScanLength : get moisture of water
  if(millis() - start >= eventInterval && timePassed < totalScanLength && initialSetupStep == 1)
  {
     digitalWrite(leftLED, HIGH);

     
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

   digitalWrite(leftLED, HIGH);


    
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
    arrayIndex = 0;
    
    Serial.println("array cleared");
     
}


void motorControl()
{
  // if (waterNeeded) : motor move to allow water and turn off after x seconds
  
  Serial.println("clockwise full");
  stepper.step(stepsPerRev);
  delay(500);
  
}
