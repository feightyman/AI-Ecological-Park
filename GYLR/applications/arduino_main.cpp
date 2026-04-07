#include <Arduino.h>
#include "HardWare.h"

void setup(void)
{
    Serial.begin();
    SHT31_Start();
    BMP280_Start();
    SSD1306_Start();
    Key_Start();
}

void loop(void)
{
    SHT31_Run();
    BMP280_Run();
    SSD1306_Run();
    RGB_Run();
    Key_Run();
}

