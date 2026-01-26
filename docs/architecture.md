# FCC Architecture

## Control Loop
- Fixed execution period
- Sensor read → estimation → control → actuation

## Timing & Determinism
- Execution time measured and bounded
- No unbounded blocking in control context

## Safety Considerations
- Sensor sanity checks
- Actuator saturation limits
- Fail-safe states

