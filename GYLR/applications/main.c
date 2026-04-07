#include "HardWare.h"

#define LED_PIN0     GET_PIN(0, 0)
#define LED_PIN1     GET_PIN(0, 1)

extern rt_bool_t init_ok;
extern MQTTClient mq_client;

#define SAMPLE_UART_NAME       "uart5"

/* 用于接收消息的信号量 */
static struct rt_semaphore rx_sem;
static rt_device_t serial;

#define ONENET_TOPIC_DP "$sys/ntB83UXCfL/Psoc6/thing/property/post"

rt_thread_t LED_thread;
rt_timer_t  TIME_Delay;
rt_thread_t MQTT_Sent_thread;


extern float Light;
extern float Soil_moisture;
extern float BMP_Temperature;
extern float BMP_Pressure;
extern float BMP_Approx;
extern unsigned char LED_Set;
extern unsigned char water_pump_set;
extern unsigned char ventilator1_set;
extern unsigned char ventilator2_set;

//void LED_entry(void *parameter)
//{
//    rt_pin_mode(LED_PIN1, PIN_MODE_OUTPUT);
//    rt_pin_mode(LED_PIN0, PIN_MODE_OUTPUT);
//
//    while(1)
//    {
//        if(LED_Set)
//        {
//        rt_pin_write(LED_PIN1, PIN_LOW);
//        rt_thread_delay(500);
//        rt_pin_write(LED_PIN1, PIN_HIGH);
//        rt_thread_delay(500);
//        rt_pin_write(LED_PIN0, PIN_LOW);
//        rt_thread_delay(500);
//        rt_pin_write(LED_PIN0, PIN_HIGH);
//        rt_thread_delay(500);
//        }
//
//    }
//}


void TIME_entry(void *parameter)
{
    SHT31_TIME++;
    BMP280_TIME++;
    SSD1306_TIME++;
}


/* 接收数据回调函数 */
static rt_err_t uart_input(rt_device_t dev, rt_size_t size)
{
    /* 串口5接收到数据后产生中断，调用此回调函数，然后发送接收信号量 */
    rt_sem_release(&rx_sem);

    return RT_EOK;
}

static void serial_thread_entry(void *parameter)
{
    char ch;

    while (1)
    {
        /* 从串口5读取一个字节的数据，没有读取到则等待接收信号量 */
        while (rt_device_read(serial, -1, &ch, 1) != 1)
        {
            /* 阻塞等待接收信号量，等到信号量后再次读取数据 */
            rt_sem_take(&rx_sem, RT_WAITING_FOREVER);
        }
        /* 读取到的数据通过串口5错位输出 */
        ch = ch + 1;
        rt_device_write(serial, 0, &ch, 1);
    }
}

static int uart_sample(int argc, char *argv[])
{
    rt_err_t ret = RT_EOK;
    char uart_name[RT_NAME_MAX];
    char str[] = "hello RT-Thread!\r\n";

    if (argc == 2)
    {
        rt_strncpy(uart_name, argv[1], RT_NAME_MAX);
    }
    else
    {
        rt_strncpy(uart_name, SAMPLE_UART_NAME, RT_NAME_MAX);
    }

    /* 查找系统中的串口设备 */
    serial = rt_device_find(uart_name);
    if (!serial)
    {
        rt_kprintf("find %s failed!\n", uart_name);
        return RT_ERROR;
    }

    rt_kprintf("find %s !\n", uart_name);

    /* 初始化信号量 */
    rt_sem_init(&rx_sem, "rx_sem", 0, RT_IPC_FLAG_FIFO);
    /* 以中断接收及轮询发送模式打开串口设备 */
    rt_device_open(serial, RT_DEVICE_FLAG_INT_RX);
    /* 设置接收回调函数 */
    rt_device_set_rx_indicate(serial, uart_input);
    /* 发送字符串 */
    rt_device_write(serial, 0, str, (sizeof(str) - 1));
    rt_kprintf("write to %s done!\n", uart_name);

    /* 创建 serial 线程 */
    rt_thread_t thread = rt_thread_create("serial", serial_thread_entry, RT_NULL, 1024, 25, 10);
    /* 创建成功则启动线程 */
    if (thread != RT_NULL)
    {
        rt_kprintf("thread startup!\n");
        rt_thread_startup(thread);
    }
    else
    {
        ret = RT_ERROR;
    }

    return ret;
}
/* 导出到 msh 命令列表中 */
MSH_CMD_EXPORT(uart_sample, uart device sample);

