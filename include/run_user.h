#ifndef RUN_USER_H
#define RUN_USER_H

#include <stdint.h>

void run_user(void (*entry)(void), uint32_t user_sp_top);

#endif
