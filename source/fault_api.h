#ifndef FAULT_API_H
#define FAULT_API_H

#include <stdint.h>

void fault_report_undef(uint32_t pc, uint32_t lr);
void fault_report_pabort(uint32_t pc, uint32_t lr);
void fault_report_dabort(uint32_t pc, uint32_t lr);

#endif
