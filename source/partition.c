#include "partition.h"
#include "sys_pmu.h"   /* HALCoGen PMU APIs: _pmuGetCycleCount_(), etc */

static volatile uint32_t g_last_overrun_pid = 0xFFFFFFFFu;
static volatile uint32_t g_last_overrun_cycles = 0;

void run_partition(const partition_t *p)
{
    uint32_t start = _pmuGetCycleCount_();

    p->entry();   /* partition code executes here */

    uint32_t end = _pmuGetCycleCount_();
    uint32_t elapsed = end - start;

    if (elapsed > p->budget_cycles)
    {
        /* For now: record and hang (later: safe mode + log + recovery) */
        g_last_overrun_pid = p->id;
        g_last_overrun_cycles = elapsed;
        while (1) { }
    }
}

/* Example partition work (replace with your real code) */
void partition0_acquire(void)
{
    /* sensor reads later; for now toggle LED */
    gioSetBit(gioPORTB, 1, 0);

}

void partition1_estimate(void)
{
    gioSetBit(gioPORTB, 1, 1);
}

void partition2_control(void)
{
    gioSetBit(gioPORTB, 2, 0);
}

void partition3_actuate(void)
{
    gioSetBit(gioPORTB, 2, 1);
}
