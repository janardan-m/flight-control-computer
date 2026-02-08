#include <stdint.h>
#include "gio.h"

/* Operation IDs (in r12 from USER) */
#define OP_GPIOA_TOGGLE   1u
#define OP_GPIOB_TOGGLE   2u
#define OP_IMU_READ       3u   /* placeholder */
#define OP_IPC_PUBLISH    4u
#define OP_IPC_READ       5u

/*
 * kernel_svc_dispatch(op, a0, a1, a2)
 * Runs in privileged SVC mode.
 * op  = operation ID (from USER r12)
 * a0/a1/a2 = args (from USER r0/r1/r2)
 *
 * Return value goes back to USER in r0.
 */
uint32_t kernel_svc_dispatch(uint32_t op, uint32_t a0, uint32_t a1, uint32_t a2)
{
    (void)a1;
    (void)a2;

    switch (op)
    {
        case OP_GPIOA_TOGGLE:
            /* Example: toggle PORTA bit a0 */
            gioToggleBit(gioPORTA, a0);
            return 0;

        case OP_GPIOB_TOGGLE:
            /* Example: toggle PORTB bit a0 */
            gioToggleBit(gioPORTB, a0);
            return 0;

        case OP_IMU_READ:
            /* Placeholder: later you’ll read IMU here in privileged mode */
            return 0;

        case OP_IPC_PUBLISH:
            /* a0 = key, a1 = user_ptr, a2 = size */
            return ipc_publish_kernel(a0, a1, a2);

        case OP_IPC_READ:
            /* a0 = key, a1 = user_ptr */
            return ipc_read_kernel(a0, a1);

        default:
            return 0xFFFFFFFFu; /* invalid op */
    }
}

