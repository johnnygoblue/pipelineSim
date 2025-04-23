# Pipeline Simulator

A simplified hardware simulator that models a 4-stage pipeline CPU with minimal instruction set.

## Overview

This simulator implements a 4-register CPU with three basic instructions (ADD, MUL, ST) that execute through a 4-stage pipeline. The simulator demonstrates fundamental pipeline concepts including instruction staging, read/write hazards, and register conflicts.

## Specifications

### CPU Architecture
- **Registers**: 4 (reg1, reg2, reg3, reg4)
- **Pipeline Stages**: 4 (S1-S4)
- **Clock Behavior**: Exactly one new instruction enters the pipeline each cycle

### Instruction Set

| Instruction | Format | Behavior | Stage Operations |
|-------------|--------|----------|------------------|
| **ADD**     | `ADD123` | Adds reg1 + reg2 → reg3 | S1: NOP<br>S2: Read reg1, reg2<br>S3: Write reg3<br>S4: NOP |
| **MUL**     | `MUL123` | Multiplies reg1 × reg2 → reg3 | S1: NOP<br>S2: Read reg1, reg2<br>S3: NOP<br>S4: Write reg3 |
| **ST**      | `ST1`    | Prints register value | S1: Read reg1<br>S2: Output value<br>S3: NOP<br>S4: NOP |

### Conflict Resolution
- **Read-Write Conflict**: Read operations take priority over writes
- **Write-Write Conflict**: Last write wins (youngest instruction in pipeline)
- **Stage Timing**: All operations complete within 4 cycles

## Execution Example

```text
Input: "ADD123 MUL341 ST1"

Cycle 0:
reg1: -99, reg2: 1, reg3: 4, reg4: 40
[Dummy cycle exist only to demo states before any instruction enters]

Cycle 1:
reg1: -99, reg2: 1, reg3: 4, reg4: 40
[ADD enters]

Cycle 2: 
reg1: -99, reg2: 1, reg3: 4, reg4: 40
[ADD@S2 reads reg1/reg2, MUL enters]

Cycle 3:
Output: -99
reg1: -99, reg2: 1, reg3: -98, reg4: 40
[ADD@S3 writes reg3, MUL@S2 reads, ST enters&reads]

Cycle 4:
reg1: -99, reg2: 1, reg3: -98, reg4: 40
[ST@S2 outputs -99, MUL@S3 waits]

Cycle 5:
reg1: 160, reg2: 1, reg3: -98, reg4: -40
[MUL@S4 writes to reg1]

Cycle 6:
reg1: 160, reg2: 1, reg3: -98, reg4: -40
[No more instruction to execute]
