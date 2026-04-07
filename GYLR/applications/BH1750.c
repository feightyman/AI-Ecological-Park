#include "sensor.h"

/* 定义一个BH1750线程句柄结构体指针 */
static rt_thread_t app_bh1750_thread = RT_NULL;

float Light;

static void app_bh1750_thread_entry(void *parameter)
{
    rt_device_t dev = RT_NULL;
    struct rt_sensor_data data;
    rt_size_t res;

    /* 查找系统中的传感器设备 */
    dev = rt_device_find("li_bh1750");
    if (dev == RT_NULL)
    {
        rt_kprintf("Can't find device:li_bh1750\n");
        return;
    }

    /* 以轮询模式打开传感器设备 */
    if (rt_device_open(dev, RT_DEVICE_FLAG_RDONLY) != RT_EOK)
    {
        rt_kprintf("open device failed!");
        return;
    }

    while(1)
    {
        /* 从传感器读取一个数据 */
        res = rt_device_read(dev, 0, &data, 1);
        if (res != 1)
        {
            rt_kprintf("read data failed!size is %d", res);
        }
        else
        {
            //rt_kprintf("light:%4d.%d lux \n", (&data)->data.light / 10, (&data)->data.light % 10);
            Light=(&data)->data.light/10.0;
            rt_kprintf("light:%4.1f lux\n",Light);
        }
            rt_thread_delay(1000);
    }
    /* 关闭传感器设备 */
    rt_device_close(dev);
}

int app_bh1750_init(void)
{
    rt_err_t rt_err;

    /* 创建BH1750线程*/
    app_bh1750_thread = rt_thread_create("app_bh1750 thread",
            app_bh1750_thread_entry, RT_NULL, 1024, 20, 10);
    /* 如果获得线程控制块，启动这个线程 */
    if (app_bh1750_thread != RT_NULL)
        rt_err = rt_thread_startup(app_bh1750_thread);
    else
        rt_kprintf("app_bh1750_thread create failure !!! \n");

    /* 判断线程是否启动成功 */
    if( rt_err == RT_EOK)
        rt_kprintf("app_bh1750_thread startup ok. \n");
    else
        rt_kprintf("app_bh1750_thread startup err. \n");

    return rt_err;
}


