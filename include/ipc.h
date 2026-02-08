#ifndef IPC_H
#define IPC_H

#include <stdint.h>

/* ---------------- System config ---------------- */
#define IPC_MAX_PARTITIONS   4
#define IPC_MAX_KEYS         8
#define IPC_MAX_DATA_SIZE    64

/* ---------------- SVC ops ---------------- */
#define OP_IPC_PUBLISH   0x30
#define OP_IPC_READ      0x31

/* ---------------- IPC keys (data products) ---------------- */
typedef enum {
    IPC_KEY_IMU = 0,
    IPC_KEY_AIRDATA,
    IPC_KEY_CONTROL_CMD,
    IPC_KEY_SERVO_OUT,
    IPC_KEY_HEALTH,
    IPC_KEY_MAX
} ipc_key_t;

/* ---------------- Flight data types ---------------- */
typedef struct {
    float ax, ay, az;
    float gx, gy, gz;
} imu_data_t;

typedef struct {
    float airspeed_mps;
    float altitude_m;
} airdata_t;

typedef struct {
    float rudder;
    float elevator;
    float aileron;
    float flaps;
} control_cmd_t;

typedef struct {
    uint16_t rudder_pwm;
    uint16_t elevator_pwm;
    uint16_t aileron_pwm;
    uint16_t flaps_pwm;
} servo_out_t;

typedef struct {
    uint32_t alive_counter;
    uint32_t last_fault;
} health_t;

#endif /* IPC_H */
