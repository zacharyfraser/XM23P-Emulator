/**
 * @file fetch.h
 * @brief Implementation of fetch stage of XM23P pipeline
 * 
 * @author Zach Fraser
 * @date 2024-06-13
 */

#ifndef FETCH_H
#define FETCH_H

#include <stdio.h>

#include "definitions.h"

int fetch_instruction(program_t *program, int stage);

#endif /* FETCH_H */