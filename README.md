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
- Architecture defined
- Platform abstraction in progress
- Control-loop timing analysis planned

## Non-Goals
- Autopilot feature completeness
- Navigation or mission planning

