/**
 * @file instruction_functions.c
 * @brief Function definitions for each instruction in XM23P Architecture
 * 
 * @author Zach Fraser
 * @date 2024-06-17
 */

#include "instruction_functions.h"

/**
 * @brief Lookup table for Memory Control Settings
 * 
 */
control_state_t control_table[READ_WRITE][WORD_BYTE] =
{
    {READ_WORD,     READ_BYTE},
    {WRITE_WORD,    WRITE_BYTE}
};

/**
 * @brief Decode Condition Code and Return Result
 * @param condition_code Condition Code
 * @param program_status_word Program Status Word
 * @return int [0|1]
 */
int check_condition(condition_code_t condition_code, status_register_t program_status_word)
{
    switch(condition_code)
    {
        /* Z = 1 */
        case EQUAL:
            return program_status_word.zero;
        /* Z = 0 */
        case NOT_EQUAL:
            return !program_status_word.zero;
        /* C = 1 */
        case CARRY:
            return program_status_word.carry;
        /* C = 0 */
        case NOT_CARRY:
            return !program_status_word.carry;
        /* N = 1 */
        case NEGATIVE:
            return program_status_word.negative;
        /* N = 0 */
        case NOT_NEGATIVE:
            return !program_status_word.negative;
        /* V = 1 */
        case OVERFLOW:
            return program_status_word.overflow;
        /* V = 0 */
        case NOT_OVERFLOW:
            return !program_status_word.overflow;
        /* C = 1 & Z = 0 */
        case UNSIGNED_GREATER:
            return !program_status_word.zero && program_status_word.carry;
        /* C = 0 | Z = 1 */
        case UNSIGNED_LESS_EQUAL:
            return program_status_word.zero || !program_status_word.carry;
        /* N == V */
        case SIGNED_GREATER_EQUAL:
            return (program_status_word.negative == program_status_word.overflow);
        /* N != V*/
        case SIGNED_LESS:
            return (program_status_word.negative != program_status_word.overflow);
        /* (N == V) & Z = 0 */
        case SIGNED_GREATER:
            return !program_status_word.zero && (program_status_word.negative == program_status_word.overflow);
        /* (N != V) | Z = 1 */
        case SIGNED_LESS_EQUAL:
            return program_status_word.zero || (program_status_word.negative != program_status_word.overflow);
        /* Always True */
        case TRUE_ALWAYS:
            return 1;
        /* Always False */
        case FALSE_ALWAYS:
            return 0;
        default:
            return -1;
    }
}
 
 /**
  * @brief Restore Branch Offset
  * @param offset Encoded offset
  * @param number_of_bits Number of bits in offset
  * @return signed short Decoded offset
  */
signed short restore_offset(word_t offset, int number_of_bits)
{
    signed short result = offset;
    /* Restore LSb */
    result <<= 1;
    if(number_of_bits == LINK_OFFSET_LENGTH)
    {
        /* Test MSb */
        if (result & BIT_13)
        {
            /* Sign Extend to upper bits */
            result |= (TWO_BITS << 14);
        }
    }
    else if(number_of_bits == BRANCH_OFFSET_LENGTH)
    {
        if (result & BIT_10)
        {
            /* If the sign bit is set, extend the sign to the upper bits */
            result |= (FIVE_BITS << 11);
        }
    }

    return result;
}

/**
 * @brief Branch Instruction
 * @param program Program Context
 * @param offset Offset to branch
 */
void branch(program_t *program, signed short offset)
{
    word_t effective_address = program->PROGRAM_COUNTER - WORD_LENGTH;
    /* Calculate Effective Address */
    effective_address += offset;
#ifdef DEBUG
    printf("Branching + %d from %04x to %04x\n", offset, program->PROGRAM_COUNTER - 2 * WORD_LENGTH, effective_address);
#endif
    if(offset != 0x0000)
    {
        /* Set PC to Effective Address */
        program->PROGRAM_COUNTER = effective_address;
        /* Clear Bubble Queue */
        clear_bubble_queue(&program->bubble_queue);
        /* Insert Bubble */
        insert_bubble(&program->bubble_queue, 1);
    }
}

/**
 * @brief Test instruction for arithmetic overflow
 * 
 * @param source Instruction source word
 * @param destination Instruction destination word
 * @param result Instruction result word
 * @param wb Word or Byte operation
 * @return byte_t 
 */
byte_t test_overflow(word_t source, word_t destination, word_t result, int wb)
{
    if(wb == 0) /* Word Operation */
    {
        return source >> 15 && destination >> 15 && !(result >> 15) || !(source >> 15) && !(destination >> 15) && result >> 15;
    }
    else if(wb == 1) /* Byte Operation */
    {
        return source >> 7 && destination >> 7 && !(result >> 7) || !(source >> 7) && !(destination >> 7) && result >> 7;
    }
    return 2;
}

/**
 * @brief Undefined Instruction Handling
 * 
 * @param instruction 
 * @param program 
 * @return int [0 = success, <= 0 failure]
 */
int execute_undefined(instruction_t *instruction, program_t *program)
{
    instruction;
    program;
#ifdef DEBUG
    printf("%04x:\t%04x - Undefined Instruction\n", instruction->address, instruction->opcode);
#endif
    return -1;
}

/**
 * @brief Branch with Link Instruction
 * 
 * @param instruction 
 * @param program 
 * @return int [0 = success, <= 0 failure]
 */
int execute_bl(instruction_t *instruction, program_t *program)
{
    if(instruction->offset != 0x0000)
    {
    /* Set Link Register to First Instruction after Branch */
    program->LINK_REGISTER = program->PROGRAM_COUNTER - WORD_LENGTH;
    }
    /* Branch to Offset */
    branch(program, restore_offset(instruction->offset, LINK_OFFSET_LENGTH));

    return 0;
}

