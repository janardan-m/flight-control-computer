# Hercules STOL Flight Computer (RC)  
## Hardware Interface Control Document (HW-ICD)

---

## 0. Scope

Defines the complete hardware architecture, including:

- Controller (TI Hercules LaunchPad)
- Sensors (IMU, airspeed, barometer/altimeter, optional sensors)
- RC receiver interface
- Actuators (servos, ESC)
- Power architecture and ADC monitoring
- Connectors, buses, and minimum electrical requirements
- Expansion paths for redundancy and avionics-grade practices

---

## 1. System Overview

**System role:**  
Fixed-wing RC flight computer with STOL capability (flaps + airspeed-aware control), built with avionics discipline (BIT, health monitoring, deterministic timing, logging).

### Top-Level Blocks
- Hercules LaunchPad (FCC core)
- Sensor Suite  
  - IMU (Gyro + Accel)  
  - Airspeed (Pitot + differential pressure)  
  - Barometer/Altimeter
  - Optional: Magnetometer, GPS
- RC Input
- Actuation (PWM to servos + ESC)
- Power & Monitoring  
  - Servo rail (5V)  
  - MCU/Sensor rail (3.3V)  
  - ADC: battery/rails/optional airspeed analog

---

## 2. Controller (FCC Core)

**Controller:** TI Hercules LaunchPad: LAUNCHXL2-TMS57012

### Required Interfaces
- UART (SBUS/CRSF + console)
- SPI (preferred for IMU)
- I2C (optional for baro / airspeed)
- ADC (rails + optional analog airspeed)
- Timers (major frame tick + PWM generation)
- Watchdog
- GPIOs (status/profiling)

### Required MCU Characteristics (Avionics-Aligned)
- Deterministic timers
- Hardware watchdog
- Robust interrupt system
- Prefer ECC / lockstep-capable devices (Hercules family fits intent)

---

## 3. Sensors & Interfaces

### 3.1 IMU (Mandatory)

**Function:** Attitude and rate stabilization (core for safe flight)

**Minimum Requirements**
- Gyro: ±2000 dps
- Accel: ±16g
- Output rate: 200–1000 Hz internal  
- FCC loop: ≥100 Hz
- Interface: SPI preferred, I2C acceptable if stable

**Electrical**
- 3.3V supply
- SPI: SCLK/MOSI/MISO/CS (+ optional INT)
- No level shifting if all logic is 3.3V

**Mounting**
- Rigid mount near CG
- Optional vibration damping (avoid excessive softness)

**Redundancy Option**
- IMU2 on separate CS or bus for cross-check

---

### 3.2 Airspeed (Pitot + Differential Pressure) *(Strongly Recommended)*

**Function:**  
Stall margin, low-speed gain scheduling, takeoff/landing assistance

**Sensor Options**
- Digital differential pressure (I2C/SPI)
- Analog pressure sensor → ADC

**Requirements**
- Suitable low-speed RC range
- ≥50 Hz update (100 Hz preferred)
- Temperature compensation or software correction

**Physical**
- Pitot facing airflow
- Static source arrangement

**Redundancy**
- Dual pressure sensors or inferred secondary airspeed

---

### 3.3 Barometer / Altimeter *(Recommended)*

**Function:** Altitude and vertical speed

- Interface: I2C or SPI
- Update rate: 25–100 Hz
- Stable readings (software filtering)

---

### 3.4 Optional Sensors (Future-Ready)
- Magnetometer (heading hold)
- GPS (position / groundspeed)
- AoA sensor (advanced STOL stall protection)

---

## 4. RC Input Interface

**Mandatory:** RC receiver interface with failsafe detection

**Preferred**
- SBUS (UART, 100k baud, inverted logic possible)
- CRSF (UART, higher rate)

**Requirements**
- Frame timestamping
- Link-loss detection
- Configurable channel mapping

---

## 5. Actuation Interfaces (Servos + ESC)

**PWM Outputs**
- Aileron L/R
- Elevator
- Rudder
- Flaps
- Throttle (ESC)

**PWM Characteristics**
- Frequency: 50–200 Hz
- Pulse width: 1000–2000 µs
- PWM generation via timers (independent of control loop)

**Safety**
- Outputs inhibited until arming
- Failsafe: neutral surfaces + throttle-safe

---

## 6. Power Architecture & ADC Monitoring

### Power Domains
- Servo rail: 5V (BEC, noisy)
- MCU/Sensor rail: 3.3V (clean)

### Mandatory ADC Channels
- Battery voltage
- 5V servo rail
- 3.3V rail (recommended)
- Optional: current sense

**ADC Notes**
- Proper dividers & RC filters
- Synchronized sampling
- Brownout → degrade / failsafe based on severity

---

## 7. Status & Service Interfaces

- UART console (logging / telemetry)
- LEDs:
  - PBIT
  - Arm status
  - Fault status
- Optional buzzer (field diagnostics)
- Profiling GPIO (oscilloscope timing validation)

---

## 8. Environmental & Mechanical

- EMI-reducing enclosure
- Strain relief on servo leads
- Sensor axis alignment marking

---

## 9. Minimum Flight-Required Hardware

**Baseline Safe Flight**
- Hercules LaunchPad
- IMU
- RC receiver
- Servos + ESC
- Stable power
- ADC monitoring (battery + servo rail)

**True STOL Capability**
- Add airspeed sensor
- Add barometer

---

## 10. Expansion for Avionics-Grade Redundancy

- IMU2 / pressure sensor 2
- Additional power monitoring
- Future: dual-MCU cross-monitoring

