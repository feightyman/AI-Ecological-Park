#include <Arduino.h>

int coloer=0;           //0~7
int coloer_level=1;     //1~51
int coloer_Temp=0;

char redPin   =D5;
char greenPin =D6;
char bluePin  =D3;
extern float SHT31_T;

void RGB_Coloer_Set(int red,int green,int blue)
{
    analogWrite(redPin,red);
    analogWrite(greenPin,green);
    analogWrite(bluePin,255-blue);
}

void RGB_Run()
{
    if(coloer_Temp==coloer){return;}
    coloer_Temp=coloer;
    if(coloer)
    {
     switch(coloer)
        {
         case 1:RGB_Coloer_Set(coloer_level*5,0,0);      //红
         break;
         case 2:RGB_Coloer_Set(0,coloer_level*5,0);      //绿
         break;
         case 3:RGB_Coloer_Set(0,0,coloer_level*5);      //蓝
         break;
         case 4:RGB_Coloer_Set(coloer_level*5,coloer_level*5,0);    //黄
         break;
         case 5:RGB_Coloer_Set(0,coloer_level*5,coloer_level*5);    //青
         break;
         case 6:RGB_Coloer_Set(coloer_level*5,0,coloer_level*5);    //紫
         break;
         case 7:RGB_Coloer_Set(coloer_level*5,coloer_level*5,coloer_level*5);  //白
         break;
        }
    }
    else
    {
        RGB_Coloer_Set(0,0,0);
    }
}
