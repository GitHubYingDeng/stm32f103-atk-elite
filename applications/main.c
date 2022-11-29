/**
 * @file main.c
 * @author DengYing (1455891964@qq.com)
 * @brief
 * @version V1.0.0_20221128_Beta
 * @date 2022-11-28
 *
 * @copyright DengYing (c) 2022
 *
 * @MCU: STM32F103ZET6
 * @par 修改日志:
 * @note:
 * <table>
 * <tr><th>Date        <th>Version   <th>Author    <th>Description  </tr>
 * <tr><td>2022-11-28     <td>V1.0.0_20221128_Beta       <td>DengYing     <td>
 * </tr>
 * </table>
 */
/******************Private includes**********************/
#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>
/**************************end***************************/
/********************Private define**********************/
#define LED0_PIN GET_PIN(B, 5)
#define LED1_PIN GET_PIN(E, 5)
#define BEEP_PIN GET_PIN(B, 8)
/**************************end***************************/
/*******************Private variables********************/

/**************************end***************************/
/********************Static variables********************/
/* 定义线程控制块 */
static rt_thread_t APPCreate_thread = RT_NULL;
static rt_thread_t Led1_thread = RT_NULL;
static rt_thread_t Led2_thread = RT_NULL;
static rt_thread_t Button_thread = RT_NULL;
static rt_thread_t Scan10ms_thread = RT_NULL;
/**************************end***************************/
/**********************static function*******************/
static void APPCreate_thread_entry(void *parameter);
static void Button_thread_entry(void *parameter);
static void Scan10ms_thread_entry(void *parameter);
static void Led1_thread_entry(void *parameter);
static void Led2_thread_entry(void *parameter);
/**************************end***************************/

/**
 * @brief
 *
 * @param[in] parameter
 */
static void APPCreate_thread_entry(void *parameter)
{

    Button_thread = rt_thread_create("Button",
                                     Button_thread_entry,
                                     RT_NULL,
                                     512,
                                     3,
                                     20);
    rt_thread_startup(Button_thread);
    Scan10ms_thread = rt_thread_create("Scan10ms",
                                       Scan10ms_thread_entry,
                                       RT_NULL,
                                       512,
                                       2,
                                       20);
    rt_thread_startup(Scan10ms_thread);
    Led1_thread = rt_thread_create("Led1",
                                   Led1_thread_entry,
                                   RT_NULL,
                                   512,
                                   3,
                                   20);
    rt_thread_startup(Led1_thread);
    Led2_thread = rt_thread_create("Led2",
                                   Led2_thread_entry,
                                   RT_NULL,
                                   512,
                                   3,
                                   20);
    rt_thread_startup(Led2_thread);

    /* 所有线程创建完毕，关闭此线程 */
 //   rt_thread_delete(APPCreate_thread);
}

/**
 * @brief
 *
 * @param[in] parameter
 */
static void Scan10ms_thread_entry(void *parameter)
{
    rt_tick_t premtime;
    while (1)
    {

        rt_thread_delay_until(&premtime, 10);
    }
}
/**
 * @brief
 *
 * @param[in] parameter
 */
static void Led1_thread_entry(void *parameter)
{

    int count = 1;
    /* set LED0 pin mode to output */
    rt_pin_mode(LED0_PIN, PIN_MODE_OUTPUT);

    while (count++)
    {
        rt_pin_write(LED0_PIN, PIN_HIGH);
        rt_thread_mdelay(500);
        rt_pin_write(LED0_PIN, PIN_LOW);
        rt_thread_mdelay(500);
    }
}
/**
 * @brief
 *
 * @param[in] parameter
 */
static void Led2_thread_entry(void *parameter)
{
    int count = 1;
    /* set LED0 pin mode to output */
    rt_pin_mode(LED1_PIN, PIN_MODE_OUTPUT);
    while (count++)
    {
        rt_pin_write(LED1_PIN, PIN_HIGH);
        rt_thread_mdelay(500);
        rt_pin_write(LED1_PIN, PIN_LOW);
        rt_thread_mdelay(500);
    }
}
/**
 * @brief
 *
 * @param[in] parameter
 */
static void Button_thread_entry(void *parameter)
{

    while (1)
    {
        rt_thread_mdelay(500);
    }
}
void LVGL_Entry(void *params){

    while(1){

    }
}

/**
 * @brief
 *
 * @return int
 */
int main(void)
{
    APPCreate_thread = rt_thread_create("APPCreate_thread",
                                        APPCreate_thread_entry,
                                        RT_NULL,
                                        512,
                                        3,
                                        20);

    if (APPCreate_thread != RT_NULL)
    {
        rt_thread_startup(APPCreate_thread);
    }
    else
        return RT_EOK;
}
