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

// array to hold values for average

// initialization and variables related to it
const int stepsPerRev = 2038;
int motorSpeed = 10;
int in1 = 8, in2 = 9, in3 = 10, in4 = 11;
Stepper stepper (stepsPerRev, in1, in2, in3, in4);
LiquidCrystal_I2C lcd (0x27, 16, 2);



// variables for program
int start;
long beginScan;
const int totalScanLength = 10; // original 60 seconds
int waterVal = 0;
int eventInterval = 1000; // initial setup will measure every second
int highestAverageMoisture[totalScanLength];
int averageSum = 0;
int arrayLength = sizeof(highestAverageMoisture) / sizeof(highestAverageMoisture[0]);

// need to change this for final test to 3600

const int routinetotalScanLength = 15;
int routineMoisture[routinetotalScanLength];
int routineArrayLength = sizeof(routineMoisture) / sizeof(int);
int routineAverageSum = 0;

int buttonPin = 5;
int buttonCommand = 0;

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
    pinMode(buttonPin, OUTPUT);
    pinMode(waterInputPin, INPUT); // The Water Sensor is an Input
    Serial.begin(9600);

    lcd.begin(16, 2);
    lcd.backlight();
    stepper.setSpeed(motorSpeed);

    // add code that will tell user to water plant fully before
    // inserting water sensor will need to add code for scrolling text.
    lcd.setCursor(0, 0);
    lcd.print("Hello and welcome");
    
    Serial.println("Starting System");
    
   
} // end of setup

void loop() 
{

    initialSetup();
    waterVal = analogRead(waterInputPin);

    // will be routine water checker
    checkMoisture();
    
    
    //motorControl();
    
} // end of loop


// during regular moisture check, this method will be called
// to see if water is necessary
void checkMoisture()
{
   // every 60 mins run scan that lasts 1 min
   
   long currentTime = millis();
   if(currentTime - start >= eventInterval && timePassed < totalScanLength && buttonCommand == 3)
   {
      // if time for scan is here: scan
      if(timePassed > totalScanLength)
      {
        Serial.println("Sequence scan in progress");
      }


      
   }


  
}


void createLevelsOfMoisture(int average)
{
    fullSaturation = average;
    threeFourthSaturation = average * 0.75;
    halfSaturation = average / 2;
    dryBoi = average / 5;
    
    // marks start of routine moisture check
    // variables to control routine check
    start = millis();
    timePassed = 0;

    // empty array

    // reset average
    
}

// have one program that runs scan so don't have to write code both in initial and 
// in routine checks
//void scan()
//{
//  
//}


int initialSetup()
{
  
  if(digitalRead(buttonPin) == HIGH && buttonCommand == 0 )
  {
    while(digitalRead(buttonPin) == HIGH){} // wait till button is released
    Serial.println("Placed in soil");
    buttonCommand++;
    start = millis();
    
  }

  long currentTime = millis();
  // every second, for totalScanLength : get moisture of water
  if(currentTime - start >= eventInterval && timePassed < totalScanLength && buttonCommand == 1)
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
        buttonCommand++;
        
     }
  }
  
  // if scan is complete. find average
  if(buttonCommand == 2)
  {
    Serial.println("Find average");
    for(int x = 0; x < arrayLength; x++)
        averageSum += highestAverageMoisture[x];
    
   
    Serial.print("Average : ");
    Serial.println(averageSum / arrayLength);

    createLevelsOfMoisture(averageSum / arrayLength);

    // buttonCommand now = 3 and initialSequence is over
    buttonCommand++;
    
  }
}// end of initialWaterReading

   

void motorControl()
{
  // if (waterNeeded) : motor move to allow water and turn off after x seconds
  
  Serial.println("clockwise full");
  stepper.step(stepsPerRev);
  delay(500);
  
}
