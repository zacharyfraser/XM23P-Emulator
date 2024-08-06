/**
 * @file display_memory.h
 * @brief Utilities for displaying memory
 * 
 * @author Zach Fraser
 * @date 2024-05-22
 */

#ifndef DISPLAY_MEMORY_H
#define DISPLAY_MEMORY_H

#include <stdio.h>

#include "definitions.h"

#define BYTES_PER_LINE 16

/* Function Prototypes */
int convert_word_to_int(byte_t *word, int *integer);
int display_memory(byte_t *memory_array, int min_address, int max_address);

#endif
