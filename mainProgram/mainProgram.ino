#include <Array.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Stepper.h>

/*
 * Use a photo resistor to measure when the user puts the water sensor 
 * in the soil. This way we don't have to wait 3 seconds and all that.
 * All we have to do is wait until the sensor tells us that it's dark
 * and then we can find the average like normal.
 */

//set up pins
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
int eventInterval = 1000; // initial setup will measure every second

// will be array for both initial and routine scan
const int totalScanLength = 3; // original 60 seconds
int highestAverageMoisture[totalScanLength];
int averageSum = 0;
int arrayLength = sizeof(highestAverageMoisture) / sizeof(highestAverageMoisture[0]);

int timeTillNextScan = 5; // 5 seconds for testing. 3600 for real (1 hour)
bool wait = false;

bool shouldScan = false;

//buttonCommand
int initialSetupStep = 0;

// variables for passing water sensor reading into array
int arrayIndex = 0;
int timePassed = 0;

// saturation variables
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
    
} // end of loop


// during regular moisture check, this method will be called
// to see if water is necessary
void checkMoisture()
{ 
   // every 60 mins run scan that lasts 1 min
   // try using millis() % 5000 so it happends every 5 seconds
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
        //shouldScan = false;

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
      timePassed++;
      
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
