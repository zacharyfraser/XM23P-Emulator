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
 * @param address Memory address to decode
 * @param instruction Pointer to structure to store decoded instruction
 * @return Exit Status - [0 = success, <- = failure]
 */
int decode_instruction(instruction_t *instruction, 
    byte_t *instruction_memory, int program_counter)
{
    /* Check if address is within instruction memory */
    if (program_counter >= INSTRUCTION_MEMORY_LENGTH)
    {
        return -1;
    }

    /* Read instruction from memory */ 
    word_t word = (instruction_memory[program_counter] << 8) 
        | instruction_memory[program_counter + 1];

    return 0;
}