#ifndef APPLICATIONS_HARDWARE_H_
#define APPLICATIONS_HARDWARE_H_

#include <rtthread.h>
#include <rtdevice.h>
#include "drv_gpio.h"
#include <board.h>

#include "paho_mqtt.h"
#include "onenet.h"
#include "SHT31.h"
#include "BMP280.h"
#include "SSD1306.h"
#include "RGB.h"
#include "Key.h"
#include "TIME.h"
#include "onenet.h"
#include "Soil_moisture.h"
#include "BH1750.h"
#include "ventilator.h"
#include "water_pump.h"
#include "Fill_light.h"
#include "UART.h"

#define LED_PIN0     GET_PIN(0, 0)
#define LED_PIN1     GET_PIN(0, 1)

//防止太块运行
extern unsigned int SHT31_TIME;
extern unsigned int BMP280_TIME;
extern unsigned int SSD1306_TIME;
extern unsigned int RGB_TIME;


#endif
