/**
 * @file decode_instructions.h
 * @brief Header file for decoding instructions
 * 
 * @author Zach Fraser
 * @date 2024-05-29
 */

#ifndef DECODE_INSTRUCTION_H
#define DECODE_INSTRUCTION_H

#define MOV_INSTRUCTION_COUNT 4
#define ARITHMETIC_REGISTER_INSTRUCTION_COUNT 12
#define SHIFT_REGISTER_INSTRUCTION_COUNT 8

#define REGISTER_CODE 0b010
#define MOVE_CODE 0b110
#define ARITHMETIC_REGISTER_CODE 0x0C
#define SWAP_REGISTER_CODE 0x0C
#define SHIFT_REGISTER_CODE 0x0D

#include "definitions.h"
#include "read_memory.h"

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
    UNDEFINED,
    SWPB,
    SXT,
    UNDEFINED,
    UNDEFINED,
    UNDEFINED
};

int decode_instruction(instruction_t *instruction, byte_t *memory, int program_counter);

#endif