/**
 * @brief Branch if Equal Instruction
 * 
 * @param instruction 
 * @param program 
 * @return int [0 = Branch Taken, 1 = Branch not taken, <= 0 failure]
 */
int execute_beq(instruction_t *instruction, program_t *program)
{
    /* If condition not met, return 1 */
    if(!check_condition(EQUAL, program->program_status_word))
    {
        return 1;
    }

    /* Branch to Offset */
    branch(program, restore_offset(instruction->offset, BRANCH_OFFSET_LENGTH));

    return 0;
}

/**
 * @brief Branch if Not Equal Instruction
 * 
 * @param instruction 
 * @param program 
 * @return int [0 = success, <= 0 failure]
 */
int execute_bne(instruction_t *instruction, program_t *program)
{
    /* If condition not met, return 1 */
    if(!check_condition(NOT_EQUAL, program->program_status_word))
    {
        return 1;
    }

    /* Branch to Offset */
    branch(program, restore_offset(instruction->offset, BRANCH_OFFSET_LENGTH));

    return 0;
}

/**
 * @brief Branch if Carry Instruction
 * 
 * @param instruction 
 * @param program 
 * @return int [0 = success, <= 0 failure]
 */
int execute_bc(instruction_t *instruction, program_t *program)
{
    /* If condition not met, return 1 */
    if(!check_condition(CARRY, program->program_status_word))
    {
        return 1;
    }

    /* Branch to Offset */
    branch(program, restore_offset(instruction->offset, BRANCH_OFFSET_LENGTH));

    return 0;
}

/**
 * @brief Branch if Not Carry Instruction
 * 
 * @param instruction 
 * @param program 
 * @return int [0 = success, <= 0 failure]
 */
int execute_bnc(instruction_t *instruction, program_t *program)
{
    /* If condition not met, return 1 */
    if(!check_condition(NOT_CARRY, program->program_status_word))
    {
        return 1;
    }

    /* Branch to Offset */
    branch(program, restore_offset(instruction->offset, BRANCH_OFFSET_LENGTH));

    return 0;
}

/**
 * @brief Branch if Negative Instruction
 * 
 * @param instruction 
 * @param program 
 * @return int [0 = success, <= 0 failure]
 */
int execute_bn(instruction_t *instruction, program_t *program)
{
    /* If condition not met, return 1 */
    if(!check_condition(NEGATIVE, program->program_status_word))
    {
        return 1;
    }

    /* Branch to Offset */
    branch(program, restore_offset(instruction->offset, BRANCH_OFFSET_LENGTH));

    return 0;
}

/**
 * @brief Branch if Greater or Equal Instruction
 * 
 * @param instruction 
 * @param program 
 * @return int [0 = success, <= 0 failure]
 */
int execute_bge(instruction_t *instruction, program_t *program)
{
    /* If condition not met, return 1 */
    if(!check_condition(SIGNED_GREATER_EQUAL, program->program_status_word))
    {
        return 1;
    }

    /* Branch to Offset */
    branch(program, restore_offset(instruction->offset, BRANCH_OFFSET_LENGTH));

    return 0;
}

/**
 * @brief Branch if Less Than Instruction
 * 
 * @param instruction 
 * @param program 
 * @return int [0 = success, <= 0 failure]
 */
int execute_blt(instruction_t *instruction, program_t *program)
{
    /* If condition not met, return 1 */
    if(!check_condition(SIGNED_LESS, program->program_status_word))
    {
        return 1;
    }
    
    /* Branch to Offset */
    branch(program, restore_offset(instruction->offset, BRANCH_OFFSET_LENGTH));

    return 0;
}

/**
 * @brief Branch Always Instruction
 * 
 * @param instruction 
 * @param program 
 * @return int [0 = success, <= 0 failure]
 */
int execute_bra(instruction_t *instruction, program_t *program)
{
    /* Branch to Offset */
    branch(program, restore_offset(instruction->offset, BRANCH_OFFSET_LENGTH));

    return 0;
}

/**
 * @brief Addition Instruction
 * 
 * @param instruction 
 * @param program 
 * @return int [0 = success, <= 0 failure]
 */
int execute_add(instruction_t *instruction, program_t *program)
{
    word_t source = program->register_file[instruction->rc][instruction->source];
    word_t destination = program->register_file[REGISTER][instruction->destination];
    int result;

    if(instruction->wb == 0) /* Word Operation */
    {
        result = source + destination;
         program->register_file[REGISTER][instruction->destination] = (word_t)result;
        
        /* Test for PSW Flags */
        program->program_status_word.carry = (result > 0xFFFF); /* Test Result exceeds word */
        program->program_status_word.zero = ((short)result == 0); /* Test Low Word for Zero */
        program->program_status_word.negative = (word_t)((result >> 15) & 0x01); /* Test MSb */
        program->program_status_word.overflow = test_overflow(source, destination, (word_t)result, instruction->wb); /* Test for incorrectly flipped sign */
    }
    else if(instruction->wb == 1) /* Byte Operation */
    {
        source &= 0x00FF;
        destination &= 0x00FF;
        result = source + destination;

        /* Clear Low Byte of Destination */
        program->register_file[REGISTER][instruction->destination] &= 0xFF00;
        /* Set Low Byte of Destination */
        program->register_file[REGISTER][instruction->destination] |= (word_t)(result & EIGHT_BITS);

        /* Test for PSW Flags */
        program->program_status_word.carry = (result > 0xFF); /* Test Result exceeds byte */
        program->program_status_word.zero = ((byte_t)result == 0); /* Test Low Byte for Zero */
        program->program_status_word.negative = (word_t)((result >> 7) & 0x01); /* Test MSb */
        program->program_status_word.overflow = test_overflow(source, destination, (word_t)result, instruction->wb); /* Test for incorrectly flipped sign */
    }

    return 0;
}

