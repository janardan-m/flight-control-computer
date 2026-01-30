# Flight Control Computer (FCC)

## Overview
This project implements a **custom embedded Flight Control Computer (FCC)**
for fixed-wing aircraft with emphasis on **deterministic execution,
timing correctness, and system observability**.

This repository represents a **real engineering system**, not a demo.

## Design Goals
- Deterministic control-loop execution
- Clear separation of sensing, control, and actuation
- Robust debugging and observability
- Safety-oriented design principles

## Architecture Overview
- Sensor acquisition
- State estimation and filtering
- Control law execution
- Actuator command generation
- Health monitoring and fault handling

## SDK, Build & Flash Instructions
Vendor SDKs are intentionally **not included**.

Detailed instructions for:
- SDK installation
- Project generation
- Build configuration
- Programming and testing

are documented here:

👉 https://www.bitsonboard.co.in

## Project Status
## Current Status (MPU + USER Mode Baseline)

This project currently implements a working **partitioned kernel** on the
TMS570 (Cortex-R4) using **MPU enforcement, USER/PRIVILEGED mode separation,
and SVC-based system calls**.

### Execution Model
- Kernel runs in **Privileged (SVC) mode**
- Application partitions run in **USER mode**
- CPU mode transitions are verified via CPSR
  - USER mode: `0x10`
  - SVC/Kernel: `0x13`
- Refer kernel_svc.c file for kernel_svc_dispatch call, add peripherals related code in this function

### Time Partitioning
- 4 application partitions
- Each partition executes for ~5 ms
- Partitions are scheduled cyclically:


## Non-Goals
- Autopilot feature completeness
- Navigation or mission planning

