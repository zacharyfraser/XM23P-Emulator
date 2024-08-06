/**
 * @file utilities.h
 * @brief Header file for utilities available to the user
 * 
 * @author Zach Fraser
 * @date 2024-06-05
 */

#ifndef UTILITIES_H
#define UTILITIES_H

#include <stdio.h>
#include <string.h>

#include "definitions.h"
#include "display_memory.h"
#include "load_memory.h"
#include "decode_instructions.h"
#include "execute_instructions.h"
#include "fetch_instructions.h"

/* Function Prototypes */
void load_memory(program_t *program, char *supplied_path);
void memory_dump(byte_t *instruction_memory, byte_t *data_memory);
void memory_write(byte_t *instruction_memory, byte_t *data_memory);
void register_dump(word_t *register_file);
void register_set(word_t *register_file);
void set_breakpoint(int *breakpoint);
void run(program_t *program);
void restart_program(program_t *program);

#endif