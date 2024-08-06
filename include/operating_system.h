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
    LOAD            = 'l',
    MEMORY_DUMP     = 'm',
    MEMORY_WRITE    = 'w',
    REGISTER_DUMP   = 'r',
    DEBUG_TOGGLE    = 'd',
    REGISTER_SET    = 's',
    SET_BREAKPOINT  = 'b',
    RUN             = 'g',
    RESTART         = 'v',
    EXIT            = 'x',
    HELP            = 'h'
};

/* Function Prototypes */
void display_utilities(void);
void run_operating_system(program_t *program);

#endif