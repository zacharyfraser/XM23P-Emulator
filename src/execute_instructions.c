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

int execute_instruction(instruction_t *instruction, program_t *program, int stage)
{
    if(instruction == NULL || program == NULL)
    {
        /* Invalid input pointers */
        return -1;
    }

    if(stage == E0)
    {
        /* Copy stage to program context for debug logging */
        if(program->debug_mode)
        {
            sprintf_s(program->instruction_execute, MAX_STAGE_LENGTH, "E0: %04x", instruction->opcode);
        }
        execute_table[instruction->type](instruction, program);
        /* Copy instruction to previous instruction */
        program->previous_instruction = *instruction;
    }
    else if(stage == E1)
    {
        if(program->previous_instruction.data_flag)
        {
            /* Perform Memory Access */
            switch(program->data_control_register)
            {
                case WRITE_BYTE:
                    program->data_memory[program->data_memory_address_register] = program->data_memory_buffer_register & 0xFF;
                    break;
                case WRITE_WORD:
                    program->data_memory[program->data_memory_address_register] = program->data_memory_buffer_register & 0xFF;
                    program->data_memory[program->data_memory_address_register + 1] = (program->data_memory_buffer_register >> 8) & 0xFF;
                    break;
                case READ_BYTE:
                    /* Read Byte from Data Memory to Data Memory Buffer */
                    program->data_memory_buffer_register = program->data_memory[program->data_memory_address_register];
                    /* Write result to destination register */
                    program->register_file[REGISTER][program->previous_instruction.destination] = program->data_memory_buffer_register;
                    break;
                case READ_WORD:
                    /* Read Word from Data Memory to Data Memory Buffer */
                    program->data_memory_buffer_register = program->data_memory[program->data_memory_address_register];
                    program->data_memory_buffer_register |= program->data_memory[program->data_memory_address_register + 1] << 8;
                    /* Write result to destination register */
                    program->register_file[REGISTER][program->previous_instruction.destination] = program->data_memory_buffer_register;
                    break;
                default:
                    break;
            }
            /* Copy stage to program context for debug logging */
            if(program->debug_mode)
            {
                sprintf_s(program->instruction_execute, MAX_STAGE_LENGTH, "E1: %04x", program->previous_instruction.opcode);
            }
        }
        /* Copy stage to program context for debug logging */
        else if(program->debug_mode)
        {
            sprintf_s(program->instruction_execute, MAX_STAGE_LENGTH, "\t");
        }
    }
    return 0;
}