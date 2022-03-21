#include <Array.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd (0x27, 16, 2);

#define waterInputPin  A0
#define motorPin 4
#define  buttonInputPin  9
#define totalScanLength  3

bool initialComplete = false;

const int totalWaitTime = 5000;
const int delayTime = 1000;
const int wateringTime = 10000; // 10 seconds

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
    //lcd.print("Hello There!");
    lcd.print("Please water your plant thoroughly then place the water sensor in the soil till the level reaches the end of the silver-colored strips ");
    
    Serial.println("--------------");
    Serial.println("Starting System");
    int value = 0;
    int sum;

    // for motor
    pinMode(motorPin, OUTPUT);

    
    
    // initialization code
    while(digitalRead(buttonInputPin) != HIGH || digitalRead(buttonInputPin) == HIGH ){}
    Serial.println("Placed in soil");

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
      
} // end of setup

void loop()
{
      //scrollText();
      
      checkMoisture();
      waitBeforeNextScan();
}

byte motor()
{
   digitalWrite(motorPin, HIGH);
   delay(wateringTime);
   digitalWrite(motorPin, LOW);
}


byte checkMoisture()
{
   if(analogRead(waterInputPin) <= halfSaturation)
   {
      Serial.println("Needs water");
      motor();
   }
   else
   {
      Serial.println("Still moist");
   }
}

byte waitBeforeNextScan()
{
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
  
}

byte scrollText()
{
   for(int PositionCount=0; PositionCount<16; PositionCount++)
   {
        lcd.autoscroll(); //builtin command to scroll right the text
        delay(1000);//delay of 150 msec
        Serial.println("Testing Scrolling Text");
   }
}
