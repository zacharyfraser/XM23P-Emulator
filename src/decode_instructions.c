/**
 * @file decode_instructions.c
 * @brief Decode instructions from memory addresses
 * 
 * @author Zach Fraser
 * @date 2024-05-29
 */

#include "decode_instructions.h"

/**
 * @brief Decode an instruction from a memory address
 * 
 * This function decodes an instruction from a memory address.
 * The instruction is decoded into an instruction_t structure.
 * 
 * @param instruction Pointer to the instruction_t structure to store the decoded instruction
 * @param instruction_memory Pointer to the instruction memory
 * @param program_counter Address of the instruction in memory
 * @return Exit Status - [0 = success, <= 0 failure, 1 = end of program]
 */
int decode_instruction(instruction_t *instruction, 
    byte_t *instruction_memory, int program_counter)
{
    word_t instruction_word;
    word_t temp_word; /* Temporary word to store decoded bits */
    int error_status = 0;
    
    /* Check if address is within instruction memory */
    if (program_counter >= INSTRUCTION_MEMORY_LENGTH)
    {
        /* Address out of bounds */
        return -1;
    }

    /* Read instruction from memory */ 
    error_status = read_instruction(&instruction_word, instruction_memory, program_counter);
    if(error_status != 0)
    {
        /* Error reading instruction */
        return -2;
    }

    if(instruction_word == 0x0000)
    {
        /* End of Program */
        return 1;
    }
    else
    {
        /* Decode First Three Bits */
        temp_word = instruction_word;
        switch ((temp_word >> FOURTEENTH_BIT) & THREE_BITS)
        {
        /* Move Instructions */
        case MOVE_CODE:
            /* Decode Opcode from next two bits */
            instruction->type = mov_table[(temp_word >> TWELFTH_BIT) & TWO_BITS];
            /* Decode source byte from next 8 bits */
            instruction->source = (temp_word >> FOURTH_BIT) & EIGHT_BITS;
            /* Decode destination register from last 3 bits */
            instruction->destination = (temp_word >> FIRST_BIT) & THREE_BITS;
            break;

        case REGISTER_CODE:
            /* Read Next Five Bits */
            if(((temp_word >> NINTH_BIT) & FIVE_BITS) < ARITHMETIC_REGISTER_CODE)
            {
                /* Arithmetic Register Instruction */
                instruction->type = arithmetic_register_table[(temp_word >> NINTH_BIT) & FIVE_BITS];
                /* Register/Constant Select */
                instruction->rc = (temp_word >> EIGHTH_BIT) & ONE_BIT;
                /* Word/Byte Select */
                instruction->wb = (temp_word >> SEVENTH_BIT) & ONE_BIT;
                /* Source Register */
                instruction->source = (temp_word >> FOURTH_BIT) & THREE_BITS;
                /* Destination Register */
                instruction->destination = (temp_word >> FIRST_BIT) & THREE_BITS;
            }
            else if (((temp_word >> NINTH_BIT) & FIVE_BITS) == SWAP_REGISTER_CODE)
            {
                /* MOV/Swap Instruction */
                if((temp_word >> SEVENTH_BIT) & ONE_BIT)
                {
                    /* Swap Instruction */
                    instruction->type = SWAP;
                }
                else
                {
                    /* Mov Instruction */
                    instruction->type = MOV;
                    /* Word/Byte Select */
                    instruction->wb = (temp_word >> SEVENTH_BIT) & ONE_BIT;
                }
                /* Source Register */
                instruction->source = (temp_word >> FOURTH_BIT) & THREE_BITS;
                /* Destination Register */
                instruction->destination = (temp_word >> FIRST_BIT) & THREE_BITS;
            }
            else if (((temp_word >> NINTH_BIT) & FIVE_BITS) == SHIFT_REGISTER_CODE)
            {
                /* Shift Register Instruction */
                /* Word/Byte Select */
                instruction->wb = (temp_word >> SEVENTH_BIT) & ONE_BIT;
                /* Decode Type */
                instruction->type = shift_register_table[(temp_word >> FOURTH_BIT) & THREE_BITS];
                /* Destination Register */
                instruction->destination = (temp_word >> FIRST_BIT) & THREE_BITS;
                /* Check if instruction is undefined */
                if(instruction->type == UNDEFINED)
                {
                    /* Instruction not implemented
                    * Print instruction word and Address */
                    print_word(instruction_word);
                    print_word(program_counter);
                }
            }
            else
            {
                /* Instruction not implemented
                * Print instruction word and Address */
                print_word(instruction_word);
                print_word(program_counter);
            }
            
            break;
        default:
        /* Instruction not implemented
        * Print instruction word and Address */
            print_word(instruction_word);
            print_word(program_counter);
            break;
        }
    }
    

    return 0;
}