/**
 * @brief Addition Instruction with Carry
 * 
 * @param instruction 
 * @param program 
 * @return int [0 = success, <= 0 failure]
 */
int execute_addc(instruction_t *instruction, program_t *program)
{
    word_t source = program->register_file[instruction->rc][instruction->source];
    word_t destination = program->register_file[REGISTER][instruction->destination];
    int result;

    if(instruction->wb == 0) /* Word Operation */
    {
        result = source + destination + program->program_status_word.carry;
         program->register_file[REGISTER][instruction->destination] = (word_t)result;
        
        /* Test for PSW Flags */
        program->program_status_word.carry = (result > 0xFFFF); /* Test Result exceeds word */
        program->program_status_word.zero = ((short)result == 0); /* Test Low Word for Zero */
        program->program_status_word.negative = (word_t)((result >> 15) & 0x01); /* Test MSb */
        program->program_status_word.overflow = test_overflow(source, destination, (word_t)result, instruction->wb); /* Test for incorrectly flipped sign */
    }
    else if(instruction->wb == 1) /* Byte Operation */
    {
        source &= 0x00FF;
        destination &= 0x00FF;
        result = source + destination + program->program_status_word.carry;
        
        /* Clear Low Byte of Destination */
        program->register_file[REGISTER][instruction->destination] &= 0xFF00;
        /* Set Low Byte of Destination */
        program->register_file[REGISTER][instruction->destination] |= (word_t)(result & EIGHT_BITS);

        /* Test for PSW Flags */
        program->program_status_word.carry = (result > 0xFF); /* Test Result exceeds byte */
        program->program_status_word.zero = ((byte_t)result == 0); /* Test Low Byte for Zero */
        program->program_status_word.negative = (word_t)((result >> 7) & 0x01); /* Test MSb */
        program->program_status_word.overflow = test_overflow(source, destination, (word_t)result, instruction->wb); /* Test for incorrectly flipped sign */
    }

    return 0;
}

/**
 * @brief Subtraction Instruction
 * 
 * @param instruction 
 * @param program 
 * @return int [0 = success, <= 0 failure]
 */
int execute_sub(instruction_t *instruction, program_t *program)
{
    word_t source = program->register_file[instruction->rc][instruction->source];
    /* 2's Compliment Source */
    source = ~source + 1;
    word_t destination = program->register_file[REGISTER][instruction->destination];
    int result;

    if(instruction->wb == 0) /* Word Operation */
    {
        result = source + destination;
         program->register_file[REGISTER][instruction->destination] = (word_t)result;
        
        /* Test for PSW Flags */
        program->program_status_word.carry = (result > 0xFFFF); /* Test Result exceeds word */
        program->program_status_word.zero = ((short)result == 0); /* Test Low Word for Zero */
        program->program_status_word.negative = (word_t)((result >> 15) & 0x01); /* Test MSb */
        program->program_status_word.overflow = test_overflow(source, destination, (word_t)result, instruction->wb); /* Test for incorrectly flipped sign */
    }
    else if(instruction->wb == 1) /* Byte Operation */
    {
        source &= 0x00FF;
        destination &= 0x00FF;
        result = source + destination;

        /* Clear Low Byte of Destination */
        program->register_file[REGISTER][instruction->destination] &= 0xFF00;
        /* Set Low Byte of Destination */
        program->register_file[REGISTER][instruction->destination] |= (word_t)(result & EIGHT_BITS);

        /* Test for PSW Flags */
        program->program_status_word.carry = (result > 0xFF); /* Test Result exceeds byte */
        program->program_status_word.zero = ((byte_t)result == 0); /* Test Low Byte for Zero */
        program->program_status_word.negative = (word_t)((result >> 7) & 0x01); /* Test MSb */
        program->program_status_word.overflow = test_overflow(source, destination, (word_t)result, instruction->wb); /* Test for incorrectly flipped sign */
    }

    return 0;
}

/**
 * @brief Subtraction Instruction with Carry
 * 
 * @param instruction 
 * @param program 
 * @return int [0 = success, <= 0 failure]
 */
int execute_subc(instruction_t *instruction, program_t *program)
{
    word_t source = program->register_file[instruction->rc][instruction->source];
    /* 1's Compliment Source */
    source = ~source;
    word_t destination = program->register_file[REGISTER][instruction->destination];
    word_t carry = program->program_status_word.carry;
    int result;

    if(instruction->wb == 0) /* Word Operation */
    {
        result = source + destination + carry;
        program->register_file[REGISTER][instruction->destination] = (word_t)result;
        
        /* Test for PSW Flags */
        program->program_status_word.carry = (result > 0xFFFF); /* Test Result exceeds word */
        program->program_status_word.zero = ((short)result == 0); /* Test Low Word for Zero */
        program->program_status_word.negative = (word_t)((result >> 15) & 0x01); /* Test MSb */
        program->program_status_word.overflow = test_overflow(source, destination, (word_t)result, instruction->wb); /* Test for incorrectly flipped sign */
    }
    else if(instruction->wb == 1) /* Byte Operation */
    {
        source &= 0x00FF;
        destination &= 0x00FF;
        result = source + destination + carry;

        /* Clear Low Byte of Destination */
        program->register_file[REGISTER][instruction->destination] &= 0xFF00;
        /* Set Low Byte of Destination */
        program->register_file[REGISTER][instruction->destination] |= (word_t)(result & EIGHT_BITS);

        /* Test for PSW Flags */
        program->program_status_word.carry = (result > 0xFF); /* Test Result exceeds byte */
        program->program_status_word.zero = ((byte_t)result == 0); /* Test Low Byte for Zero */
        program->program_status_word.negative = (word_t)((result >> 7) & 0x01); /* Test MSb */
        program->program_status_word.overflow = test_overflow(source, destination, (word_t)result, instruction->wb); /* Test for incorrectly flipped sign */
    }

    return 0;
}

