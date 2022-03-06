#include <Array.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd (0x27, 16, 2);

#define waterInputPin  A0
#define  buttonInputPin  9
// LEDs for testing spammingIssue
#define  leftLED  11
#define  middleLED  12
#define  rightLED  13

#define totalScanLength  3
const int eventInterval = 1000;

int fullSaturation = 0;
int threeFourthSaturation = 0;
int halfSaturation = 0;
int dryBoi = 0;



int start;

//int highestAverageMoisture[totalScanLength];
//int arrayLength = sizeof(highestAverageMoisture) / sizeof(highestAverageMoisture[0]);

int timeTillNextScan = 5; 
int initialSetupStep = 0;
int arrayIndex = 0;
int timePassed = 0;

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

    // when button is pressed we start program
    if(digitalRead(buttonInputPin) == HIGH)
        while(digitalRead(buttonInputPin) == HIGH) {}
    
    digitalWrite(leftLED, HIGH);

    int average = 0;
    int sum;
    int waterVal = 0;

    int highestAverageMoisture = 0;

    // first initial scan is below

    for(byte x = 0; x < totalScanLength; x++)
    {
         waterVal = analogRead(waterInputPin);
         Serial.println(waterVal);
         sum += waterVal;
         delay(eventInterval);
    }
    
    averageSum = sum / totalScanLength;
    Serial.print("highest Moisture : ");  Serial.println(averageSum);

    fullSaturation = average;
    threeFourthSaturation = average * 0.75;
    halfSaturation = average / 2;
    dryBoi = average / 5;    
    
    digitalWrite(leftLED, LOW);
} // end of setup

void loop()
{
    checkMoisture();
    waitBeforeNextScan();
}

byte checkMoisture()
{
   digitalWrite(leftLED, LOW);
   digitalWrite(middleLED, HIGH);
   digitalWrite(rightLED, LOW);

   /* don't need to find average because value we get from sensor will be representative
      of average because water level is continuous
   */
   
   if(analogRead(waterInputPin) <= halfSaturation) Serial.println("Needs water");
   else Serial.println("Still moist");
   digitalWrite(middleLED, LOW);
}

byte waitBeforeNextScan()
{
   digitalWrite(middleLED, LOW);
   digitalWrite(rightLED, HIGH);

   uint32_t startTime = millis();
   bool wait = true;

   while(wait)
   {
      uint32_t eventLength = millis - startTime;
      // after 1 second has expired
      if(eventLength % 1000 == 0)
      {
          Serial.print("Time Passed");  Serial.println((eventLength) / 1000 );
      }

      if(eventLength == eventInterval) wait = false;
   }
   
   Serial.println("Wait Complete");
   digitalWrite(rightLED, LOW);


}
