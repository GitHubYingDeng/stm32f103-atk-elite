/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-12-04     zylx         The first version for STM32F4xx
 */

#ifndef __DRV_SRAM_H__
#define __DRV_SRAM_H__

/* parameters for sdram peripheral */
/* Bank1 */
#define SRAM_TARGET_BANK                FSMC_NORSRAM_BANK4
/* stm32f4 Bank1:0x60000000 */
#define SRAM_BANK_ADDR                  ((uint32_t)0x60000000)
/* data width: 8, 16, 32 */
#define SRAM_DATA_WIDTH                 16
/* sram size */
#define SRAM_SIZE                       ((uint32_t)0x100000)

/* Timing configuration for IS61WV102416BLL-10MLI */

#endif