/**
 * @brief Decimal Add Instruction
 * 
 * @param instruction 
 * @param program 
 * @return int [0 = success, <= 0 failure]
 */
int execute_dadd(instruction_t *instruction, program_t *program)
{
    /* Refactor to use a loop */
    word_t source = program->register_file[instruction->rc][instruction->source];
    word_t destination = program->register_file[REGISTER][instruction->destination];

    byte_t source_digit = source & 0x000F;
    byte_t destination_digit = destination & 0x000F;
    word_t sum = 0x0000;
    word_t result = 0x0000;

    /* Add 1's Digits */
    result = source_digit + destination_digit + program->program_status_word.carry;
    if(result > 0x0009)
    {
        result -= 0x000A;
        program->program_status_word.carry = 1;
    }
    else
    {
        program->program_status_word.carry = 0;
    }
    
    sum |= result;
    /* Add 10's Digits */
    source_digit = (source >> 4) & 0x000F;
    destination_digit = (destination >> 4) & 0x000F;
    result = source_digit + destination_digit + program->program_status_word.carry;
    if(result > 0x0009)
    {
        result -= 0x000A;
        program->program_status_word.carry = 1;
    }
    else
    {
        program->program_status_word.carry = 0;
    }
    
    sum |= result << 4;

    if(instruction->wb == 0) /* Word Operation */
    {    
        /* Add 100's Digits */
        source_digit = (source >> 8) & 0x000F;
        destination_digit = (destination >> 8) & 0x000F;
        result = source_digit + destination_digit + program->program_status_word.carry;
        if(result > 0x0009)
        {
            result -= 0x000A;
            program->program_status_word.carry = 1;
        }
        else
        {
            program->program_status_word.carry = 0;
        }
        
        sum |= result << 8;
        /* Add 1000's Digits */
        source_digit = (source >> 12) & 0x000F;
        destination_digit = (destination >> 12) & 0x000F;
        result = source_digit + destination_digit + program->program_status_word.carry;
        if(result > 0x0009)
        {
            result -= 0x000A;
            program->program_status_word.carry = 1;
        }
        else
        {
            program->program_status_word.carry = 0;
        }
        
        sum |= result << 12;

        /* Set Destination */
        program->register_file[REGISTER][instruction->destination] = sum;

        /* Test for Zero */
        program->program_status_word.zero = (sum == 0);
    }
    else if(instruction->wb == 1) /* Byte Operation */
    {
        /* Clear Low Byte of Destination */
        program->register_file[REGISTER][instruction->destination] &= 0xFF00;
        /* Set Low Byte of Destination */
        program->register_file[REGISTER][instruction->destination] |= (sum & 0x00FF);
        /* Test for Zero */
        program->program_status_word.zero = ((sum & 0x00FF) == 0);
    }
    /* Clear Negative */
    program->program_status_word.negative = 0;
    /* Clear Overflow */
    program->program_status_word.overflow = 0;

    return 0;
}

/**
 * @brief Compare Instruction
 * 
 * @param instruction 
 * @param program 
 * @return int [0 = success, <= 0 failure]
 */
int execute_cmp(instruction_t *instruction, program_t *program)
{
    word_t source = program->register_file[instruction->rc][instruction->source];
    /* 2's Compliment Source */
    source = ~source + 1;
    word_t destination = program->register_file[REGISTER][instruction->destination];
    int result;

    if(instruction->wb == 0) /* Word Operation */
    {
        result = source + destination;
        
        /* Test for PSW Flags */
        program->program_status_word.carry = (result > 0xFFFF); /* Test Result exceeds word */
        program->program_status_word.zero = ((short)result == 0); /* Test Low Word for Zero */
        program->program_status_word.negative = (word_t)((result >> 15) & 0x01); /* Test MSb */
        program->program_status_word.overflow = test_overflow(source, destination, (word_t)result, instruction->wb); /* Test for incorrectly flipped sign */
    }
    else if(instruction->wb == 1) /* Byte Operation */
    {
        source &= 0x00FF;
        destination &= 0x00FF;
        result = source + destination;

        /* Test for PSW Flags */
        program->program_status_word.carry = (result > 0xFF); /* Test Result exceeds byte */
        program->program_status_word.zero = ((byte_t)result == 0); /* Test Low Byte for Zero */
        program->program_status_word.negative = (word_t)((result >> 7) & 0x01); /* Test MSb */
        program->program_status_word.overflow = test_overflow(source, destination, (word_t)result, instruction->wb); /* Test for incorrectly flipped sign */
    }

    return 0;
}

/**
 * @brief XOR Instruction
 * 
 * @param instruction 
 * @param program 
 * @return int [0 = success, <= 0 failure]
 */
int execute_xor(instruction_t *instruction, program_t *program)
{
    word_t source = program->register_file[instruction->rc][instruction->source];

    if(instruction->wb == 0) /* Word Operation */
    {
        /* DST = DST OR SRC */
        program->register_file[REGISTER][instruction->destination] ^= source;

        /* Test Zero */
        program->program_status_word.zero = ((program->register_file[REGISTER][instruction->destination] == 0));
        /* Test MSb */
        program->program_status_word.negative = 
            (word_t)((program->register_file[REGISTER][instruction->destination] >> 15) & 0x01);
    } 
    else if(instruction->wb == 1) /* Byte Operation */
    {
        /* Clear MSB of Source */
        source &= 0x00FF;
        /* DST = DST OR SRC */
        program->register_file[REGISTER][instruction->destination] ^= source;

        /* Test Zero */
        program->program_status_word.zero = ((program->register_file[REGISTER][instruction->destination] & 0x00FF) == 0);
        /* Test MSb */
        program->program_status_word.negative =
            (byte_t)((program->register_file[REGISTER][instruction->destination] >> 7) & 0x01);
    }
    
    return 0;
}

