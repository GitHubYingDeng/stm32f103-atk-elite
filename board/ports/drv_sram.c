/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-12-04     zylx         first version
 */

#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>

#ifdef BSP_USING_SRAM
#include <drv_sram.h>

#define DRV_DEBUG
#define LOG_TAG "drv.sram"
#include <drv_log.h>

static SRAM_HandleTypeDef hsram4;

#ifdef RT_USING_MEMHEAP_AS_HEAP
static struct rt_memheap system_heap;
#endif

static int rt_hw_sram_init(void)
{
    int result = RT_EOK;
    GPIO_InitTypeDef GPIO_Initure;
    FSMC_NORSRAM_TimingTypeDef Timing = {0};
    FSMC_NORSRAM_TimingTypeDef ExtTiming = {0};
    __HAL_RCC_FSMC_CLK_ENABLE();  //使能FSMC时钟
    __HAL_RCC_GPIOD_CLK_ENABLE(); //使能GPIOD时钟
    __HAL_RCC_GPIOE_CLK_ENABLE(); //使能GPIOE时钟
    __HAL_RCC_GPIOG_CLK_ENABLE(); //使能GPIOG时钟

    //初始化PD0,1,4,5,8,9,10,14,15
    GPIO_Initure.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_8 |
                       GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_14 | GPIO_PIN_15;
    GPIO_Initure.Mode = GPIO_MODE_AF_PP;       //推挽复用
    GPIO_Initure.Pull = GPIO_PULLUP;           //上拉
    GPIO_Initure.Speed = GPIO_SPEED_FREQ_HIGH; //高速
    HAL_GPIO_Init(GPIOD, &GPIO_Initure);       //初始化

    //初始化PE7,8,9,10,11,12,13,14,15
    GPIO_Initure.Pin = GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 |
                       GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
    HAL_GPIO_Init(GPIOE, &GPIO_Initure);

    //初始化PG0,12
    GPIO_Initure.Pin = GPIO_PIN_0 | GPIO_PIN_12;
    HAL_GPIO_Init(GPIOG, &GPIO_Initure);

    /** Perform the SRAM4 memory initialization sequence
     */
    hsram4.Instance = FSMC_NORSRAM_DEVICE;
    hsram4.Extended = FSMC_NORSRAM_EXTENDED_DEVICE;
    /* hsram4.Init */
    hsram4.Init.NSBank = FSMC_NORSRAM_BANK4;
    hsram4.Init.DataAddressMux = FSMC_DATA_ADDRESS_MUX_DISABLE;
    hsram4.Init.MemoryType = FSMC_MEMORY_TYPE_SRAM;
#if SRAM_DATA_WIDTH == 8
    hsram4.Init.MemoryDataWidth = FSMC_NORSRAM_MEM_BUS_WIDTH_8;
#elif SRAM_DATA_WIDTH == 16
    hsram4.Init.MemoryDataWidth = FSMC_NORSRAM_MEM_BUS_WIDTH_16;
#else
    hsram4.Init.MemoryDataWidth = FSMC_NORSRAM_MEM_BUS_WIDTH_32;
#endif
    hsram4.Init.BurstAccessMode = FSMC_BURST_ACCESS_MODE_DISABLE;
    hsram4.Init.WaitSignalPolarity = FSMC_WAIT_SIGNAL_POLARITY_LOW;
    hsram4.Init.WaitSignalActive = FSMC_WAIT_TIMING_BEFORE_WS;
    hsram4.Init.WriteOperation = FSMC_WRITE_OPERATION_ENABLE;
    hsram4.Init.WaitSignal = FSMC_WAIT_SIGNAL_DISABLE;
    hsram4.Init.ExtendedMode = FSMC_EXTENDED_MODE_ENABLE;
    hsram4.Init.AsynchronousWait = FSMC_ASYNCHRONOUS_WAIT_DISABLE;
    hsram4.Init.WriteBurst = FSMC_WRITE_BURST_DISABLE;
    /* Timing */
    Timing.AddressSetupTime = 0x06;
    Timing.AddressHoldTime = 0;
    Timing.DataSetupTime = 26;
    Timing.AccessMode = FSMC_ACCESS_MODE_A;
    /* ExtTiming */
    ExtTiming.AddressSetupTime = 3;
    ExtTiming.AddressHoldTime = 0;
    ExtTiming.DataSetupTime = 0x06;
    ExtTiming.BusTurnAroundDuration = 0;
    ExtTiming.AccessMode = FSMC_ACCESS_MODE_A;

    /* Initialize the SRAM controller */
    if (HAL_SRAM_Init(&hsram4, &Timing, &ExtTiming) != HAL_OK)
    {
        LOG_E("SRAM init failed!");
        result = -RT_ERROR;
    }
    else
    {
        LOG_D("sram init success, mapped at 0x%X, size is %d bytes, data width is %d", SRAM_BANK_ADDR, SRAM_SIZE, SRAM_DATA_WIDTH);
#ifdef RT_USING_MEMHEAP_AS_HEAP
        /* If RT_USING_MEMHEAP_AS_HEAP is enabled, SRAM is initialized to the heap */
        rt_memheap_init(&system_heap, "sram", (void *)SRAM_BANK_ADDR, SRAM_SIZE);
#endif
    }

    return result;
}
INIT_BOARD_EXPORT(rt_hw_sram_init);