//void MQTT_Sent_entry(void *parameter)
//{
//    char Sent_msg[256];
//    MQTTMessage message;
//    message.qos = QOS1;
//    message.retained = 0;
//    while(1)
//    {
//
//    sprintf(Sent_msg,"{\"id\":\"1\",\"params\":{\"temperature\":{\"value\":%.2f}}}",1.11);
//    message.payload = (void *)Sent_msg;
//    message.payloadlen = rt_strlen(message.payload);
//
//    if(init_ok=RT_TRUE && mq_client.isconnected)
//    {
//         MQTTPublish(&mq_client, ONENET_TOPIC_DP, &message);
//         rt_memset(Sent_msg, 0, 256);
//    }
//
//    rt_thread_mdelay(2000);
//    }
//}




int main(void)
{
    char Sent_msg[256];
    char chose=0;
    MQTTMessage message;
    message.qos = QOS1;
    message.retained = 0;

    //扫描wifi情况
    rt_wlan_scan();
    //等待连接
    while(rt_wlan_connect(IFX_RW007_WIFI_SSID, IFX_RW007_WIFI_PASSWORD) != RT_EOK)
    {
        rt_kprintf("Wait For WIFI Connect\n");
        rt_thread_mdelay(1000);
    }
    rt_thread_mdelay(2000);//等待获取IP

#if defined IOT_THREAD_ENABLE
    extern void iot_thread_creat(void);
    iot_thread_creat();
#endif

    onenet_mqtt_init();

//    LED_thread=rt_thread_create("LED",LED_entry, RT_NULL, 512, 30, 20);
    TIME_Delay=rt_timer_create("Delay", TIME_entry, RT_NULL, 100, RT_TIMER_FLAG_PERIODIC|RT_TIMER_FLAG_SOFT_TIMER);
//  MQTT_Sent_thread=rt_thread_create("MQTT",MQTT_Sent_entry, RT_NULL, 512, 30, 20);

//    rt_thread_startup(LED_thread);
    rt_timer_start(TIME_Delay);
//  rt_thread_startup(MQTT_Sent_thread);

    app_bh1750_init();
    adc1_vol_init();
    ventilator_init();
    water_pump_init();
    LED_init();

    while(1)
    {

        if(chose==0)
        {
            sprintf(Sent_msg,"{\"id\":\"1\",\"params\":{\"humidity\":{\"value\":%.2f},\"elevation\":{\"value\":%.2f},\"pressure\":{\"value\":%.2f}}}",SHT31_Hum,BMP_Approx,BMP_Pressure);
        }
        if(chose==1)
        {
            sprintf(Sent_msg,"{\"id\":\"2\",\"params\":{\"temperature\":{\"value\":%.2f},\"Light\":{\"value\":%.1f},\"Soil_moisture\":{\"value\":%.2f}}}",SHT31_Temp,Light,Soil_moisture);
        }
        if(chose==2)
        {
            sprintf(Sent_msg,"{\"id\":\"3\",\"params\":{\"LED\":{\"value\":%d},\"ventilator1\":{\"value\":%d},\"ventilator2\":{\"value\":%d},\"water_pump\":{\"value\":%d}}}",LED_Set,ventilator1_set,ventilator2_set,water_pump_set);
        }


        chose++;
        chose%=3;

        message.payload = (void *)Sent_msg;
        message.payloadlen = rt_strlen(message.payload);

        if(init_ok=RT_TRUE && mq_client.isconnected)
        {
             MQTTPublish(&mq_client, ONENET_TOPIC_DP, &message);
             rt_memset(Sent_msg, 0, 256);
        }

        rt_thread_mdelay(1000);

    }

}

