#include <Array.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Stepper.h>
#include <Vector.h>



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
int maxWaterLevel = 0;
int waterVal = 0;
int numberValuesRead = 0;


//vector<int> highestAverageMoistureLevel;



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
    findHighestAverageWaterLevel();
    readWaterVal();

    //motorControl();


    
      
} // end of loop



// find highest water value

int findHighestAverageWaterLevel()
{
  // if a water level is detected and it's the first time program is being started
  // start process to wait 3 seconds and then read values for highest average
  if(waterVal > 50  && initialWaterReading)
  {
     boolInitialWaitOnUser = true;
     // starting the time. should only call once
     start = millis();
     Serial.println("Placed in soil");
     initialWaterReading = false;
  }

  
  // after water value is read 
  // and water sensor is placed in soil
  // wait 3 seconds to give time to get accurate average highest
  if(boolInitialWaitOnUser)
  {
     //wait 3 seconds till reading average
     if(millis() - start >= waitBeforeReadTime)
     {
        Serial.println("3 seconds have passed - starting soil reading");
        boolInitialWaitOnUser = false; 
            
     }

    // include code that will gather average from user
    // read water value every 3 seconds for 1 minute to find average highest value

    // have initial array

    // when new value is found raise new array value to that

    // copy information from old array into new array

    // continue process until 1 miniute has passed

    
    

    
  }
       
  
}


void motorControl()
{
  Serial.println("clockwise full");
  stepper.step(stepsPerRev);
  delay(500);
  
}

int readWaterVal()
{
    waterVal = analogRead(waterInputPin);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(waterVal);
    //Serial.print("Water Value");
    //Serial.println(waterVal);
    //delay(250);

    
  
}

int setMoistureLevelsFromAverage(int average)
{
  return 0;
}
