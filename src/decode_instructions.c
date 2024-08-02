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
 * @brief Reset instruction arguments to 0
 * 
 * @param instruction - Pointer to instruction struct
 * @return int - [0 = Success, -1 = Null Pointer]
 */
int reset_instruction_arguments(instruction_t *instruction)
{
    if(instruction == NULL)
    {
        return -1;
    }

    instruction->rc = 0;
    instruction->wb = 0;
    instruction->source = 0;
    instruction->destination = 0;

    return 0;
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
int decode_instruction(instruction_t *instruction, word_t instruction_register)
{
    
    if(instruction == NULL)
    {
        /* Invalid instruction pointer */
        return -1;
    }

    /* Clear Instruction Struct */
    reset_instruction_arguments(instruction);
    instruction->opcode = instruction_register;
    
    /* Decode Bits Thirteen through Fifteen */
    switch ((instruction_register >> 13) & THREE_BITS)
    {
    /* Move Instructions */
    case MOVE_CODE:
        /* Decode Opcode from bits Eleven and Twelve */
        instruction->type = mov_table[(instruction_register >> 11) & TWO_BITS];
        /* Decode source byte from bits Three through Ten */
        instruction->source = (instruction_register >> 3) & EIGHT_BITS;
        /* Decode destination register from bits Zero through Two */
        instruction->destination = (instruction_register >> 0) & THREE_BITS;
        break;

    case REGISTER_CODE:
        /* Read bits Eight through Twelve */
        if(((instruction_register >> 8) & FIVE_BITS) < ARITHMETIC_REGISTER_CODE)
        {
            /* Arithmetic Register Instruction - Bits Eight through Twelve */
            instruction->type = arithmetic_register_table[(instruction_register >> 8) & FIVE_BITS];
            /* Register/Constant Select - Bit Seven */
            instruction->rc = (instruction_register >> 7) & ONE_BIT;
            /* Word/Byte Select - Bit Six */
            instruction->wb = (instruction_register >> 6) & ONE_BIT;
            /* Source Register - Bits Three through Five*/
            instruction->source = (instruction_register >> 3) & THREE_BITS;
            /* Destination Register - Bits Zero through Two */
            instruction->destination = (instruction_register >> 0) & THREE_BITS;
        }
        /* Read Bits Eight through Twelve */
        else if (((instruction_register >> 8) & FIVE_BITS) == SWAP_REGISTER_CODE)
        {
            /* MOV/Swap Instruction - Bit Seven */
            if((instruction_register >> 7) & ONE_BIT)
            {
                /* Swap Instruction */
                instruction->type = SWAP;
            }
            else
            {
                /* Mov Instruction */
                instruction->type = MOV;
                /* Word/Byte Select - Bit Six*/
                instruction->wb = (instruction_register >> 6) & ONE_BIT;
            }
            /* Source Register - Bits Three through Five */
            instruction->source = (instruction_register >> 3) & THREE_BITS;
            /* Destination Register - Bits Zero through Three */
            instruction->destination = (instruction_register >> 0) & THREE_BITS;
        }
        /* Read Bits Eight through Twelve */
        else if (((instruction_register >> 8) & FIVE_BITS) == SHIFT_REGISTER_CODE)
        {
            /* Read Bit Seven */
            if(((instruction_register >> 7) & ONE_BIT))
            {
                /* CPU Instructions */
                /* Read bit 6*/
                if((instruction_register >> 6) & ONE_BIT)
                {
                    /* CLRCC */
                    instruction->type = CLRCC;
                    instruction->status.overflow = (instruction_register >> 4) & ONE_BIT;
                    instruction->status.sleep = (instruction_register >> 3) & ONE_BIT;
                    instruction->status.negative = (instruction_register >> 2) & ONE_BIT;
                    instruction->status.zero = (instruction_register >> 1) & ONE_BIT;
                    instruction->status.carry = (instruction_register >> 0) & ONE_BIT;
                }
                /* Read bit 5 */
                else if ((instruction_register >> 5) & ONE_BIT)
                {
                    /* SETCC */
                    instruction->type = SETCC;
                    instruction->status.overflow = (instruction_register >> 4) & ONE_BIT;
                    instruction->status.sleep = (instruction_register >> 3) & ONE_BIT;
                    instruction->status.negative = (instruction_register >> 2) & ONE_BIT;
                    instruction->status.zero = (instruction_register >> 1) & ONE_BIT;
                    instruction->status.carry = (instruction_register >> 0) & ONE_BIT;
                }
                /* Read bit 4 */
                else if ((instruction_register >> 4) & ONE_BIT)
                {
                    /* SVC */
                    instruction->type = SVC;
                }
                /* Read bit 3 */
                else if(((instruction_register >> 3) & ONE_BIT))
                {
                    /* Invalid Instruction */
                    instruction->type = UNDEFINED;
                }
                else
                {
                    /* SETPRI */
                    instruction->type = SETPRI;
                }
            }
            else
            {
                /* Shift Register Instruction */
                /* Word/Byte Select - Bit Six */
                instruction->wb = (instruction_register >> 6) & ONE_BIT;
                /* Decode Type */
                instruction->type = shift_register_table[(instruction_register >> 3) & THREE_BITS];
                /* Destination Register */
                instruction->destination = (instruction_register >> 0) & THREE_BITS;

            }
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