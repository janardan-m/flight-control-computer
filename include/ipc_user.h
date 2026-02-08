#include "ipc.h"

static inline int ipc_publish(ipc_key_t key,
                              const void *data,
                              uint32_t size)
{
    return svc_call(OP_IPC_PUBLISH,
                    (uint32_t)key,
                    (uint32_t)data,
                    size);
}

static inline int ipc_read(ipc_key_t key,
                           void *out)
{
    return svc_call(OP_IPC_READ,
                    (uint32_t)key,
                    (uint32_t)out,
                    0);
}
