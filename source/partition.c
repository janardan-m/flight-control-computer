#include "partition.h"
#include "sys_pmu.h"   /* HALCoGen PMU APIs: _pmuGetCycleCount_(), etc */
#include <stdint.h>
#include "svc_call.h"

#define OP_GPIOA_TOGGLE 1u
#define OP_GPIOB_TOGGLE 2u



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
    svc_call(OP_GPIOB_TOGGLE, 1 /*pin*/, 0, 0);
}

void partition1_estimate(void)
{
    svc_call(OP_GPIOB_TOGGLE, 1 /*pin*/, 0, 0);
}

void partition2_control(void)
{
    svc_call(OP_GPIOB_TOGGLE, 1 /*pin*/, 0, 0);
}

void partition3_actuate(void)
{
    svc_call(OP_GPIOB_TOGGLE, 1 /*pin*/, 0, 0);
}
