#ifndef HEALTH_H
#define HEALTH_H

#include <stdint.h>

#define MAX_PARTITIONS  4

typedef enum {
    FAULT_NONE      = 0,
    FAULT_UNDEF     = 1,
    FAULT_PABORT    = 2,
    FAULT_DABORT    = 3,
    FAULT_OTHER     = 4,
} fault_reason_t;

typedef struct {
    volatile uint32_t faulted;       // 0 = healthy, 1 = faulted
    volatile uint32_t reason;        // fault_reason_t
    volatile uint32_t count;         // number of faults
    volatile uint32_t last_fault_pc; // optional
    volatile uint32_t last_fault_lr; // optional
} part_health_t;

extern volatile uint32_t g_active_pid;                  // currently running partition id
extern volatile part_health_t g_health[MAX_PARTITIONS]; // health table

void health_mark_fault(uint32_t pid, uint32_t reason, uint32_t pc, uint32_t lr);
uint32_t health_is_faulted(uint32_t pid);

#endif
