/** @file sys_main.c 
*   @brief Application main file
*   @date 11-Dec-2018
*   @version 04.07.01
*
*   This file contains an empty main function,
*   which can be used for the application.
*/

/* 
* Copyright (C) 2009-2018 Texas Instruments Incorporated - www.ti.com 
* 
* 
*  Redistribution and use in source and binary forms, with or without 
*  modification, are permitted provided that the following conditions 
*  are met:
*
*    Redistributions of source code must retain the above copyright 
*    notice, this list of conditions and the following disclaimer.
*
*    Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in the 
*    documentation and/or other materials provided with the   
*    distribution.
*
*    Neither the name of Texas Instruments Incorporated nor the names of
*    its contributors may be used to endorse or promote products derived
*    from this software without specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
*  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
*  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
*  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
*  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
*  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
*  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
*  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
*  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
*  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
*/


/* USER CODE BEGIN (0) */
/* USER CODE END */

/* Include Files */

#include "sys_common.h"
#include "system.h"
#include "gio.h"
#include "het.h"
#include "sys_vim.h"
#include "sys_pmu.h"
#include "sci.h"
#include "partition.h"
#include "sys_mpu.h"
#include "run_user.h"
#include "health.h"

/* ---- Scheduling flags ---- */
extern volatile uint8_t g_minor_flag;
extern volatile uint32_t g_minor_tick;

/*
 * Stores the kernel return address when entering USER mode.
 * Written by run_user.asm (privileged mode)
 * Read by undef_handler.asm (exception return)
 */
volatile unsigned int g_run_user_lr = 0;

volatile uint32_t g_user_sp_top = 0;

volatile uint32 g_kernel_sp = 0;

void sciDisplayText(sciBASE_t *sci, uint8 *text, uint32 length);

void sciDisplayText(sciBASE_t *sci, uint8 *text, uint32 length)
{
    while (length--)
    {
        while (!sciIsTxReady(sci));     // Wait for TX ready
        sciSendByte(sci, *text++);
    }
}

void pmu_start_cycle_counter(void)
{
    _pmuInit_();                      // enable PMU
    _pmuEnableCountersGlobal_();      // global enable
    _pmuResetCounters_();             // reset all
    _pmuStartCounters_(pmuCYCLE_COUNTER); // START CYCLE COUNTER
}

int main(void)
{
    uint8 message[] = "Flight Control computer data prompt\r\n";

    /* === Core init === */
    systemInit();
    vimInit();

    /* === Peripheral init === */
    gioInit();
    hetInit();
    pmu_start_cycle_counter();
    sciInit();
    sciDisplayText(sciREG, message, sizeof(message) - 1);

    gioSetDirection(gioPORTB, (1U << 1) | (1U << 2));
    gioSetBit(gioPORTB, 1, 0);
    gioSetBit(gioPORTB, 2, 0);

    /* === MPU ON (before IRQs) === */
    _mpuInit_();
    _mpuEnable_();



    /* === Now enable interrupts === */
    vimEnableInterrupt(24, SYS_IRQ);
    _enable_IRQ();

    uint8_t cur = 0;

    while (1)
    {
        /* Nothing here ? all timing via HET */
        if (g_minor_flag == 0)
        {
            continue; /* later you can WFI */
        }

        g_minor_flag = 0;

        // round robin
        if (health_is_faulted(cur)) {
            cur++;
            if (cur >= PART_COUNT) cur = 0;
            continue;
        }

        // set which partition is active (for fault handlers)
        g_active_pid = cur;

        mpu_set_active_partition(cur);
        /* Enter USER mode and run partition */
        run_user(g_partitions[cur].entry, 0);


        cur++;
        if (cur >= PART_COUNT)
        {
            cur = 0;
        }

    }
}

/* USER CODE BEGIN (4) */
/* USER CODE END */
