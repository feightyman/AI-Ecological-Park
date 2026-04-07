#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>
#include "drv_gpio.h"

#define LED_PIN5     GET_PIN(10, 5)

rt_thread_t LED_thread;
extern unsigned char LED_Set;

static void LED_entry(void *parameter)
{
    rt_pin_mode(LED_PIN5, PIN_MODE_OUTPUT);

    while(1)
    {
        if(LED_Set){
        rt_pin_write(LED_PIN5, PIN_HIGH);
        }
        else{
        rt_pin_write(LED_PIN5, PIN_LOW);
        }
        rt_thread_delay(1000);
    }
}

int LED_init(void)
{
    rt_err_t rt_err;

    LED_thread = rt_thread_create("water_pump thread",
            LED_entry, RT_NULL, 256, 20, 10);
    /* 如果获得线程控制块，启动这个线程 */
    if (LED_thread != RT_NULL)
        rt_err = rt_thread_startup(LED_thread);
    else
        rt_kprintf("LED_thread create failure !!! \n");

    /* 判断线程是否启动成功 */
    if( rt_err == RT_EOK)
        rt_kprintf("LED_thread startup ok. \n");
    else
        rt_kprintf("LED_thread startup err. \n");

    return rt_err;
}
