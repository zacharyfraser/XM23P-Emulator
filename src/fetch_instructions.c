/**
 * @file fetch.c
 * @brief Implementation of fetch stage of XM23P pipeline
 * 
 * @author Zach Fraser
 * @date 2024-06-13
 */

#include "fetch_instructions.h"

/**
 * @brief Pipelined fetch stage
 * 
 * @param program Program context
 * @param stage Fetch stage selection
 * @return int [0 = SUCCESS, < 0 = FAILURE]
 */
int fetch_instruction(program_t *program, int stage)
{
    /* F0 Stage */
    if(stage == F0)
    {
        /* Set IMAR to PC */
        program->instruction_memory_address_register = program->PROGRAM_COUNTER;
        /* Set ICTRL to Read */
        program->instruction_control_register = READ_WORD;
        /* Increment PC */
        program->PROGRAM_COUNTER += 2;
        /* Copy stage to program context for debug logging */
        if(program->debug_mode)
        {
            sprintf_s(program->instruction_fetch, MAX_STAGE_LENGTH, "F0: %04x", program->instruction_memory_address_register);
        }
    }
    /* F1 Stage */
    else if(stage == F1)
    {
        /* IMBR = IMEM[IMAR}] */
        program->instruction_memory_buffer_register = program->instruction_memory[program->instruction_memory_address_register];
        program->instruction_memory_buffer_register |= program->instruction_memory[program->instruction_memory_address_register + 1] << 8;
        /* IR = IMBR */
        program->instruction_register = program->instruction_memory_buffer_register;
        /* Insert bubble if bubble flag raised */
        if(program->bubble_flag)
        {
            /* Replace next instruction with NOOP */
            printf("Bubblin'...\n");
            program->instruction_register = INSTRUCTION_NOOP;
            program->bubble_flag = 0;
        }
        /* Copy stage to program context for debug logging */
        if(program->debug_mode)
        {
            sprintf_s(program->instruction_fetch, MAX_STAGE_LENGTH, "F1: %04x", program->instruction_register);
        }
    }
    return 0;
}