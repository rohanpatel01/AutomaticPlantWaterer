#include <stdio.h>
#include <MemoryFree.h>;

#include <Array.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Stepper.h>

int waterInputPin = A0;
int buttonInputPin = 9;

LiquidCrystal_I2C lcd (0x27, 16, 2);

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
    Serial.println("--------------");
    Serial.println("Starting System");
   
} // end of setup

void loop()
{
    waterVal = analogRead(waterInputPin);
   
    initialSetup();
    
    checkMoisture();
    
    waitBeforeNextScan();
}

void checkMoisture()
{
   if(millis() - start >= eventInterval && timePassed < totalScanLength && shouldScan)
   {
      digitalWrite(leftLED, LOW);
      digitalWrite(middleLED, HIGH);
      digitalWrite(rightLED, LOW);

      // this will pass data from water sensor into array
      // if time is almost up we will continue getting the values
      // when time is up we will get the final value in the else statement
      if(timePassed < totalScanLength - 1)
      {
         timePassed++;
         
          highestAverageMoisture[arrayIndex] = waterVal;
          Serial.println(highestAverageMoisture[arrayIndex]);
          arrayIndex++;
          
          start = millis();
      }else
      {
        highestAverageMoisture[arrayIndex] = waterVal;
        Serial.println(highestAverageMoisture[arrayIndex]);

        // find average of moisture
        for(int x = 0; x < arrayLength; x++)
        {
            averageSum += highestAverageMoisture[x];
        }
        
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
      digitalWrite(middleLED, LOW);
      digitalWrite(rightLED, HIGH);

      // this will pass data from water sensor into array
      // if time is almost up we will continue getting the values
      // when time is up we will get the final value in the else statement
      if(timePassed < timeTillNextScan - 1)
      {
        timePassed++;
        Serial.println(timePassed);
        start = millis();
        
      }else   
      {
        
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

    // values for next phase
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
    {
         averageSum += highestAverageMoisture[x];
    }
    
    Serial.print("Average : ");
    Serial.println(averageSum / arrayLength);
    createLevelsOfMoisture(averageSum / arrayLength);
    
    clearArray();
    
    initialSetupStep++;
    
  }
}// end of initialWaterReading

void  clearArray()
{
    for(int x = 0; x < arrayLength; x++)
    {
      highestAverageMoisture[x] = 0;
    }
    
    averageSum = 0;
    arrayIndex = 0;
    
    Serial.println("array cleared");
     
}
