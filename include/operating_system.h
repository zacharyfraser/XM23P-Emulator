/**
 * @file operating_system.h
 * @brief Header file for implementation of utilities operating system
 * 
 * @author Zach Fraser
 * @date 2024-06-05
 */

#ifndef OPERATING_SYSTEM_H
#define OPERATING_SYSTEM_H

#include <stdio.h>
#include "utilities.h"
/**
 * @brief Enum for Available Utilities
 */
enum utilities
{
    MEMORY_DUMP     = 'm',
    MEMORY_WRITE    = 'w',
    REGISTER_DUMP   = 'r',
    REGISTER_SET    = 's',
    SET_BREAKPOINT  = 'b',
    EXIT            = 'x'
};

void display_utilities(void);
void run_operating_system(byte_t *instruction_memory, byte_t *data_memory, byte_t *register_file, int *breakpoints);

#endif


