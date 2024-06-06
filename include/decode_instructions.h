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

#include <stdio.h>
#include <string.h>

#include "definitions.h"


extern instruction_type_t mov_table[MOV_INSTRUCTION_COUNT];
extern instruction_type_t arithmetic_register_table[ARITHMETIC_REGISTER_INSTRUCTION_COUNT];
extern instruction_type_t shift_register_table[SHIFT_REGISTER_INSTRUCTION_COUNT];

int read_instruction(word_t *instruction, byte_t *memory, int program_counter);
int decode_instruction(instruction_t *instruction, word_t instruction_register);

#endif