/*----------------------------------------------------------------------------*/
/* sys_link_freeRTOS.cmd                                                      */
/*                                                                            */
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

/*                                                                            */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN (0) */
/* USER CODE END */
/*----------------------------------------------------------------------------*/
/* Linker Settings                                                            */

--retain="*(.intvecs)"

/* USER CODE BEGIN (1) */
/* USER CODE END */

/*----------------------------------------------------------------------------*/
/* Memory Map                                                                 */

MEMORY
{
/* Flash */
VECTORS      (X)  : origin=0x00000000 length=0x00000020
KERNEL_FLASH (RX) : origin=0x00000020 length=0x00008000
FLASH0       (RX) : origin=0x00008020 length=0x00137FE0

/* RAM (no overlaps; MPU-friendly boundaries for later) */
STACKS       (RW) : origin=0x08000000 length=0x00000800 /* 2 KB */
KRAM         (RW) : origin=0x08000800 length=0x00000800 /* 2 KB */

/* System RAM holds kernel heap + global .bss/.data (28 KB) */
SYSTEM_RAM   (RW) : origin=0x08001000 length=0x00007000 /* 0x08001000..0x08007FFF */

/* Shared memory for IPC / monitoring (16 KB) */
SHARED_RAM   (RW) : origin=0x08008000 length=0x00004000 /* 0x08008000..0x0800BFFF */

/* 4 partition private RAM blocks (16 KB each) */
P0_RAM       (RW) : origin=0x0800C000 length=0x00004000 /* 0x0800C000..0x0800FFFF */
P1_RAM       (RW) : origin=0x08010000 length=0x00004000 /* 0x08010000..0x08013FFF */
P2_RAM       (RW) : origin=0x08014000 length=0x00004000 /* 0x08014000..0x08017FFF */
P3_RAM       (RW) : origin=0x08018000 length=0x00004000 /* 0x08018000..0x0801BFFF */

/* Free / expansion RAM (80 KB) */
FREE_RAM     (RW) : origin=0x0801C000 length=0x00014000 /* 0x0801C000..0x0802FFFF */

/* USER CODE BEGIN (2) */
/* USER CODE END */
}

/* USER CODE BEGIN (3) */
/* USER CODE END */

/*----------------------------------------------------------------------------*/
/* Section Configuration                                                      */

SECTIONS
{
.intvecs : {} > VECTORS

/* FreeRTOS Kernel in protected region of Flash */
.kernelTEXT   : {} > KERNEL_FLASH
.cinit        : {} > KERNEL_FLASH
.pinit        : {} > KERNEL_FLASH

/* Rest of code to user mode flash region */
.text         : {} > FLASH0
.const        : {} > FLASH0

/* Stack placement (explicit) */
.stack        : {} > STACKS

/* FreeRTOS Kernel data in protected region of RAM */
.kernelBSS    : {} > KRAM

/* System RAM (kernel heap + global data) */
.kernelHEAP   : {} > SYSTEM_RAM
.bss          : {} > SYSTEM_RAM
.data         : {} > SYSTEM_RAM

/* Shared RAM (define vars with __attribute__((section(".shared")))) */
.sharedRAM    : {} > SHARED_RAM

/* Partition private RAM (same-file partitions supported via section tags) */
.p0RAM        : {} > P0_RAM
.p1RAM        : {} > P1_RAM
.p2RAM        : {} > P2_RAM
.p3RAM        : {} > P3_RAM

/* Optional: general-purpose/free RAM for future */
.freeRAM      : {} > FREE_RAM

/* USER CODE BEGIN (4) */
/* USER CODE END */
}

/* USER CODE BEGIN (5) */
/* USER CODE END */

/*----------------------------------------------------------------------------*/
/* Misc                                                                       */

/* USER CODE BEGIN (6) */
/* USER CODE END */

/*----------------------------------------------------------------------------*/