/**
 * @brief AND Instruction
 * 
 * @param instruction 
 * @param program 
 * @return int [0 = success, <= 0 failure]
 */
int execute_and(instruction_t *instruction, program_t *program)
{
    word_t source = program->register_file[instruction->rc][instruction->source];

    if(instruction->wb == 0) /* Word Operation */
    {
        /* DST = DST AND SRC */
        program->register_file[REGISTER][instruction->destination] &= source;

        /* Test Zero */
        program->program_status_word.zero = ((program->register_file[REGISTER][instruction->destination] == 0));
        /* Test MSb */
        program->program_status_word.negative = 
            (word_t)((program->register_file[REGISTER][instruction->destination] >> 15) & 0x01);
    } 
    else if(instruction->wb == 1) /* Byte Operation */
    {
        /* Clear MSB of Source */
        source &= 0x00FF;

        /* DST = DST AND SRC */
        program->register_file[REGISTER][instruction->destination] &= source;

        /* Test Zero */
        program->program_status_word.zero = ((program->register_file[REGISTER][instruction->destination] & 0x00FF) == 0);
        /* Test MSb */
        program->program_status_word.negative =
            (byte_t)((program->register_file[REGISTER][instruction->destination] >> 7) & 0x01);
    }
    
    return 0;
}

/**
 * @brief OR Instruction
 * 
 * @param instruction 
 * @param program 
 * @return int [0 = success, <= 0 failure]
 */
int execute_or(instruction_t *instruction, program_t *program)
{
    word_t source = program->register_file[instruction->rc][instruction->source];

    if(instruction->wb == 0) /* Word Operation */
    {
        /* DST = DST OR SRC */
        program->register_file[REGISTER][instruction->destination] |= source;

        /* Test Zero */
        program->program_status_word.zero = ((program->register_file[REGISTER][instruction->destination] == 0));
        /* Test MSb */
        program->program_status_word.negative = 
            (word_t)((program->register_file[REGISTER][instruction->destination] >> 15) & 0x01);
    } 
    else if(instruction->wb == 1) /* Byte Operation */
    {
        /* Clear MSB of Source */
        source &= 0x00FF;
        /* DST = DST OR SRC */
        program->register_file[REGISTER][instruction->destination] |= source;

        /* Test Zero */
        program->program_status_word.zero = ((program->register_file[REGISTER][instruction->destination] & 0x00FF) == 0);
        /* Test MSb */
        program->program_status_word.negative =
            (byte_t)((program->register_file[REGISTER][instruction->destination] >> 7) & 0x01);
    }
    
    return 0;
}

/**
 * @brief Bit Test Instruction
 * 
 * @param instruction 
 * @param program 
 * @return int [0 = success, <= 0 failure]
 */
int execute_bit(instruction_t *instruction, program_t *program)
{
    word_t source = program->register_file[instruction->rc][instruction->source];
    word_t destination = program->register_file[REGISTER][instruction->destination];

    if(instruction->wb == 0) /* Word Operation */
    {
        if(source > 15 || source < 0)
        {
            /* Invalid Bit Shift */
            return -1;
        }

        /* Test if Bit is Set */
        program->program_status_word.zero = ((destination & (1 << source)) == 0);
    }
    else if(instruction->wb == 1) /* Byte Operation */
    {
        /* Clear MSB of Source */
        source &= 0x00FF;

        if(source > 7 || source < 0)
        {
            /* Invalid Bit Shift */
            return -1;
        }

        /* Test if Bit is Set */
        program->program_status_word.zero = ((destination & (1 << source)) == 0);
    }
    return 0;
}

/**
 * @brief Bit Clear Instruction
 * 
 * @param instruction 
 * @param program 
 * @return int [0 = success, <= 0 failure]
 */
int execute_bic(instruction_t *instruction, program_t *program)
{
    word_t source = program->register_file[instruction->rc][instruction->source];

    if(instruction->wb == 0) /* Word Operation */
    {
        if(source > 15 || source < 0)
        {
            /* Invalid Bit Shift */
            return -1;
        }
        /* Clear Bit */
        program->register_file[REGISTER][instruction->destination] &= 
            ~(1 << program->register_file[REGISTER][instruction->source]);

        /* Test Zero */
        program->program_status_word.zero = ((program->register_file[REGISTER][instruction->destination] == 0));
        /* Test MSb */
        program->program_status_word.negative = 
        (word_t)((program->register_file[REGISTER][instruction->destination] >> 15) & 0x01);

    }
    else if(instruction->wb == 1) /* Byte Operation */
    {
        /* Clear MSB of Source */
        source &= 0x00FF;

        if(source > 7 || source < 0)
        {
            /* Invalid Bit Shift */
            return -1;
        }
        /* Clear Bit */
        program->register_file[REGISTER][instruction->destination] &= 
            ~(1 << program->register_file[REGISTER][instruction->source]);

        /* Test Zero */
        program->program_status_word.zero = ((program->register_file[REGISTER][instruction->destination] & 0x00FF) == 0);
        /* Test MSb */
        program->program_status_word.negative =
            (byte_t)((program->register_file[REGISTER][instruction->destination] >> 7) & 0x01);
    }

    return 0;
}

