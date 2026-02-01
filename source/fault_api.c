#include "fault_api.h"
#include "health.h"

void fault_report_undef(uint32_t pc, uint32_t lr)
{
    health_mark_fault(g_active_pid, FAULT_UNDEF, pc, lr);
}

void fault_report_pabort(uint32_t pc, uint32_t lr)
{
    health_mark_fault(g_active_pid, FAULT_PABORT, pc, lr);
}

void fault_report_dabort(uint32_t pc, uint32_t lr)
{
    health_mark_fault(g_active_pid, FAULT_DABORT, pc, lr);
}
