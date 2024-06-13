/**
 * @file fetch.c
 * @brief Implementation of fetch stage of XM23P pipeline
 * 
 * @author Zach Fraser
 * @date 2024-06-13
 */

#include "fetch.h"

int fetch_instruction(program_t *program, int stage)
{
    switch (stage)
    {
    case 0:
        /* Set IMAR to PC */
        program->instruction_memory_address_register = program->register_file[PROGRAM_COUNTER];
        /* Set Instruction Program Counter for Printing */
        program->instruction.program_counter = program->register_file[PROGRAM_COUNTER] - 2;
        /* Set ICTRL to Read */
        program->instruction_control_register = CONTROL_READ;
        /* Increment PC */
        program->register_file[PROGRAM_COUNTER] += 2;
        break;
    case 1:
        /* IMBR = IMEM[IMAR}] */
        program->instruction_memory_buffer_register = program->instruction_memory[program->instruction_memory_address_register];
        program->instruction_memory_buffer_register |= program->instruction_memory[program->instruction_memory_address_register + 1] << 8;
        /* IR = IMBR */
        program->instruction_register = program->instruction_memory_buffer_register;
        break;
    }
    return 0;
}