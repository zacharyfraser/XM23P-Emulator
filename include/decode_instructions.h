/**
 * @file decode_instructions.h
 * @brief Header file for decoding instructions
 * 
 * @author Zach Fraser
 * @date 2024-05-29
 */

#define MOV_INSTRUCTION_COUNT 4
#define ARITHMETIC_REGISTER_INSTRUCTION_COUNT 12
#define SHIFT_REGISTER_INSTRUCTION_COUNT 4

#define REGISTER_CODE 0b010
#define MOVE_CODE 0b110

#include "definitions.h"

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
 * @brief Array of MOV instructions.
 * 
 * This array represents the MOV instructions supported by the system.
 */
instruction_type_t mov_table[MOV_INSTRUCTION_COUNT] = 
{
    MOVL, 
    MOVLZ, 
    MOVLS, 
    MOVH
};

/**
 * @brief Table of arithmetic register instructions.
 * 
 * This table stores the arithmetic register instructions supported by the system.
 * The instructions are stored in the order they appear in the instruction set.
 */
instruction_type_t arithmetic_register_table[ARITHMETIC_REGISTER_INSTRUCTION_COUNT] = 
{
    ADD, 
    ADDC, 
    SUB, 
    SUBC, 
    DADD, 
    CMP,
    XOR,
    AND,
    OR,
    BIT,
    BIC,
    BIS,
};

/**
 * @brief Table of shift register instructions.
 * 
 * This table stores the shift register instructions supported by the system.
 */
instruction_type_t shift_register_table[SHIFT_REGISTER_INSTRUCTION_COUNT] = 
{
    SRA,
    RRC,
    SWPB,
    SXT
};

int decode_instruction(instruction_t *instruction, byte_t *memory, int program_counter);