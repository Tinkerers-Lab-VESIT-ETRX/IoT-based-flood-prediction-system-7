#include "DHT.h"

#define DHTPIN 8
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x38,16,2);

float vol = 0.0,l_minute;
unsigned char flowsensor = 2; // Sensor Input
unsigned long currentTime;
unsigned long cloopTime;
unsigned long flow_frequency;

const int echoPin = 2; // Echo Pin of Ultrasonic Sensor
const int pingPin = 3; // Trigger Pin of Ultrasonic Sensor


void flow () // Interrupt function to increment flow
{
   flow_frequency++;
}

void setup()
{
   Serial.begin(9600);
   pinMode(flowsensor, INPUT);
   digitalWrite(flowsensor, HIGH); // Optional Internal Pull-Up
   attachInterrupt(digitalPinToInterrupt(flowsensor), flow, RISING); // Setup Interrupt

   dht.begin();
    
   lcd.begin(16,2);
   lcd.backlight();
   lcd.clear();
   currentTime = millis();
   cloopTime = currentTime;

   pinMode(pingPin, OUTPUT); // initialising pin 3 as output
   pinMode(echoPin, INPUT); // initialising pin 2 as input

   pinMode (2 , INPUT);
}

void loop ()
{
   lcd.clear();
   lcd.setCursor(0,0);
   lcd.print("Humidity = ");
   lcd.print((float)dht.readHumidity());//print the humidity
   lcd.print(" %");
   delay(250);
   lcd.setCursor(0,1);
   lcd.print("Temperature = ");
   lcd.print((float)dht.readTemperature());//print the temperature
   lcd.print(" Celsius");
   delay(250);

   Serial.print("Humidity = ");
   Serial.print((float)dht.readHumidity());//print the humidity
   Serial.println(" %");

   Serial.print("Temperature = ");
   Serial.print((float)dht.readTemperature());//print the humidity
   Serial.println(" Celsius");

   int rain = digitalRead(2);
   if(rain==1)
   {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Weather Forecast: ");
      lcd.print("Its Raining");
      delay(250);

      Serial.print("Weather Forecast: ");
      Serial.println("Its Raining");
   }
   else
   {
      lcd.clear();
      lcd.setCursor(0,1);
      lcd.print("Weather Forecast: ");
      lcd.print("No Rain");
      delay(250);

      //Serial.print("Weather Forecast: ");
      //Serial.println("No Rain");
   }
   
   long duration, inches, cm;
  
   digitalWrite(pingPin, LOW);
   delayMicroseconds(2);
  
   digitalWrite(pingPin, HIGH);
   delayMicroseconds(10);
  
   digitalWrite(pingPin, LOW);
  
   duration = pulseIn(echoPin, HIGH); // using pulsin function to determine total time
   inches = microsecondsToInches(duration); // calling method
   cm = microsecondsToCentimeters(duration); // calling method
   lcd.clear();
   lcd.setCursor(0,0);
   lcd.print("Water level: ");
   lcd.print(cm);
   lcd.print(" cm");
   delay(250);

   Serial.print("Water level: ");
   Serial.print( cm );
   Serial.println(" cm");
   
   currentTime = millis();
   // Every second, calculate and print litres/hour
   if(currentTime >= (cloopTime + 1000))
   {
    cloopTime = currentTime; // Updates cloopTime
    if(flow_frequency != 0)
    {
      
       l_minute = (flow_frequency / 7.5); // (Pulse frequency x 60 min) / 7.5Q = flowrate in L/hour
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Rate: ");
      lcd.print(l_minute);
      lcd.print(" L/M");
      delay(250);
      l_minute = l_minute/60;
      lcd.setCursor(0,1);
      vol = vol +l_minute;
      lcd.print("Vol:");
      lcd.print(vol);
      lcd.print(" L");
      delay(250);

      //Serial.print("Rate: ");
      //Serial.print(l_minute);
      //Serial.println(" L/M");
      //Serial.print("Vol:");
      //Serial.print(vol);
      //Serial.println(" L");
   
      flow_frequency = 0; // Reset Counter
    }
    else {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Rate: ");
      lcd.print( flow_frequency );
      lcd.print(" L/M");
      delay(250);
      lcd.setCursor(0,1);
      lcd.print("Vol:");
      lcd.print(vol);
      lcd.print(" L");
      delay(250);

      Serial.print("Rate: ");
      Serial.print( flow_frequency );
      Serial.println(" L/M");
      Serial.print("Vol:");
      Serial.print(vol);
      Serial.println(" L");
    }
   }
   
}

long microsecondsToInches(long microseconds) // method to covert microsec to inches 
{
 return microseconds / 74 / 2;
}
long microsecondsToCentimeters(long microseconds) // method to covert microsec to centimeters
{
   return microseconds / 29 / 2;
}
