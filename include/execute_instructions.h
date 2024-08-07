/**
 * @file execute_instructions.h
 * @brief Header file for executing instructions
 * 
 * @author Zach Fraser
 * @date 2024-06-06
 */

#ifndef EXECUTE_INSTRUCTIONS_H
#define EXECUTE_INSTRUCTIONS_H

#include <stdio.h>
#include <string.h>

#include "definitions.h"
#include "instruction_functions.h"

/* Function Pointer Type for Instruction Execution */
typedef int (*execute_instruction_t)(instruction_t *instruction, program_t *program);

/* List of instruction names as strings */
extern char *instruction_names[NUM_OF_INSTRUCTIONS];

/* Function Prototypes */
int execute_instruction(instruction_t *instruction, program_t *program, int stage);

#endif


