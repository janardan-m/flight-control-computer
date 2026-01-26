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
#include "sci.h"
#include "sys_common.h"
#include "reg_gio.h"

/* USER CODE BEGIN (1) */
/* USER CODE END */

/** @fn void main(void)
*   @brief Application main function
*   @note This function is empty by default.
*
*   This function is called after startup.
*   The user can use this function to implement the application.
*/

/* USER CODE BEGIN (2) */
/* USER CODE END */

void sciDisplayText(sciBASE_t *sci, uint8 *text, uint32 length);

void sciDisplayText(sciBASE_t *sci, uint8 *text, uint32 length)
{
    while (length--)
    {
        while (!sciIsTxReady(sci));     // Wait for TX ready
        sciSendByte(sci, *text++);
    }
}

void delay(void)
{
    volatile uint32 i;
    for(i = 0; i < 1000000; i++) {}  // crude delay
}

int main(void)
{
/* USER CODE BEGIN (3) */
/* USER CODE END */
    sciInit();  // Initializes both sciREG and scilinREG
    gioInit();  // Initialize the GIO module


    uint8 message[] = "My First UART program on herculus\r\n";

    // Set GIOB_1 and GIOB_2 as output (bit positions 1 and 2)
    gioSetDirection(gioPORTB, (1U << 1) | (1U << 2));  // Set bit 1 and 2 as output

    while (1)
    {
        // Set GIOB_1 = 1, GIOB_2 = 0
        gioSetBit(gioPORTB, 1, 1);
        gioSetBit(gioPORTB, 2, 0);
        delay();

        // Set GIOB_1 = 0, GIOB_2 = 1
        gioSetBit(gioPORTB, 1, 0);
        gioSetBit(gioPORTB, 2, 1);
        delay();
        sciDisplayText(sciREG, message, sizeof(message) - 1);
//        for (volatile int i = 0; i < 500000; i++); // crude delay
    }

    return 0;

}


/* USER CODE BEGIN (4) */
/* USER CODE END */