/**
 * @brief Bit Set Instruction
 * 
 * @param instruction 
 * @param program 
 * @return int [0 = success, <= 0 failure]
 */
int execute_bis(instruction_t *instruction, program_t *program)
{
    word_t source = program->register_file[instruction->rc][instruction->source];

    if(instruction->wb == 0) /* Word Operation */
    {
        if(source > 15 || source < 0)
        {
            /* Invalid Bit Shift */
            return -1;
        }

        /* Set Bit */
        program->register_file[REGISTER][instruction->destination] |= 
            (1 << program->register_file[REGISTER][instruction->source]);

        /* Test Zero */
        program->program_status_word.zero = ((program->register_file[REGISTER][instruction->destination] == 0));
        /* Test MSb */
        program->program_status_word.negative = 
        (word_t)((program->register_file[REGISTER][instruction->destination] >> 15) & 0x01);

    }
    else if(instruction->wb == 1) /* Byte Operation */
    {
        /* Clear MSB of Source */
        source &= 0x00FF;

        if(source > 7 || source < 0)
        {
            /* Invalid Bit Shift */
            return -1;
        }

        /* Set Bit */
        program->register_file[REGISTER][instruction->destination] |= 
            (1 << program->register_file[REGISTER][instruction->source]);

        /* Test Zero */
        program->program_status_word.zero = ((program->register_file[REGISTER][instruction->destination] & 0x00FF) == 0);
        /* Test MSb */
        program->program_status_word.negative =
            (byte_t)((program->register_file[REGISTER][instruction->destination] >> 7) & 0x01);
    }

    return 0;
}

/**
 * @brief Move Instruction
 *  DST = SRC
 * @param instruction 
 * @param program 
 * @return int [0 = success, <= 0 failure]
 */
int execute_mov(instruction_t *instruction, program_t *program)
{
    if(instruction->wb == 0)/* Word Operation */
    {
        /* DST = SRC */
        program->register_file[REGISTER][instruction->destination] = 
            program->register_file[REGISTER][instruction->source];
    }
    else if(instruction->wb == 1)/* Byte Operation */
    {
        /* Clear DST LSB */
        program->register_file[REGISTER][instruction->destination] &= 0xFF00;
        /* Set DST LSB */
        program->register_file[REGISTER][instruction->destination] |= 
            program->register_file[REGISTER][instruction->source] & 0x00FF;
    }
    if(instruction->destination == PC)
        {
            clear_bubble_queue(&program->bubble_queue);
            insert_bubble(&program->bubble_queue, BUBBLE);
        }
    return 0;
}

/**
 * @brief Swap Register Instruction
 *  DST = SRC
 *  SRC = DST
 * @param instruction 
 * @param program 
 * @return int [0 = success, <= 0 failure]
 */
int execute_swap(instruction_t *instruction, program_t *program)
{
    /* Store Destination in Temp Register */
    word_t temp = program->register_file[REGISTER][instruction->destination];
    /* DST = SRC */
    program->register_file[REGISTER][instruction->destination] = 
        program->register_file[REGISTER][instruction->source];
    /* SRC = Temp */
    program->register_file[REGISTER][instruction->source] = temp;

    if(instruction->destination == PC)
        {
            clear_bubble_queue(&program->bubble_queue);
            insert_bubble(&program->bubble_queue, BUBBLE);
        }

    return 0;
}

/**
 * @brief Shift Right Arithmetic Instruction
 * 
 * @param instruction 
 * @param program 
 * @return int 
 */
int execute_sra(instruction_t *instruction, program_t *program)
{
    if(instruction->wb == 0) /* Word Operation */
    {
        short temp = program->register_file[REGISTER][instruction->destination];
        /* Arithmetic Shift Right */
        temp >>= 1;
        program->register_file[REGISTER][instruction->destination] = (word_t)temp;

        /* Test Zero */
        program->program_status_word.zero = (temp == 0);
        /* Test MSb */
        program->program_status_word.negative = (temp >> 15) & 0x01;
    }
    else if(instruction->wb == 1) /* Byte Operation */
    {
        signed char temp = (signed char)program->register_file[REGISTER][instruction->destination];
        /* Arithmetic Shift Right */
        temp >>= 1;
        /* Clear LSB of DST */
        program->register_file[REGISTER][instruction->destination] &= 0xFF00;
        /* Set LSB of DST */
        program->register_file[REGISTER][instruction->destination] |= (word_t)(temp & 0x00FF);

        /* Test Zero */
        program->program_status_word.zero = (temp == 0);
        /* Test MSb */
        program->program_status_word.negative = (temp >> 7) & 0x01;
    } 
    
    return 0;
}

/**
 * @brief Rotate Right through Carry Instruction
 * 
 * @param instruction 
 * @param program 
 * @return int 
 */
int execute_rrc(instruction_t *instruction, program_t *program)
{
    /* Save LSb */
    byte_t new_carry = program->register_file[REGISTER][instruction->destination] & 0x0001;

    if(instruction->wb == 0) /* Word Operation */
    {
        /* Arithmetic Shift Right */
        program->register_file[REGISTER][instruction->destination] >>= 1;
        /* Set MSb */
        program->register_file[REGISTER][instruction->destination] |= (program->program_status_word.carry << 15);
        
        /* Set Carry */
        program->program_status_word.carry = new_carry;
        /* Test Zero */
        program->program_status_word.zero = (program->register_file[REGISTER][instruction->destination] == 0);
        /* Test MSb */
        program->program_status_word.negative = 
            (program->register_file[REGISTER][instruction->destination] >> 15) & 0x01;
    }
    else if(instruction->wb == 1) /* Byte Operation */
    {
        /* Save LSB */
        byte_t temp = program->register_file[REGISTER][instruction->destination] & 0x00FF;
        /* Clear LSB of DST */
        program->register_file[REGISTER][instruction->destination] &= 0xFF00;
        /* Arithmetic Shift Right */
        temp >>= 1;
        /* Set MSb */
        program->register_file[REGISTER][instruction->destination] |= (program->program_status_word.carry << 7);
        /* set LSB */
        program->register_file[REGISTER][instruction->destination] |= (temp & 0x00FF);
        
        /* Set Carry */
        program->program_status_word.carry = new_carry;
        /* Test Zero */
        program->program_status_word.zero = (temp == 0);
        /* Test MSb */
        program->program_status_word.negative = (temp >> 7) & 0x01;
    } 
    
    return 0;
}

