/**
 * @file decode_instructions.c
 * @brief Decode instruction register into instruction structure
 * 
 * @author Zach Fraser
 * @date 2024-05-29
 */

#include "decode_instructions.h"

/**
 * @brief Array of MOV instructions.
 * 
 * This array represents the MOV instructions supported by the system.
 */
instruction_type_t mov_table[MOV_INSTRUCTION_COUNT] = 
{
    MOVL, 
    MOVLZ, 
    MOVLS, 
    MOVH
};

/**
 * @brief Table of arithmetic register instructions.
 * 
 * This table stores the arithmetic register instructions supported by the system.
 * The instructions are stored in the order they appear in the instruction set.
 */
instruction_type_t arithmetic_register_table[ARITHMETIC_REGISTER_INSTRUCTION_COUNT] = 
{
    ADD, 
    ADDC, 
    SUB, 
    SUBC, 
    DADD, 
    CMP,
    XOR,
    AND,
    OR,
    BIT,
    BIC,
    BIS,
};

/**
 * @brief Table of shift register instructions.
 * 
 * This table stores the shift register instructions supported by the system.
 */
instruction_type_t shift_register_table[SHIFT_REGISTER_INSTRUCTION_COUNT] = 
{
    SRA,
    RRC,
    UNDEFINED,
    SWPB,
    SXT,
    UNDEFINED,
    UNDEFINED,
    UNDEFINED
};

/**
 * @brief Read an instruction from memory stored in little endian format
 * 
 * @param instruction_word Reference to the instruction word to store the read instruction
 * @param instruction_memory Memory containing the instruction
 * @param program_counter Address of the instruction in memory
 * @return Exit Status [0 = success, <= 0 failure]
 */
int read_instruction(word_t *instruction_word, byte_t *instruction_memory, int program_counter)
{
    int error_status = 0;
    if(instruction_memory == NULL || instruction_word == NULL)
    {
        /* Invalid memory or instruction word pointer */
        error_status = -1;
    }

    /* Read instruction from memory in little endian format */
    *instruction_word = (instruction_memory[program_counter] | ((word_t)instruction_memory[++program_counter] << NINTH_BIT));
    return error_status;
}

/**
 * @brief Decode an instruction from a memory address
 * 
 * This function decodes an instruction from the instruction register.
 * The instruction is decoded into an instruction_t structure.
 * 
 * @param instruction Reference to the instruction structure to store the decoded instruction
 * @param instruction_register Instruction word to decode
 * @return Exit Status - [0 = success, <= 0 failure]
 */
int decode_instruction(instruction_t *instruction, 
    word_t instruction_register)
{
    
    if(instruction == NULL)
    {
        /* Invalid instruction pointer */
        return -1;
    }

    /* Clear Instruction Struct */
    memset(instruction, 0, sizeof(instruction_t));


    /* Decode First Three Bits */
    switch ((instruction_register >> FOURTEENTH_BIT) & THREE_BITS)
    {
    /* Move Instructions */
    case MOVE_CODE:
        /* Decode Opcode from next two bits */
        instruction->type = mov_table[(instruction_register >> TWELFTH_BIT) & TWO_BITS];
        /* Decode source byte from next 8 bits */
        instruction->source = (instruction_register >> FOURTH_BIT) & EIGHT_BITS;
        /* Decode destination register from last 3 bits */
        instruction->destination = (instruction_register >> FIRST_BIT) & THREE_BITS;
        break;

    case REGISTER_CODE:
        /* Read Next Five Bits */
        if(((instruction_register >> NINTH_BIT) & FIVE_BITS) < ARITHMETIC_REGISTER_CODE)
        {
            /* Arithmetic Register Instruction */
            instruction->type = arithmetic_register_table[(instruction_register >> NINTH_BIT) & FIVE_BITS];
            /* Register/Constant Select */
            instruction->rc = (instruction_register >> EIGHTH_BIT) & ONE_BIT;
            /* Word/Byte Select */
            instruction->wb = (instruction_register >> SEVENTH_BIT) & ONE_BIT;
            /* Source Register */
            instruction->source = (instruction_register >> FOURTH_BIT) & THREE_BITS;
            /* Destination Register */
            instruction->destination = (instruction_register >> FIRST_BIT) & THREE_BITS;
        }
        else if (((instruction_register >> NINTH_BIT) & FIVE_BITS) == SWAP_REGISTER_CODE)
        {
            /* MOV/Swap Instruction */
            if((instruction_register >> SEVENTH_BIT) & ONE_BIT)
            {
                /* Swap Instruction */
                instruction->type = SWAP;
            }
            else
            {
                /* Mov Instruction */
                instruction->type = MOV;
                /* Word/Byte Select */
                instruction->wb = (instruction_register >> SEVENTH_BIT) & ONE_BIT;
            }
            /* Source Register */
            instruction->source = (instruction_register >> FOURTH_BIT) & THREE_BITS;
            /* Destination Register */
            instruction->destination = (instruction_register >> FIRST_BIT) & THREE_BITS;
        }
        else if (((instruction_register >> NINTH_BIT) & FIVE_BITS) == SHIFT_REGISTER_CODE)
        {
            /* Shift Register Instruction */
            /* Word/Byte Select */
            instruction->wb = (instruction_register >> SEVENTH_BIT) & ONE_BIT;
            /* Decode Type */
            instruction->type = shift_register_table[(instruction_register >> FOURTH_BIT) & THREE_BITS];
            /* Destination Register */
            instruction->destination = (instruction_register >> FIRST_BIT) & THREE_BITS;
        }
        else
        {
            /* Instruction not implemented */
            instruction->type = UNDEFINED;
        }
        
        break;
    default:
    /* Instruction not implemented */
    instruction->type = UNDEFINED;
        break;
    }


    return 0;
}