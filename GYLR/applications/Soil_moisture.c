#include <rtthread.h>
#include <rtdevice.h>

#define ADC1_DEV_NAME        "adc1"      /* ADC ???? */
#define ADC1_DEV_CHANNEL     2           /* ADC ?? */

static rt_thread_t adc1_thread = RT_NULL;

float Soil_moisture;

static void adc1_vol_Read_entry(void *parameter)
{
    rt_adc_device_t adc_dev;
    rt_uint32_t value, vol;
    rt_err_t ret = RT_EOK;

    /* ???? */
    adc_dev = (rt_adc_device_t)rt_device_find(ADC1_DEV_NAME);
    if (adc_dev == RT_NULL)
    {
        rt_kprintf("adc sample run failed! can't find %s device!\n", ADC1_DEV_NAME);
    }
    /* ???? */
    ret = rt_adc_enable(adc_dev, ADC1_DEV_CHANNEL);

    /* ????? */
    while(1)
    {
    value = rt_adc_read(adc_dev, ADC1_DEV_CHANNEL);
    rt_kprintf("ADC1 the value is :%d \n", value);

    /* ????????,3.3V??12????4096,??????100??2??? */
    vol = value * 500 / 4096;
    Soil_moisture=value *100.00/4096;
    //rt_kprintf("ADC1 the voltage is :%d.%02d \n", vol / 100, vol % 100);

    rt_thread_delay(1000);
    }

}

int adc1_vol_init(void)
{
    rt_err_t rt_err;

    adc1_thread = rt_thread_create("adc1_vol thread",
            adc1_vol_Read_entry, RT_NULL, 1024, 20, 10);
    /* 如果获得线程控制块，启动这个线程 */
    if (adc1_thread != RT_NULL)
        rt_err = rt_thread_startup(adc1_thread);
    else
        rt_kprintf("adc1_vol create failure !!! \n");

    /* 判断线程是否启动成功 */
    if( rt_err == RT_EOK)
        rt_kprintf("adc1_vol startup ok. \n");
    else
        rt_kprintf("adc1_vol startup err. \n");

    return rt_err;
}
