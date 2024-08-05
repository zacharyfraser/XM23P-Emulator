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

instruction_type_t branch_table[BRANCH_INSTRUCTION_COUNT] = 
{
    BEQ, 
    BNE, 
    BC, 
    BNC, 
    BN, 
    BGE, 
    BLT, 
    BRA
};

/**
 * @brief Reset instruction arguments to 0
 * 
 * @param instruction - Pointer to instruction struct
 * @return int - [0 = Success, -1 = Null Pointer]
 */
int reset_instruction_arguments(instruction_t *instruction)
{
    /* Check for NULL Pointer */
    if(instruction == NULL)
    {
        return -1;
    }
    /* Copy Opcode and Address */
    word_t opcode = instruction->opcode;
    word_t address = instruction->address;
    
    /* Clear Instruction Struct */
    memset(instruction, 0, sizeof(instruction_t));
    /* Restore Opcode and Address */
    instruction->opcode = opcode;
    instruction->address = address;


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
int decode_instruction(instruction_t *instruction, program_t *program)
{    
    /* Check for NULL Pointer */
    if(instruction == NULL || program == NULL)
    {
        return -1;
    }

    word_t instruction_register = program->instruction_register;

    /* Clear Instruction Struct */
    reset_instruction_arguments(instruction);
    instruction->opcode = instruction_register;
    
    /* Decode Bits Thirteen through Fifteen */
    switch (READ_BITS(instruction_register, 13, 15))
    {
    /* Move Instructions */
    case MOVE_CODE:
        /* Decode Opcode from bits Eleven and Twelve */
        instruction->type = mov_table[READ_BITS(instruction_register, 11, 12)];
        /* Decode source byte from bits Three through Ten */
        instruction->source = READ_BITS(instruction_register, 3, 10);
        /* Decode destination register from bits Zero through Two */
        instruction->destination = READ_BITS(instruction_register, 0, 2);
        break;

    case REGISTER_CODE:
        /* Read bits Eight through Twelve */
        if(READ_BITS(instruction_register, 8, 12) < ARITHMETIC_REGISTER_CODE)
        {
            /* Arithmetic Register Instruction - Bits Eight through Twelve */
            instruction->type = arithmetic_register_table[READ_BITS(instruction_register, 8, 12)];
            /* Register/Constant Select - Bit Seven */
            instruction->rc = READ_BITS(instruction_register, 7, 7);
            /* Word/Byte Select - Bit Six */
            instruction->wb = READ_BITS(instruction_register, 6, 6);
            /* Source Register - Bits Three through Five*/
            instruction->source = READ_BITS(instruction_register, 3, 5);
            /* Destination Register - Bits Zero through Two */
            instruction->destination = READ_BITS(instruction_register, 0, 2);
        }
        /* Read Bits Eight through Twelve */
        else if (READ_BITS(instruction_register, 8, 12) == SWAP_REGISTER_CODE)
        {
            /* MOV/Swap Instruction - Bit Seven */
            if(READ_BITS(instruction_register, 7, 7))
            {
                /* Swap Instruction */
                instruction->type = SWAP;
            }
            else
            {
                /* Mov Instruction */
                instruction->type = MOV;
                /* Word/Byte Select - Bit Six*/
                instruction->wb = READ_BITS(instruction_register, 6, 6);
            }
            /* Source Register - Bits Three through Five */
            instruction->source = READ_BITS(instruction_register, 3, 5);
            /* Destination Register - Bits Zero through Three */
            instruction->destination = READ_BITS(instruction_register, 0, 3);
        }
        /* Read Bits Eight through Twelve */
        else if (READ_BITS(instruction_register, 8, 12) == SHIFT_REGISTER_CODE)
        {
            /* Read Bit Seven */
            if(READ_BITS(instruction_register, 7, 7) == 1)
            {
                /* CPU Instructions */
                /* Read bit 6*/
                if(READ_BITS(instruction_register, 6, 6) == 1)
                {
                    /* CLRCC */
                    instruction->type = CLRCC;
                    instruction->status.overflow = READ_BITS(instruction_register, 4, 4);
                    instruction->status.sleep = READ_BITS(instruction_register, 3, 3);
                    instruction->status.negative = READ_BITS(instruction_register, 2, 2);
                    instruction->status.zero = READ_BITS(instruction_register, 1, 1);
                    instruction->status.carry = READ_BITS(instruction_register, 0, 0);
                }
                /* Read bit 5 */
                else if (READ_BITS(instruction_register, 5, 5) == 1)
                {
                    /* SETCC */
                    instruction->type = SETCC;
                    instruction->status.overflow = READ_BITS(instruction_register, 4, 4);
                    instruction->status.sleep = READ_BITS(instruction_register, 3, 3);
                    instruction->status.negative = READ_BITS(instruction_register, 2, 2);
                    instruction->status.zero = READ_BITS(instruction_register, 1, 1);
                    instruction->status.carry = READ_BITS(instruction_register, 0, 0);
                }
                /* Read bit 4 */
                else if (READ_BITS(instruction_register, 4, 4) == 1)
                {
                    /* SVC */
                    instruction->type = SVC;
                    /* Service Address - Bits Zero through Three */
                    instruction->sa = READ_BITS(instruction_register, 0, 3);
                }
                /* Read bit 3 */
                else if(READ_BITS(instruction_register, 3, 3) == 1)
                {
                    /* Invalid Instruction */
                    instruction->type = UNDEFINED;
                }
                else
                {
                    /* SETPRI */
                    instruction->type = SETPRI;
                    /* Priority Level - Bits Zero through Three */
                }
            }
            else
            {
                /* Shift Register Instruction */
                /* Word/Byte Select - Bit Six */
                instruction->wb = READ_BITS(instruction_register, 6, 6);
                /* Decode Type from bits 3 through 5 */
                instruction->type = shift_register_table[READ_BITS(instruction_register, 3, 5)];
                /* Destination Register - Bits 0 through 2 */
                instruction->destination = READ_BITS(instruction_register, 0, 2);

            }
        }
        else if(READ_BITS(instruction_register, 12, 12) == 1)
        {
            switch(READ_BITS(instruction_register, 10, 11))
            {
                case CEX_CODE:
                    /* CEX */
                    instruction->type = CEX;
                    /* Decode Condition Code */
                    instruction->cc = READ_BITS(instruction_register, 6, 9);
                    /* Decode True Count */
                    instruction->t_count = READ_BITS(instruction_register, 3, 5);
                    /* Decode False Count */
                    instruction->f_count = READ_BITS(instruction_register, 0, 2);
                    break;
                case LD_CODE:
                    /* LD */
                    instruction->type = LD;
                    /* Decode Pre/Post Increment Flag */
                    instruction->prpo = READ_BITS(instruction_register, 9, 9);
                    /* Decode Decrement Flag */
                    instruction->decrement = READ_BITS(instruction_register, 8, 8);
                    /* Decode Increment Flag */
                    instruction->increment = READ_BITS(instruction_register, 7, 7);
                    /* Decode Word/Byte Flag */
                    instruction->wb = READ_BITS(instruction_register, 6, 6);
                    /* Decode Source Register */
                    instruction->source = READ_BITS(instruction_register, 3, 5);
                    /* Decode Destination Register */
                    instruction->destination = READ_BITS(instruction_register, 0, 2);
                    break;
                case ST_CODE:
                    /* ST */
                    instruction->type = ST;
                    /* Decode Pre/Post Increment Flag */
                    instruction->prpo = READ_BITS(instruction_register, 9, 9);
                    /* Decode Decrement Flag */
                    instruction->decrement = READ_BITS(instruction_register, 8, 8);
                    /* Decode Increment Flag */
                    instruction->increment = READ_BITS(instruction_register, 7, 7);
                    /* Decode Word/Byte Flag */
                    instruction->wb = READ_BITS(instruction_register, 6, 6);
                    /* Decode Source Register */
                    instruction->source = READ_BITS(instruction_register, 3, 5);
                    /* Decode Destination Register */
                    instruction->destination = READ_BITS(instruction_register, 0, 2);

                    /* Check if destination is PC, and insert bubble */
                    if(instruction->destination == PC)
                    {
                        program->bubble_flag = 1;
                    }
                    break;
                default:
                    /* Instruction not implemented */
                    instruction->type = UNDEFINED;
                    break;
            }
        }
        else
        {
            /* Instruction not implemented */
            instruction->type = UNDEFINED;
        }
        
        break;
    case BRANCH_CODE:
        /* Decode Type from bits 10 - 12 */
        instruction->type = branch_table[READ_BITS(instruction_register, 10, 12)];
        /* Decode Branch Offset from bits 9 - 0 */
        instruction->offset = READ_BITS(instruction_register, 0, 9);
        break;
    case LINK_CODE:
        instruction->type = BL;
        /* Decode Branch Offset from bits 0 - 12 */
        instruction->offset = READ_BITS(instruction_register, 0, 12);
        break;
    default:
        if(READ_BITS(instruction_register, 14, 15) == LOAD_RELATIVE_CODE)
        {
            /* Load Relative Instruction */
            instruction->type = LDR;
            /* Decode Offset from bits 7 - 13 */
            instruction->offset = READ_BITS(instruction_register, 7, 13);
            /* Decode Word/Byte Select from bit 6 */
            instruction->wb = READ_BITS(instruction_register, 6, 6);
            /* Decode Source Register from bits 3 - 5 */
            instruction->source = READ_BITS(instruction_register, 3, 5);
            /* Decode Destination Register from bits 0 - 3 */
            instruction->destination = READ_BITS(instruction_register, 0, 2);
        }
        else if(READ_BITS(instruction_register, 14, 15) == STORE_RELATIVE_CODE)
        {
            /* Store Relative Instruction */
            instruction->type = STR;
            /* Decode Offset from bits 7 - 13 */
            instruction->offset = READ_BITS(instruction_register, 7, 13);
            /* Decode Word/Byte Select from bit 6 */
            instruction->wb = READ_BITS(instruction_register, 6, 6);
            /* Decode Source Register from bits 3 - 5 */
            instruction->source = READ_BITS(instruction_register, 3, 5);
            /* Decode Destination Register from bits 0 - 2 */
            instruction->destination = READ_BITS(instruction_register, 0, 2);
            /* Check if destination is PC, and insert bubble */
            if(instruction->destination == PC)
            {
                program->bubble_flag = 1;
            }
        }
        else
        {
            /* Instruction not implemented */
            instruction->type = UNDEFINED;
        }
        break;
    }

    return 0;
}