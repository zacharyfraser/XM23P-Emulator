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

execute_instruction_t execute_table[NUM_OF_INSTRUCTIONS] = 
{
    execute_undefined, execute_bl, execute_beq, execute_bne,
    execute_bc, execute_bnc, execute_bn, execute_bge,
    execute_blt, execute_bra, execute_add, execute_addc,
    execute_sub, execute_subc, execute_dadd, execute_cmp,
    execute_xor, execute_and, execute_or, execute_bit,
    execute_bic, execute_bis, execute_mov, execute_swap,
    execute_sra, execute_rrc, execute_swpb, execute_sxt,
    execute_setpri, execute_svc, execute_setcc, execute_clrcc,
    execute_cex, execute_ld, execute_st, execute_movl,
    execute_movlz, execute_movls, execute_movh, execute_ldr,
    execute_str
};

int execute_instruction(instruction_t *instruction, program_t *program)
{
    if(instruction == NULL || program == NULL)
    {
        /* Invalid input pointers */
        return -1;
    }
    execute_table[instruction->type](instruction, program);

    return 0;
}