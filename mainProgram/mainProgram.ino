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
const int totalWaitTime = 5000;
const int delayTime = 1000;

int fullSaturation = 0;
int threeFourthSaturation = 0;
int halfSaturation = 0;
int dryBoi = 0;

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
    Serial.println("Placed in soil");
    digitalWrite(leftLED, HIGH);
    int value = 0;
    int sum;

    for(byte x = 0; x < totalScanLength; x++)
    {
         value = analogRead(waterInputPin);
         Serial.println(value);
         sum += value;
         delay(delayTime);
    }
    int average = sum / totalScanLength;
    Serial.print("Average : ");  Serial.println(average);

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
   if(analogRead(waterInputPin) <= halfSaturation) Serial.println("Needs water");
   else Serial.println("Still moist");
   digitalWrite(middleLED, LOW);
}

byte waitBeforeNextScan()
{
    digitalWrite(rightLED, HIGH);
    uint32_t startTime = millis();
    int elapsedSecondCount = 0;
    
    uint32_t elapsedTime;

    do
    {
      elapsedTime = millis() - startTime;
      int elapsedSecond = elapsedTime / 1000;
  
      if (elapsedSecond > elapsedSecondCount)
      {
        Serial.print("Elapsed seconds: ");
        Serial.println(elapsedSecond);
        elapsedSecondCount = elapsedSecond;
      }
    } while (elapsedTime < totalWaitTime);
    
     Serial.println("Wait Complete");
     digitalWrite(rightLED, LOW);
  
}
