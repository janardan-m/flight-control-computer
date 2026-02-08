#include "ipc.h"
#include <string.h>

/* Kernel must already have this */
extern uint8_t kernel_current_pid(void);

/* ---------------- IPC storage ---------------- */
typedef struct {
    uint8_t  valid;
    uint8_t  src_pid;
    uint16_t size;
    uint32_t seq;
    uint8_t  data[IPC_MAX_DATA_SIZE];
} ipc_object_t;

/* One object per key (global data products) */
static ipc_object_t ipc_table[IPC_KEY_MAX];

/* ---------------- Access control ---------------- */
static int ipc_writer_allowed(uint8_t pid, ipc_key_t key)
{
    switch (key) {
        case IPC_KEY_IMU:          return (pid == 0);
        case IPC_KEY_AIRDATA:      return (pid == 0);
        case IPC_KEY_CONTROL_CMD:  return (pid == 2);
        case IPC_KEY_SERVO_OUT:    return (pid == 3);
        case IPC_KEY_HEALTH:       return 1;   /* all allowed */
        default:                   return 0;
    }
}

/* ---------------- IPC publish ---------------- */
uint32_t ipc_publish_kernel(uint32_t key,
                                   uint32_t size,
                                   uint32_t user_ptr)
{
    uint8_t pid = kernel_current_pid();

    if (key >= IPC_KEY_MAX)
        return 1;

    if (!ipc_writer_allowed(pid, (ipc_key_t)key))
        return 2;

    if (size > IPC_MAX_DATA_SIZE)
        return 3;

    ipc_object_t *obj = &ipc_table[key];

    memcpy(obj->data, (void *)user_ptr, size);
    obj->size    = size;
    obj->src_pid = pid;
    obj->seq++;
    obj->valid   = 1;

    return 0;
}

/* ---------------- IPC read ---------------- */
uint32_t ipc_read_kernel(uint32_t key,
                                uint32_t user_ptr)
{
    if (key >= IPC_KEY_MAX)
        return 1;

    ipc_object_t *obj = &ipc_table[key];

    if (!obj->valid)
        return 2;

    memcpy((void *)user_ptr, obj->data, obj->size);
    return 0;
}
