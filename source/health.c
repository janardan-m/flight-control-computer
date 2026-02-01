#include "health.h"

volatile uint32_t g_active_pid = 0;

volatile part_health_t g_health[MAX_PARTITIONS] = {0};

void health_mark_fault(uint32_t pid, uint32_t reason, uint32_t pc, uint32_t lr)
{
    if (pid >= MAX_PARTITIONS) return;

    g_health[pid].faulted = 1U;
    g_health[pid].reason = reason;
    g_health[pid].count++;
    g_health[pid].last_fault_pc = pc;
    g_health[pid].last_fault_lr = lr;
}

uint32_t health_is_faulted(uint32_t pid)
{
    if (pid >= MAX_PARTITIONS) return 1U;
    return g_health[pid].faulted;
}
