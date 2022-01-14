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
int maxWaterLevel = 0;
int waterVal = 0;
int numberValuesRead = 0;
bool boolWait3Seconds = false;
bool checkPlacedInSoil = true;

int highestAverageMoisture[60];

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
    // if placed in soil start initial setup

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
    }

    // start reading initial values



    

        
    //initialWaterReading = false;


   
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
