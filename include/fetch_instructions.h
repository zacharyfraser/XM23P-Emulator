/**
 * @file fetch.h
 * @brief Implementation of fetch stage of XM23P pipeline
 * 
 * @author Zach Fraser
 * @date 2024-06-13
 */

#ifndef FETCH_H
#define FETCH_H

#include "definitions.h"

int fetch_instruction(program_t *program, int stage);

/* Available states for ICTRL and DCTRL registers */
typedef enum control_state {
    CONTROL_READ = 0,
    CONTROL_WRITE = 1
} control_state_t;

#endif /* FETCH_H */