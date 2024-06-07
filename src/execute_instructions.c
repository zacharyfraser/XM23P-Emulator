/**
 * @file execute_instructions.c
 * @brief Module for executing instructions
 * 
 * @author Zach Fraser
 * @date 2024-06-06
 */

#include "execute_instructions.h"

char *instruction_names[NUM_OF_INSTRUCTIONS] = 
{
    "UNDEFINED", "BL", "BEQ", "BNE",
    "BC", "BNC", "BN", "BGE",
    "BLT", "BRA", "ADD", "ADDC",
    "SUB", "SUBC", "DADD", "CMP",
    "XOR", "AND", "OR", "BIT",
    "BIC", "BIS", "MOV", "SWAP",
    "SRA", "RRC", "SWPB", "SXT",
    "SETPRI", "SVC", "SETCC", "CLRCC",
    "CEX", "LD", "ST", "MOVL",
    "MOVLZ", "MOVLS", "MOVH", "LDR",
    "STR"
};

int execute_instruction(instruction_t *instruction, program_t *program)
{
    if(instruction == NULL || program == NULL)
    {
        /* Invalid input pointers */
        return -1;
    }

    if(instruction->type == UNDEFINED)
    {
        /* Instruction Not Implemented - Print Address and Instruction Register */
        printf("%04x: %04x\n", program->register_file[PROGRAM_COUNTER], program->instruction_register);
    }
    else
    {
        printf("%04x: %04x - %-5s RC: %01x WB: %01x Source: %02x Destination: %02x\n", 
            program->register_file[PROGRAM_COUNTER], 
            program->instruction_register,
            instruction_names[instruction->type], 
            instruction->rc, 
            instruction->wb, 
            instruction->source, 
            instruction->destination);
    }

    return 0;
    
}