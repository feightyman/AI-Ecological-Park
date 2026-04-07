#include <Arduino.h>
#include "HardWare.h"
#include <RTduino.h>

int Key1=D9;
int Key2=D10;
int Key_Num=0;
int Key_Status=0;

void Key_Start()
{
    pinMode(Key2, INPUT_PULLUP);
}

void Key_Run()
{
    Key_Num=0;
    if(digitalRead(Key2)==LOW)
    {
        Key_Num=2;
    }

    switch(Key_Num)
    {
    case 0:Key_Status=0;
    break;
    case 1: ;
    break;
    case 2:if(Key_Status==0){Key_Status=1;coloer++;coloer%=8;}
    break;
    }

}

