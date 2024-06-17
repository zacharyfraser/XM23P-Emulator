/**
 * @file definitions.h
 * @brief Type and constant definitions common to the project
 * 
 * @author Zach Fraser
 * @date 2024-05-20
 */

#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <ctype.h>

#define KILOBYTE (1 << 10) /* 1024 bytes */
#define byte_t unsigned char
#define word_t unsigned short
#define NUL '\0'

/* Bit Groupings */
#define ONE_BIT 0x01
#define TWO_BITS 0x03
#define THREE_BITS 0x07
#define FOUR_BITS 0x0F
#define FIVE_BITS 0x1F
#define SIX_BITS 0x3F
#define SEVEN_BITS 0x7F
#define EIGHT_BITS 0xFF

/* Bit Masks */
#define Bit_0   0x0001
#define Bit_1   0x0002
#define Bit_2   0x0004
#define Bit_3   0x0008
#define Bit_4   0x0010
#define Bit_5   0x0020
#define Bit_6   0x0040
#define Bit_7   0x0080
#define Bit_8   0x0100
#define Bit_9   0x0200
#define Bit_10  0x0400
#define Bit_11  0x0800
#define Bit_12  0x1000
#define Bit_13  0x2000
#define Bit_14  0x4000
#define Bit_15  0x8000

#define MAX_RECORD_LENGTH 256
#define INSTRUCTION_MEMORY_LENGTH (64 * KILOBYTE)
#define DATA_MEMORY_LENGTH (64 * KILOBYTE)

#define REGISTER_FILE_LENGTH 8
#define MAX_PATH_LENGTH 256
#define NUM_OF_INSTRUCTIONS 41

#define INSTRUCTION_MEMORY '0'
#define DATA_MEMORY '1'
#define PROGRAM_COUNTER 7
#define INSTRUCTION_NOOP 0x0000

#define ADDRESS_LENGTH 2 /* 2 Byte Memory Addressing */
#define CHECKSUM_LENGTH 1
#define CHECKSUM_VALUE 0xFF /* Expected value of sum of record bytes */

/**
 * @brief Read two characters and interpret them as a single byte.
 *        Increases character pointer.
 *
 * @param character_pointer Pointer to the character to be read.
 * @return The interpreted byte value.
 */
#define Read_Byte(character_pointer) ( \
    ((isdigit(*character_pointer) ? (*character_pointer++ - '0') \
    : (toupper(*character_pointer++) - 'A' + 10)) << 4) + \
    \
    (isdigit(*character_pointer) ? (*character_pointer++ - '0') \
    : (toupper(*character_pointer++) - 'A' + 10)) \
    )

/**
 * @brief Pipeline Cycle States
 * 
 */
typedef enum cycle_state {
    CYCLE_START = 0,
    CYCLE_WAIT_0 = 1,
    CYCLE_WAIT_1 = 2
} cycle_state_t;

/**
 * @brief Valid Record Types - Stored as Characters
 * 
 */
enum
{
NAME_TYPE           = '0',
INSTRUCTION_TYPE    = '1',
DATA_TYPE           = '2',
ADDRESS_TYPE        = '9'
};

/**
 * @brief Represents an S-Record.
 *
 */
typedef struct s_record_t
{
    byte_t type;
    byte_t length;
    byte_t address[2];
    byte_t data[MAX_RECORD_LENGTH];
}s_record_t;

/**
 * @brief Enumeration of all XM23P Instructions
 * 
 * Contiguous encoding of each instruction to be used with lookup table
 */
typedef enum instruction_type 
{
    UNDEFINED, BL, BEQ, BNE,
    BC, BNC, BN, BGE,
    BLT, BRA, ADD, ADDC,
    SUB, SUBC, DADD, CMP,
    XOR, AND, OR, BIT,
    BIC, BIS, MOV, SWAP,
    SRA, RRC, SWPB, SXT,
    SETPRI, SVC, SETCC, CLRCC,
    CEX, LD, ST, MOVL,
    MOVLZ, MOVLS, MOVH, LDR,
    STR
} instruction_type_t;

/**
 * @brief Structure representing an instruction.
 * 
 * Contains instruction type and arguments
 */
typedef struct instruction
{
    instruction_type_t type;

    byte_t source; /* Source Register/Constant Code */
    byte_t destination; /* Destination Register Code */
    byte_t rc; /* [0 = Register, 1 = Constant] */
    byte_t wb; /* [0 = Word, 1 = Byte ] */
} instruction_t;

/**
 * @brief Represents a program.
 *
 * The structure contains the program context.
 */
typedef struct program_t
{
    /* Memory Space */
    byte_t instruction_memory[INSTRUCTION_MEMORY_LENGTH];   /* 64KiB Instruction Memory */
    byte_t data_memory[DATA_MEMORY_LENGTH];                 /* 64KiB Data Memory */

    word_t register_file[REGISTER_FILE_LENGTH]; /* 8 CPU Registers */
    word_t program_status_word;                 /* Status Indicators */
    word_t instruction_memory_address_register; /* Holds the address of the instruction to be fetched */
    word_t instruction_memory_buffer_register;  /* Has the read from location specified in IMAR */
    word_t instruction_control_register;        /* Indicates if an instruction is to be read from address in IMAR */
    word_t instruction_register;                /* Holds the instruction to be decoded */

    cycle_state_t cycle_state;                  /* Current State of the CPU Cycle */
    instruction_t instruction;                  /* Current Instruction */
    int breakpoint;                             /* Address of the Breakpoint */
    int starting_address;                       /* Starting Address of the Program */
    int clock_cycles;                           /* Number of Clock Cycles */

    byte_t executable_name[MAX_RECORD_LENGTH];  /* Name of the Executable */
}program_t;

#endif /* DEFINITIONS_H */