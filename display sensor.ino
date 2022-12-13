/***************************************************
Copyright (c) 2017 Luis Llamas
(www.luisllamas.es)

Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License
 ****************************************************/
 
#include "AsyncTaskLib.h"
#include "DHTStable.h"
#include <LiquidCrystal.h>

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
DHTStable DHT;

#define DHT11_PIN 32
#define DEBUG(a) Serial.print(millis()); Serial.print(": "); Serial.println(a);
#define LED_RED 14
#define LED_GREEN 15
#define LED_BLUE 16

AsyncTask asyncTask1(500);
AsyncTask asyncTask2(1000);

int temperature = 0;
int humidity = 0;

void action1()
{
  DEBUG("Expired 2");
	asyncTask1.Start();
  temperature = DHT.getTemperature();
  lcd.setCursor(0, 0);
  lcd.print("Temperatura :" + temperature);
	
}

void action2()
{
	DEBUG("Expired 1");
	asyncTask2.Start();
  humidity = DHT.getHumidity();
  lcd.setCursor(0, 1);
  lcd.print("Humedad : " + humidity);
}

void setup()
{
  lcd.begin(16, 2);
	Serial.begin(115200);
	Serial.println("Starting");
  delay(5000);
	asyncTask1.OnFinish = action1;
	asyncTask2.OnFinish = action2;
	asyncTask1.Start();
  asyncTask2.Start();
  lcd.clear();
}

void loop()
{
	asyncTask1.Update();
	asyncTask2.Update();

  if (temperature > 28) {
    digitalWrite(LED_RED, HIGH);
    digitalWrite(LED_GREEN, LOW);
    digitalWrite(LED_BLUE, LOW);
  }
  else if (temperature < 25) {
    digitalWrite(LED_BLUE, HIGH);
    digitalWrite(LED_RED, LOW);
    digitalWrite(LED_GREEN, LOW);
  }
  else {
    digitalWrite(LED_GREEN, HIGH);
    digitalWrite(LED_RED, LOW);
    digitalWrite(LED_BLUE, LOW);
  }
}
