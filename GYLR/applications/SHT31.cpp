#include <Arduino.h>
#include <Wire.h>
#include "Adafruit_SHT31.h"

bool enableHeater = false;
uint8_t loopCnt = 0;

Adafruit_SHT31 sht31 = Adafruit_SHT31();

float SHT31_Temp;
float SHT31_Hum;
unsigned int SHT31_TIME=0;

void SHT31_Start() {

//  while (!Serial)
//   delay(10);     // will pause Zero, Leonardo, etc until serial console opens

  Serial.println("SHT31 test");
  if (! sht31.begin(0x44)) {   // Set to 0x45 for alternate i2c addr
    Serial.println("Couldn't find SHT31");
    while (1) delay(1);
  }

  Serial.print("Heater Enabled State: ");
  if (sht31.isHeaterEnabled())
    Serial.println("ENABLED");
  else
    Serial.println("DISABLED");
}


void SHT31_Run() {
    if(SHT31_TIME<10)
    {return;}SHT31_TIME=0;
    SHT31_Temp = sht31.readTemperature();
    SHT31_Hum = sht31.readHumidity();

  if (! isnan(SHT31_Temp)) {  // check if 'is not a number'
//    Serial.print("Temp *C = "); Serial.print(SHT31_Temp); Serial.print("\t\t");
  } else {
    Serial.println("Failed to read temperature");
  }

  if (! isnan(SHT31_Hum)) {  // check if 'is not a number'
//    Serial.print("Hum. % = "); Serial.println(SHT31_Hum); Serial.print("\n");
  } else {
    Serial.println("Failed to read humidity");
  }

  //delay(1000);

  // Toggle heater enabled state every 30 seconds
  // An ~3.0 degC temperature increase can be noted when heater is enabled
  if (loopCnt >= 30) {
    enableHeater = !enableHeater;
    sht31.heater(enableHeater);
    Serial.print("Heater Enabled State: ");
    if (sht31.isHeaterEnabled())
      Serial.println("ENABLED");
    else
      Serial.println("DISABLED");

    loopCnt = 0;
  }
  loopCnt++;
}
