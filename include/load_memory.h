/**
 * @file load_memory.h
 * @brief Load program instructions and data into memory - 
 *                                      Architecture agnostic
 * 
 * @author Zach Fraser
 * @date 2024-05-19
 */

#ifndef LOAD_MEMORY_H
#define LOAD_MEMORY_H

#include <stddef.h>

#include "definitions.h"

int copy_byte_array(
    byte_t *source, byte_t *destination, int length, int offset);
int load_record_address(s_record_t *s_record, int *destination);
int load_record_data(s_record_t *s_record, byte_t *destination);
int load_record_name(s_record_t *s_record, byte_t *destination);

#endif