#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>
#include "drv_gpio.h"

#define IN_PIN3     GET_PIN(10, 3)
#define IN_PIN4     GET_PIN(10, 4)

rt_thread_t water_pump_thread;
extern unsigned char water_pump_set;

static void water_pump_entry(void *parameter)
{
    rt_pin_mode(IN_PIN3, PIN_MODE_OUTPUT);
    rt_pin_mode(IN_PIN4, PIN_MODE_OUTPUT);

    while(1)
    {
        if(water_pump_set){
        rt_pin_write(IN_PIN3, PIN_HIGH);
        rt_pin_write(IN_PIN4, PIN_LOW);
        }
        else{
        rt_pin_write(IN_PIN3, PIN_LOW);
        rt_pin_write(IN_PIN4, PIN_LOW);
        }
        rt_thread_delay(1000);
    }
}

int water_pump_init(void)
{
    rt_err_t rt_err;

    water_pump_thread = rt_thread_create("water_pump thread",
            water_pump_entry, RT_NULL, 256, 20, 10);
    /* 如果获得线程控制块，启动这个线程 */
    if (water_pump_thread != RT_NULL)
        rt_err = rt_thread_startup(water_pump_thread);
    else
        rt_kprintf("water_pump create failure !!! \n");

    /* 判断线程是否启动成功 */
    if( rt_err == RT_EOK)
        rt_kprintf("water_pump startup ok. \n");
    else
        rt_kprintf("water_pump startup err. \n");

    return rt_err;
}
