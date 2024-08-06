# Assignment 5 - Conditional Execution

<div style="position: absolute; top: 0; right: 0;">Zachary Fraser</div>

This assignment aims to implement conditional execution of instructions other than branches using the CEX instruction.  This will allow the XM23P to execute conditional code in a way that is less expensive than branching.

## Design

The design contains logic flowcharts detailing the conditional execution.  A Data dictionary describing the instructions, PSW, and register file is also included.

![Conditional Execution](Conditional_Execution.svg)

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

REGFILE         =   3{WORD}3 + BP + LR + SP + PC
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

START_ADDRESS   =   ADDRESS

INSTRUCTION     =   CODE + 1{PARAMETER}4
CODE            =   [0-20] *Contiguous encoding of instructions*
PARAMETER       =   [RC|WB|SOURCE|DESTINATION|BYTE|T_COUNT|F_COUNT|CONDITION_CODE]

RC              =   BIT
WB              =   BIT
SOURCE          =   3{BIT}3
DESTINATION     =   3{BIT}3

T_COUNT         =   [0-7]           *Number of instructions to execute if condition is true*
F_COUNT         =   [0-7]           *Number of instructions to execute if condition is false*
CONDITION_CODE  =   [#0000 - #1111] *Corresponds to execution condition*

ADDRESS         =   WORD
WORD            =   2{BYTE}2
BYTE            =   8{BIT}8
BIT             =   [0|1]
```

## Testing

The following tests were implemented:

- [Test XX: True Condition](#test-xx-true-condition)
- [Test XX: False Condition](#test-xx-false-condition)
- [Test XX: No True False](#test-xx-no-true-false)
- [Test XX: Branch](#test-xx-branch)

Each test may be run from a powershell terminal with the following command:

``` powershell
Get-Content '.\Path\To\Input\File' | '.\Path\To\Executable'
```

<!-- Page Break -->
<div style="page-break-after: always;"></div>

### Test XX: True Condition

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

### Test XX: False Condition

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

### Test XX: No True False

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

### Test XX: Branch

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
