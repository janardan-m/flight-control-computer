#include "partition.h"
#include "sys_pmu.h"   /* HALCoGen PMU APIs: _pmuGetCycleCount_(), etc */
#include <stdint.h>
#include "svc_call.h"
#include "sys_common.h"
#include "sys_mpu.h"

/* Fixed shared RAM region from your screenshot (Region 3) */
#define SHARED_RAM_BASE   (0x08008000U)
#define SHARED_RAM_SIZE   (MPU_16_KB)

/* Which MPU regions we are using dynamically */
#define MPU_REGION_SHARED   (mpuREGION3)   /* Region 3 in HALCoGen UI */
#define MPU_REGION_ACTIVE   (mpuREGION4)   /* Region 4 in HALCoGen UI */

extern
volatile uint32_t g_user_sp_top;



#define OP_GPIOA_TOGGLE 1u
#define OP_GPIOB_TOGGLE 2u

#define P_RAM_SIZE   (0x4000u)  /* 16KB */

typedef struct {
    uint32 ram_base;
    uint32 ram_size;     /* must be MPU_* enum size (like MPU_16_KB), not raw bytes */
    uint32 sp_top;       /* absolute address: top of stack inside partition RAM */
} partition_mem_t;

/* Example based on your RAM blocks */
static const partition_mem_t g_part_mem[4] =
{
    /* P0 */ { 0x0800C000U, MPU_16_KB, 0x08010000U - 0x40U },
    /* P1 */ { 0x08010000U, MPU_16_KB, 0x08014000U - 0x40U },
    /* P2 */ { 0x08014000U, MPU_16_KB, 0x08018000U - 0x40U },
    /* P3 */ { 0x08018000U, MPU_16_KB, 0x0801C000U - 0x40U },
};


static volatile uint32_t g_last_overrun_pid = 0xFFFFFFFFu;
static volatile uint32_t g_last_overrun_cycles = 0;

/* Example partition work (replace with your real code) */
void partition0_acquire(void)
{
    *(int *)0x0800C000 = 1;   // own RAM  OK
    *(int *)0x08008000 = 3;   // shared RAM OK
//    *(int *)0x08000000 = 4;   // kernel RAM MUST DABORT
//    *(int *)0x08010000 = 2;   // P1 RAM MUST DABORT
//    gioToggleBit(gioPORTB, 1); // Peripheral access Aport
    svc_call(OP_GPIOB_TOGGLE, 1 /*pin*/, 0, 0);
}

void partition1_estimate(void)
{

    *(int *)0x08010000 = 2;   // P1 RAM ok
    *(int *)0x08008100 = 3;   // shared RAM OK
//    *(int *)0x08000000 = 4;   // kernel RAM MUST DABORT
//    *(int *)0x0800C000 = 1;   // P0 Abort
    svc_call(OP_GPIOB_TOGGLE, 1 /*pin*/, 0, 0);
}

void partition2_control(void)
{
    *(int *)0x08014000 = 3;   //P2 RAM 0k
    *(int *)0x08008110 = 3;   // shared RAM OK
//    *(int *)0x08000000 = 4;   // kernel RAM MUST DABORT
//    *(int *)0x08010000 = 2;   // P1 RAM abort
//    ((void (*)(void))0x08008000)();   // RAM marked NOEXEC Pabort

    svc_call(OP_GPIOB_TOGGLE, 1 /*pin*/, 0, 0);
}

void partition3_actuate(void)
{
    *(int *)0x08018000 = 3;   //P3 RAM 0k
    *(int *)0x08008120 = 3;   // shared RAM OK
//    *(int *)0x08000000 = 4;   // kernel RAM MUST DABORT
//    *(int *)0x0800c004 = 2;   // P0 RAM abort
    svc_call(OP_GPIOB_TOGGLE, 1 /*pin*/, 0, 0);
}

void mpu_set_active_partition(uint32 pid)
{
    const partition_mem_t *p = &g_part_mem[pid];

    /* 1) Update the USER stack top used by run_user.asm */
    g_user_sp_top = p->sp_top;

    /* 2) Reprogram MPU safely (do it in privileged mode only) */
    _mpuDisable_();

    /* ---- Region 4: Active Partition RAM (this is what changes per switch) ---- */
    _mpuSetRegion_(MPU_REGION_ACTIVE);
    _mpuSetRegionBaseAddress_(p->ram_base);
    _mpuSetRegionTypeAndPermission_(MPU_NORMAL_OINC_NONSHARED,
                                    MPU_PRIV_RW_USER_RW_NOEXEC);
    _mpuSetRegionSizeRegister_(mpuREGION_ENABLE | p->ram_size);

    _mpuEnable_();
}