/**
 * @brief Swap Bytes
 *  DST = {DST[7:0], DST[15:8]}
 * @param instruction 
 * @param program 
 * @return int [0 = success, <= 0 failure]
 */
int execute_swpb(instruction_t *instruction, program_t *program)
{
    /* Store LSB in temp byte */
    byte_t temp = (byte_t)program->register_file[REGISTER][instruction->destination];
    /* Clear LSB of DST */
    program->register_file[REGISTER][instruction->destination] &= 0xFF00;
    /* Set LSB of DST */
    program->register_file[REGISTER][instruction->destination] |= 
        (program->register_file[REGISTER][instruction->destination] >> 8) & 0x00FF;
    /* Clear MSB of DST */
    program->register_file[REGISTER][instruction->destination] &= 0x00FF;
    /* Set MSB of DST */
    program->register_file[REGISTER][instruction->destination] |= temp << 8;

    return 0;
}

/**
 * @brief Sign Extend Byte to Word
 * 
 * @param instruction 
 * @param program 
 * @return int 
 */
int execute_sxt(instruction_t *instruction, program_t *program)
{
    if((program->register_file[REGISTER][instruction->destination] >> 7) & 0x01)
    {
        program->register_file[REGISTER][instruction->destination] |= 0xFF00;
    }
    else
    {
        program->register_file[REGISTER][instruction->destination] &= 0x00FF;
    }

    /* Test Zero */
    program->program_status_word.zero = (program->register_file[REGISTER][instruction->destination] == 0);
    /* Test MSb */
    program->program_status_word.negative = 
        (program->register_file[REGISTER][instruction->destination] >> 15) & 0x01;

    return 0;
}

/**
 * @brief Set Priority Instruction
 * 
 * @param instruction 
 * @param program 
 * @return int 
 */
int execute_setpri(instruction_t *instruction, program_t *program)
{
    instruction;
    program;
    //printf("%04x\tSet Priority\n", instruction->address);
    return -1;
}

/**
 * @brief Vector Interrupt Instruction
 * 
 * @param instruction 
 * @param program 
 * @return int 
 */
int execute_svc(instruction_t *instruction, program_t *program)
{
    instruction;
    program;
    //printf("%04x\tSoftware Interrupt\n", instruction->address);
    return -1;
}

/**
 * @brief Set Condition Code Instruction
 * 
 * @param instruction 
 * @param program 
 * @return int 
 */
int execute_setcc(instruction_t *instruction, program_t *program)
{
    program->program_status_word.negative |= instruction->status.negative;
    program->program_status_word.zero |= instruction->status.zero;
    program->program_status_word.overflow |= instruction->status.overflow;
    program->program_status_word.sleep |= instruction->status.sleep;
    program->program_status_word.carry |= instruction->status.carry;
    return 0;
}

/**
 * @brief Clear Condition Code Instruction
 * 
 * @param instruction 
 * @param program 
 * @return int 
 */
int execute_clrcc(instruction_t *instruction, program_t *program)
{
    program->program_status_word.negative &= ~(instruction->status.negative);
    program->program_status_word.zero &= ~(instruction->status.zero);
    program->program_status_word.overflow &= ~(instruction->status.overflow);
    program->program_status_word.sleep &= ~(instruction->status.sleep);
    program->program_status_word.carry &= ~(instruction->status.carry);
    return 0;
}

/**
 * @brief Conditional Execution Instruction
 * 
 * @param instruction 
 * @param program 
 * @return int 
 */
int execute_cex(instruction_t *instruction, program_t *program)
{
    if(check_condition(instruction->condition_code, program->program_status_word))
    {
        /* Execute True, Skip False*/
        /* Add True_Count No_Bubbles then False_Count Bubbles to bubble queue */
        for(int i = 0; i < instruction->t_count; i++)
        {
            insert_bubble(&program->bubble_queue, NO_BUBBLE);
        }
        for(int i = 0; i < instruction->f_count; i++)
        {
            insert_bubble(&program->bubble_queue, BUBBLE);
        }
    }
    else
    {
        /* Skip True, Execute False */
        /* Add False_Count No_Bubbles then True_Count Bubbles to bubble queue */
        for(int i = 0; i < instruction->t_count; i++)
        {
            insert_bubble(&program->bubble_queue, BUBBLE);
        }
        for(int i = 0; i < instruction->f_count; i++)
        {
            insert_bubble(&program->bubble_queue, NO_BUBBLE);
        }
    }
    
    return 0;
}

/**
 * @brief Load Instruction
 * 
 * @param instruction 
 * @param program 
 * @return int 
 */