#ifdef DRV_DEBUG
#ifdef FINSH_USING_MSH
static int sram_test(void)
{
    int i = 0;
    uint32_t start_time = 0, time_cast = 0;
#if SRAM_DATA_WIDTH == 8
    char data_width = 1;
    uint8_t data = 0;
#elif SRAM_DATA_WIDTH == 16
    char data_width = 2;
    uint16_t data = 0;
#else
    char data_width = 4;
    uint32_t data = 0;
#endif

    /* write data */
    LOG_D("Writing the %ld bytes data, waiting....", SRAM_SIZE);
    start_time = rt_tick_get();
    for (i = 0; i < SRAM_SIZE / data_width; i++)
    {
#if SRAM_DATA_WIDTH == 8
        *(__IO uint8_t *)(SRAM_BANK_ADDR + i * data_width) = (uint8_t)0x55;
#elif SRAM_DATA_WIDTH == 16
        *(__IO uint16_t *)(SRAM_BANK_ADDR + i * data_width) = (uint16_t)0x5555;
#else
        *(__IO uint32_t *)(SRAM_BANK_ADDR + i * data_width) = (uint32_t)0x55555555;
#endif
    }
    time_cast = rt_tick_get() - start_time;
    LOG_D("Write data success, total time: %d.%03dS.", time_cast / RT_TICK_PER_SECOND,
          time_cast % RT_TICK_PER_SECOND / ((RT_TICK_PER_SECOND * 1 + 999) / 1000));

    /* read data */
    LOG_D("start Reading and verifying data, waiting....");
    for (i = 0; i < SRAM_SIZE / data_width; i++)
    {
#if SRAM_DATA_WIDTH == 8
        data = *(__IO uint8_t *)(SRAM_BANK_ADDR + i * data_width);
        if (data != 0x55)
        {
            LOG_E("SRAM test failed!");
            break;
        }
#elif SRAM_DATA_WIDTH == 16
        data = *(__IO uint16_t *)(SRAM_BANK_ADDR + i * data_width);
        if (data != 0x5555)
        {
            LOG_E("SRAM test failed!");
            break;
        }
#else
        data = *(__IO uint32_t *)(SRAM_BANK_ADDR + i * data_width);
        if (data != 0x55555555)
        {
            LOG_E("SRAM test failed!");
            break;
        }
#endif
    }

    if (i >= SRAM_SIZE / data_width)
    {
        LOG_D("SRAM test success!");
    }

    return RT_EOK;
}
MSH_CMD_EXPORT(sram_test, sram test);
#endif /* FINSH_USING_MSH */
#endif /* DRV_DEBUG */
#endif /* BSP_USING_SRAM */
