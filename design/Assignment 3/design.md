# Assignment 3 - XM23P Data Pipeline

<div style="position: absolute; top: 0; right: 0;">Zachary Fraser</div>

This assignment implements 9 branching instructions to the XM23P emulator:

1. Branch with Link
2. Branch if equal or zero
3. Branch if not equal or not zero
4. Branch if carry/higher or same (unsigned)
5. Branch if no carry/lower (unsigned)
6. Branch if negative
7. Branch if greater or equal (signed)
8. Branch if less (signed)
9. Branch Always

These instructions allow for programs with flow of control change using branching.

## Design

The design contains logic flowcharts detailing the CPU Loop, and the Fetch, Decode, and Execute Stages.  A Data dictionary describing the instructions and register file is also included.

![CPU Cycle Flowchart](CPU_Cycle.svg)

<!-- Page Break -->
<div style="page-break-after: always;"></div>

![Fetch Stage Flowchart](Fetch.svg)

<!-- Page Break -->
<div style="page-break-after: always;"></div>

![Decode Stage Flowchart](Decode.svg)

<!-- Page Break -->
<div style="page-break-after: always;"></div>

![Execute Stage Flowchart](Execute.svg)

<!-- Page Break -->
<div style="page-break-after: always;"></div>

### Data Dictionary

```  PSEUDOCODE
IMEM            =   32*2^10{WORD}32*2^10
IMAR            =   ADDRESS
ICTRL           =   [READ|WRITE]
IMBR            =   WORD
IR              =   WORD

DMEM            =   64*2^10{BYTE}64*2^10
DMAR            =   ADDRESS
DCTRL           =   [READ|WRITE]
DMBR            =   WORD

REGFILE         =   3{GPR}3 + BP + LR + SP + PC
GPR             =   WORD *General Purpose Register*
BP              =   WORD *Base Pointer*
LR              =   WORD *Link Register*
SP              =   WORD *Stack Pointer*
PC              =   WORD *Program Counter*

PSW             =   PRV_PRI + 4{DC}4 + FLT + CUR_PRI + V + SLP + N + Z + C
PRV_PRI         =   3{BIT}3 *Previous Priority*
DC              =   BIT     *Don't Care*
FLT             =   BIT     *Fault*
CUR_PRI         =   3{BIT}3 *Current Priority*
V               =   BIT     *Arithmetic overflow*
SLP             =   BIT     *Sleep State*
N               =   BIT     *Negative Result*
Z               =   BIT     *Zero Result*
C               =   BIT     *Carry*

BREAKPOINT      =   ADDRESS
START_ADDRESS   =   ADDRESS

INSTRUCTION     =   CODE + 1{PARAMETER}4
CODE            =   [0-20] *Contiguous encoding of instructions*
PARAMETER       =   [RC|WB|SOURCE|DESTINATION|BYTE]

RC              =   BIT
WB              =   BIT
SOURCE          =   3{BIT}3
DESTINATION     =   3{BIT}3
READ            =   0x0000
WRITE           =   0x0001

ADDRESS         =   WORD
WORD            =   2{BYTE}2
BYTE            =   8{BIT}8
BIT             =   [0|1]
```

<!-- Page Break -->
<div style="page-break-after: always;"></div>

## Testing

The following tests were implemented:

- Test_XX: Test One Name
- Test_XX: Test Two Name
- Test_XX: Test Three Name

<!-- Page Break -->
<div style="page-break-after: always;"></div>

### Test_XX: Test One Name

#### Purpose

Test description.

#### Configuration

.\tests\Execute_Tests\Input_Files\TestXX.in

1) TestXX_Test_File.xme was loaded into the emulator.
2) `b xxxx` was entered to set a breakpoint at address `#xxxx`
3) `g` was entered to run the program
4) `m d xxxx yyyy` was entered to dump the data memory between address xxxx and yyyy.

#### Expected Results

Expected Results

#### Results

The register contents correctly matched:\
<img src="Test_XX.png" alt="Test XX" width="50%">

#### Pass/Fail

Pass.

<!-- Page Break -->
<div style="page-break-after: always;"></div>