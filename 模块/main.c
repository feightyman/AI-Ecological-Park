#include "HardWare.h"

rt_thread_t LED_thread;


void LED_entry(void *parameter)
{
    rt_pin_mode(LED_PIN1, PIN_MODE_OUTPUT);
    rt_pin_mode(LED_PIN0, PIN_MODE_OUTPUT);
    while(1)
    {
        rt_pin_write(LED_PIN1, PIN_LOW);
        rt_thread_delay(500);
        rt_pin_write(LED_PIN1, PIN_HIGH);
        rt_thread_delay(500);
        rt_pin_write(LED_PIN0, PIN_LOW);
        rt_thread_delay(500);
        rt_pin_write(LED_PIN0, PIN_HIGH);
        rt_thread_delay(500);
    }
}


int main(void)
{
    LED_thread=rt_thread_create("LED",LED_entry, RT_NULL, 512, 30, 20);
    rt_thread_startup(LED_thread);

}
