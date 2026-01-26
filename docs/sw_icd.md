# Hercules STOL Flight Computer  
## Software Interface Control Document (SW-ICD)

---

## 0. Scope

Defines the software architecture, interfaces, rules, and guarantees for:

- ARINC-653-style partitioning
- Deterministic C coding rules
- Redundancy strategy (dual-lane + voting hooks)
- BIT (PBIT / CBIT / IBIT)
- Watchdog supervision
- Profiling / WCET / load monitoring
- Logging and black-box recording
- Fault handling and degraded modes
- ICD-level APIs and data objects

---

## 1. System Model

### 1.1 ARINC 653–Style Concepts Implemented

- **Time Partitioning:** Cyclic executive with fixed Major Time Frame (MTF)
- **Space Partitioning (Pragmatic):**
  - Static memory ownership
  - Interface-only sharing
  - Optional MPU
  - CRC / canaries for integrity
- **Ports:** Sampling & Queuing ports
- **Health Monitor:** Central fault manager + watchdog supervisor

---

## 2. Partition List and Responsibilities

### P1 — SENSORS
- Acquire IMU / airspeed / baro / ADC rails
- Timestamp acquisition
- Minimal filtering
- Publish to sampling ports
- Detect comm faults

### P2 — ESTIMATION
- Consume sensor ports
- Produce state estimate
- Publish confidence flags

### P3 — CONTROL
- Consume state + RC input
- Rate damping / attitude hold
- Apply limiters
- Output control demand vector

### P4 — ACTUATION
- Mix surfaces
- Flap schedule + pitch compensation
- Arming/inhibit gate
- PWM updates
- Saturation tracking

### P5 — HEALTH MONITOR
- CBIT checks
- Deadline & jitter monitoring
- **Only watchdog owner**
- Fault severity handling
- BIT state machine
- Black-box snapshot trigger

### P6 — LOGGING / TELEMETRY *(Optional / Slack)*
- Rate-limited telemetry
- Ring-buffer dump
- Summary statistics

---

## 3. Time Partitioning and Scheduler

- **MTF:** 10 ms (100 Hz)
- Fixed partition windows
- Late partitions do not shift schedule

### Overrun Policy
- Partition overrun → fault event
- Frame overrun → major fault, watchdog gating

---

## 4. C Rules and Determinism Constraints

### Mandatory (Flight Mode)
- No dynamic allocation
- No recursion
- No unbounded loops
- Fixed-point preferred (FP only if measured)
- Fixed-size buffers
- Explicit interfaces only

### Defensive Coding
- Range checks on all inputs
- Saturation arithmetic
- Debug asserts → faults in release

### Build Profiles
- **DEV:** logs + relaxed watchdog
- **FLIGHT:** strict budgets + watchdog enabled

---

## 5. Port Interface Definitions

### Sampling Ports
- Latest value always available
- Atomic update
- Non-blocking read

Examples:
- `SP_SensorData`
- `SP_StateEstimate`
- `SP_RCInput`
- `SP_PowerHealth`

### Queuing Ports
- Fixed-depth ring buffer
- Bounded enqueue
- Overflow → counter + fault event

Examples:
- `QP_Events`
- `QP_Commands`

---

## 6. Data Objects (Units Mandatory)

### Sensor Data
- IMU gyro / accel
- Airspeed
- Baro altitude & vertical speed
- ADC rails
- Timestamp + validity flags

### State Estimate
- Body rates
- Attitude (roll/pitch/yaw)
- Confidence flags
- Timestamp

### RC Input
- Normalized channels
- Raw µs
- Link status
- Mode / arming

### Control Demand
- Roll / pitch / yaw
- Throttle
- Flaps
- Limit flags

### Actuator Output
- Servo pulse widths
- Inhibit flag
- Timestamp

---

## 7. Built-In Test (BIT)

### PBIT
- Flash CRC
- RAM test
- Timer check
- ADC rails
- Sensor presence
- PWM inhibit check

### CBIT
- Sensor plausibility
- Deadline monitoring
- Stack canaries
- Port overflow
- Power rails

### IBIT
- Deeper sensor tests
- Surface sweep (throttle inhibited)

---

## 8. Watchdog Supervision Policy

- **Owner:** Health Monitor only
- Kick allowed only if:
  - P1–P4 complete
  - No critical faults
  - Sensors + RC valid
  - Power rails safe

Failure → failsafe or reset

---

## 9. Profiling, WCET, Load

- Per-partition runtime
- Frame time & slack
- Jitter
- High-water marks
- Rolling load %

Actions:
- Reduce telemetry
- Disable non-critical features
- Degrade or reset

---

## 10. Logging / Black Box Recorder

- Constant-time RAM ring buffer
- Fault snapshots preserve last N seconds
- Dump on reboot

---

## 11. Redundancy Design

- Dual-lane estimator/control
- Mismatch detection
- Optional triple-vote hooks
- Sensor cross-checking

---

## 12. Fault Management & Degraded Modes

- Severity: INFO / WARN / MAJOR / CRITICAL
- Degraded ladder:
  - Disable airspeed scheduling
  - Disable assist modes
  - Manual passthrough
  - Failsafe outputs

---

## 13. Module Interfaces

### Drivers
`drv_imu_*`, `drv_adc_*`, `drv_pwm_*`, `drv_timer_*`, `drv_uart_*`

### Core Services
`scheduler_*`, `profiling_*`, `health_*`, `faultmgr_*`, `bit_*`, `log_*`

### Flight Logic
`estimator_*`, `control_*`, `mixer_fixedwing_*`, `flap_schedule_*`, `modes_*`

---

## 14. Evidence & Build Truth

Software must generate:
- WCET & load logs
- BIT arming proof
- Fault injection evidence
- Watchdog supervision proof
- Black-box pre-fault traces

