#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>
#include "drv_gpio.h"

#define ventilator_PIN6     GET_PIN(10, 1)
#define ventilator_PIN7     GET_PIN(10, 2)

rt_thread_t ventilator_thread;
extern unsigned char ventilator1_set;
extern unsigned char ventilator2_set;

static void ventilator_entry(void *parameter)
{
    rt_pin_mode(ventilator_PIN6, PIN_MODE_OUTPUT);
    rt_pin_mode(ventilator_PIN7, PIN_MODE_OUTPUT);

    while(1)
    {
        if(ventilator1_set==0){
        rt_pin_write(ventilator_PIN6, PIN_HIGH);
        }
        else{
        rt_pin_write(ventilator_PIN6, PIN_LOW);
        }

        if(ventilator2_set==0){
        rt_pin_write(ventilator_PIN7, PIN_HIGH);
        }
        else{
        rt_pin_write(ventilator_PIN7, PIN_LOW);
        }

        rt_thread_delay(1000);
    }
}

int ventilator_init(void)
{
    rt_err_t rt_err;

    ventilator_thread = rt_thread_create("ventilator thread",
            ventilator_entry, RT_NULL, 512, 20, 10);
    /* 如果获得线程控制块，启动这个线程 */
    if (ventilator_thread != RT_NULL)
        rt_err = rt_thread_startup(ventilator_thread);
    else
        rt_kprintf("ventilator create failure !!! \n");

    /* 判断线程是否启动成功 */
    if( rt_err == RT_EOK)
        rt_kprintf("ventilator startup ok. \n");
    else
        rt_kprintf("ventilator startup err. \n");

    return rt_err;
}