int execute_ld(instruction_t *instruction, program_t *program)
{

    word_t increment_size = 2 - instruction->wb;
    /* Handle Pre Inc/Dec */
    if(instruction->prpo == PRE)
    {
        if(instruction->increment == 1)
        {
            program->register_file[REGISTER][instruction->source]+=increment_size;
        }
        else if(instruction->decrement == 1)
        {
            program->register_file[REGISTER][instruction->source]-=increment_size;
        }
    }
    /* Copy Memory Source Address to DMAR */
    program->data_memory_address_register = program->register_file[REGISTER][instruction->source];
    /* Handle Pose Inc/Dec */
    if(instruction->prpo == POST)
    {
        if(instruction->increment == 1)
        {
            program->register_file[REGISTER][instruction->source]+=increment_size;
        }
        else if(instruction->decrement == 1)
        {
            program->register_file[REGISTER][instruction->source]-=increment_size;
        }
    }
    /* Set DCTRL to Read */
    program->data_control_register = control_table[READ][instruction->wb];
    /* Set Data Flag*/
    instruction->data_flag = 1;

    return 0;
}

/**
 * @brief Store Instruction
 * 
 * @param instruction 
 * @param program 
 * @return int 
 */
int execute_st(instruction_t *instruction, program_t *program)
{
    word_t increment_size = 2 - instruction->wb;
    /* Handle Pre Inc/Dec */
    if(instruction->prpo == PRE)
    {
        if(instruction->increment == 1)
        {
            program->register_file[REGISTER][instruction->destination]+=increment_size;
        }
        else if(instruction->decrement == 1)
        {
            program->register_file[REGISTER][instruction->destination]-=increment_size;
        }
    }
    /* Copy Memory Destination Address to DMAR */
    program->data_memory_address_register = program->register_file[REGISTER][instruction->destination];
    /* Copy Source Data to Data Memory Buffer */
    program->data_memory_buffer_register = program->register_file[REGISTER][instruction->source];
    /* Handle Pose Inc/Dec */
    if(instruction->prpo == POST)
    {
        if(instruction->increment == 1)
        {
            program->register_file[REGISTER][instruction->destination]-=increment_size;
        }
        else if(instruction->decrement == 1)
        {
            program->register_file[REGISTER][instruction->destination]-=increment_size;
        }
    }
    /* Set DCTRL to Write */
    program->data_control_register = control_table[WRITE][instruction->wb];
    /* Set Data Flag*/
    instruction->data_flag = 1;

    return 0;
}

/**
 * @brief Move Low Byte - Most Significant Byte Unchanged
 * 
 * @param instruction 
 * @param program 
 * @return int [0 = success, <= 0 failure]
 */
int execute_movl(instruction_t *instruction, program_t *program)
{
    /* Clear LSB of DST */
    program->register_file[REGISTER][instruction->destination] &= 0xFF00;
    /* Set LSB of DST */
    program->register_file[REGISTER][instruction->destination] |= (instruction->source & 0x00FF);

    return 0;
}

/**
 * @brief Move Low Byte - Most Significant Byte Cleared
 * 
 * @param instruction 
 * @param program 
 * @return int [0 = success, <= 0 failure]
 */
int execute_movlz(instruction_t *instruction, program_t *program)
{
    /* Clear DST */
    program->register_file[REGISTER][instruction->destination] = 0x0000;
    /* Set LSB of DST */
    program->register_file[REGISTER][instruction->destination] |= (instruction->source & 0x00FF);

    return 0;
}

/**
 * @brief Move Low Byte - Most Significant Byte Set
 * 
 * @param instruction 
 * @param program 
 * @return int [0 = success, <= 0 failure]
 */
int execute_movls(instruction_t *instruction, program_t *program)
{
    /* Set MSB and Clear LSB of DST */
    program->register_file[REGISTER][instruction->destination] = 0xFF00;
    /* Set LSB of DST */
    program->register_file[REGISTER][instruction->destination] |= instruction->source & 0x00FF;

    return 0;
}

/**
 * @brief Move High Byte - Least Significant Byte Unchanged
 * 
 * @param instruction 
 * @param program 
 * @return int [0 = success, <= 0 failure]
 */
int execute_movh(instruction_t *instruction, program_t *program)
{
    /* Clear MSB of DST */
    program->register_file[REGISTER][instruction->destination] &= 0x00FF;
    /* Set MSB of DST */
    program->register_file[REGISTER][instruction->destination] |= 
        ((instruction->source & 0x00FF) << 8);

    return 0;
}

/**
 * @brief Load Relative Instruction
 * 
 * @param instruction 
 * @param program 
 * @return int 
 */
int execute_ldr(instruction_t *instruction, program_t *program)
{
    /* Sign Extend Offset */
    signed short offset = (signed short)((signed char)(instruction->offset << 1) >> 1);
    /* Calculate Effective Address */
    word_t effective_address = program->register_file[REGISTER][instruction->source] + offset;
    /* Copy Memory Source Address to DMAR */
    program->data_memory_address_register = effective_address;
    /* Set DCTRL to Read */
    program->data_control_register = control_table[READ][instruction->wb];
    /* Set Data Flag*/
    instruction->data_flag = 1;

    return 0;
}

/**
 * @brief Store Relative Instruction
 * 
 * @param instruction 
 * @param program 
 * @return int 
 */
int execute_str(instruction_t *instruction, program_t *program)
{
    /* Sign Extend Offset */
    signed short offset = (signed short)((signed char)(instruction->offset << 1) >> 1);
    /* Calculate Effective Address */
    word_t effective_address = program->register_file[REGISTER][instruction->destination] + offset;
    /* Copy Memory Source Address to DMAR */
    program->data_memory_address_register = effective_address;
    /* Copy Source Data to Data Memory Buffer */
    program->data_memory_buffer_register = program->register_file[REGISTER][instruction->source];
    /* Set DCTRL to Read */
    program->data_control_register = control_table[WRITE][instruction->wb];
    /* Set Data Flag*/
    instruction->data_flag = 1;

    return 0;
}