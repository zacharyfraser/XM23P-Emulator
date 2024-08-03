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
#define BRANCH_INSTRUCTION_COUNT 8

#define REGISTER_CODE 0b010
#define MOVE_CODE 0b011
#define BRANCH_CODE 0b001
#define LINK_CODE 0b000
#define LOAD_RELATIVE_CODE 0b10
#define STORE_RELATIVE_CODE 0b11
#define ARITHMETIC_REGISTER_CODE 0x0C
#define SWAP_REGISTER_CODE 0x0C
#define SHIFT_REGISTER_CODE 0x0D

#define CEX_CODE 0b00
#define LD_CODE 0b10
#define ST_CODE 0b11

#define READ_BITS(value, start, end) ((value >> start) & ((1 << (end - start + 1)) - 1))

#include <stdio.h>
#include <string.h>

#include "definitions.h"

/* Instruction Lookup Tables */
extern instruction_type_t mov_table[MOV_INSTRUCTION_COUNT];
extern instruction_type_t arithmetic_register_table[ARITHMETIC_REGISTER_INSTRUCTION_COUNT];
extern instruction_type_t shift_register_table[SHIFT_REGISTER_INSTRUCTION_COUNT];

int reset_instruction_arguments(instruction_t *instruction);
int decode_instruction(instruction_t *instruction, word_t instruction_register);

#endif