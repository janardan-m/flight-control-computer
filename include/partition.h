#ifndef PARTITION_H
#define PARTITION_H

#include "sys_common.h"
#include "system.h"
#include "gio.h"
#include "het.h"
#include "sci.h"
#include "sys_pmu.h"

#include "partition.h"

/* These symbols are defined in the linker .cmd file */
extern uint32_t __P0_BASE__;
extern uint32_t __P0_SIZE__;

extern uint32_t __P1_BASE__;
extern uint32_t __P1_SIZE__;

extern uint32_t __P2_BASE__;
extern uint32_t __P2_SIZE__;

extern uint32_t __P3_BASE__;
extern uint32_t __P3_SIZE__;

extern uint32_t __SHM_BASE__;
extern uint32_t __SHM_SIZE__;

typedef void (*partition_entry_t)(void);

typedef struct
{
    uint8_t            id;
    const char        *name;
    partition_entry_t  entry;
    uint32_t           budget_cycles;   /* PMU cycles budget (4ms worth) */
} partition_t;

/* Choose 4ms window */
#define MINOR_MS   (4u)

/* 160MHz example: cycles per ms = 160,000  => per 4ms = 640,000
   Update if your CPU clock differs */
#define CPU_HZ     (160000000u)
#define CYCLES_PER_MS (CPU_HZ / 1000u)
#define BUDGET_4MS   (CYCLES_PER_MS * MINOR_MS)

/* Runs one partition and checks overrun */
void run_partition(const partition_t *p);
void partition0_acquire(void);
void partition1_estimate(void);
void partition2_control(void);
void partition3_actuate(void);

static const partition_t g_partitions[] =
{
    { 0, "ACQ",  partition0_acquire,  BUDGET_4MS },
    { 1, "EST",  partition1_estimate, BUDGET_4MS },
    { 2, "CTRL", partition2_control,  BUDGET_4MS },
    { 3, "ACT",  partition3_actuate,  BUDGET_4MS },
};

#define PART_COUNT (sizeof(g_partitions)/sizeof(g_partitions[0]))




#endif
