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

#define INSTRUCTION_MEMORY_LENGTH (64 * KILOBYTE)
#define DATA_MEMORY_LENGTH (64 * KILOBYTE)
#define REGISTER_FILE_LENGTH 8
#define MAX_PATH_LENGTH 256

#define INSTRUCTION_MEMORY '0'
#define DATA_MEMORY '1'
#define PROGRAM_COUNTER 7
#define ONE_BIT 0x01
#define TWO_BITS 0x03
#define THREE_BITS 0x07
#define FOUR_BITS 0x0F
#define FIVE_BITS 0x1F
#define SIX_BITS 0x3F
#define SEVEN_BITS 0x7F
#define EIGHT_BITS 0xFF

#define FIRST_BIT 0
#define SECOND_BIT 1
#define THIRD_BIT 2
#define FOURTH_BIT 3
#define FIFTH_BIT 4
#define SIXTH_BIT 5
#define SEVENTH_BIT 6
#define EIGHTH_BIT 7
#define NINTH_BIT 8
#define TENTH_BIT 9
#define ELEVENTH_BIT 10
#define TWELFTH_BIT 11
#define THIRTEENTH_BIT 12
#define FOURTEENTH_BIT 13
#define FIFTEENTH_BIT 14
#define SIXTEENTH_BIT 15

#define MAX_RECORD_LENGTH 256
#define INSTRUCTION_MEMORY_LENGTH (64 * KILOBYTE)
#define DATA_MEMORY_LENGTH (64 * KILOBYTE)

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
 * @brief Valid Record Types - Stored as Characters
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
 * The S-Record struct contains the necessary fields to represent an S-Record.
 */
typedef struct s_record_t
{
byte_t type;
byte_t length;
byte_t address[2];
byte_t data[MAX_RECORD_LENGTH];
}s_record_t;

/* Program Struct */
typedef struct program_t
{
    /* Memory Space */
byte_t instruction_memory[INSTRUCTION_MEMORY_LENGTH];
byte_t data_memory[DATA_MEMORY_LENGTH];

/* Register File */
word_t register_file[REGISTER_FILE_LENGTH];

/* Breakpoint Address */
int breakpoint;
int starting_address;

/* Executable Name */
byte_t executable_name[MAX_RECORD_LENGTH];

}program_t;
/**
 * @brief Enumeration representing different types of instructions.
 * 
 * This enumeration defines the various types of instructions that can be decoded.
 * Each instruction type is represented by a unique value.
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
 * This structure holds information about an instruction, including its type, and other parameters.
 */
typedef struct instruction
{
    instruction_type_t type;
    byte_t source;
    byte_t destination;
    byte_t byte;
    byte_t rc;
    byte_t wb;
} instruction_t;

#endif