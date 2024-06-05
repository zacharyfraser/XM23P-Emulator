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

#include "definitions.h"
#include "display_memory.h"

void memory_dump(byte_t *instruction_memory, byte_t *data_memory);
void memory_write(byte_t *instruction_memory, byte_t *data_memory);
void register_dump(byte_t *register_file);
void register_set(byte_t *register_file);
void set_breakpoint(int *breakpoint);

#endif