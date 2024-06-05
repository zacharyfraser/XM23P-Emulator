/**
 * @file utilities.c
 * @brief Implementation of utilities available to the user
 * 
 * @author Zach Fraser
 * @date 2024-06-05
 */

#include "utilities.h"

/**
 * @brief Memory Dump Utility - Dumps memory to the console
 * 
 */
void memory_dump(byte_t *instruction_memory, byte_t *data_memory)
{
    printf("Memory Dump Utility\n");
    (void) getchar();
    printf("Select Memory Type: \n");
    printf("0 - Program Memory | 1 - Data Memory\n");
    char memory_type;
    scanf_s("%c", &memory_type, 1);
    printf("Enter Memory Start Address: ");
    int start_address;
    scanf_s("%x", &start_address);
    printf("Enter Memory End Address: ");
    int end_address;
    scanf_s("%x", &end_address);
    switch (memory_type)
    {
    case INSTRUCTION_MEMORY:
            if(display_memory(instruction_memory, start_address, end_address))
            {
                printf("Invalid Address\n");
            }
        break;
    case DATA_MEMORY:
            if(display_memory(data_memory, start_address, end_address))
            {
                printf("Invalid Address\n");
            }
        break;
    
    default:
        break;
    }
}

/**
 * @brief Memory Write Utility - Writes to memory
 * 
 */
void memory_write(byte_t *instruction_memory, byte_t *data_memory)
{
    printf("Memory Write Utility\n");
    (void) getchar();
    printf("Select Memory Type: \n");
    printf("0 - Program Memory | 1 - Data Memory\n");
    char memory_type;
    scanf_s("%c", &memory_type, 1);
    printf("Enter Memory Address: ");
    int address;
    scanf_s("%x", &address);
    printf("Enter Memory Word: ");
    int word;
    scanf_s("%x", &word);

    switch (memory_type)
    {
    case INSTRUCTION_MEMORY:
        if(address + 1 < INSTRUCTION_MEMORY_LENGTH)
        {
            instruction_memory[address] = (byte_t) (word >> 8);
            instruction_memory[address + 1] = (byte_t) (word & 0xff);
        }
        else
        {
            printf("Invalid Address\n");
        }
        break;
    case DATA_MEMORY:
        if(address + 1 < DATA_MEMORY_LENGTH)
        {
            data_memory[address] = (byte_t) (word >> 8);
            data_memory[address + 1] = (byte_t) (word & 0xFF);
        }
        else
        {
            printf("Invalid Address\n");
        }
        break;
    default:
        printf("Invalid Memory Type\n");
        break;
    }
}

/**
 * @brief Register Dump Utility - Dumps register file contents to the console
 * 
 */
void register_dump(byte_t *register_file)
{
    printf("Register Dump Utility\n");
    for(int i = 0; i < REGISTER_FILE_LENGTH; i++)
    {
        printf("R%d: %02x %02x\n", i, register_file[2 * i], register_file[(2 * i) + 1]);
    }
}

/**
 * @brief Register Set Utility - Sets register file contents
 * 
 */
void register_set(byte_t *register_file)
{
    printf("Register Set Utility\n");
    printf("Enter Register Number: ");
    int register_number;
    scanf_s("%d", &register_number);
    printf("Enter Register Value: ");
    int register_value;
    scanf_s("%x", &register_value);
    if(register_number < REGISTER_FILE_LENGTH && register_number >= 0)
    {
        register_file[2 * register_number] = (byte_t) (register_value >> 8);
        register_file[2 * register_number + 1] = (byte_t) (register_value & 0xFF);
    }
    else
    {
        printf("Invalid Register Number\n");
    }
}

/**
 * @brief Set Breakpoint Utility - Sets a breakpoint
 * 
 */
void set_breakpoint(int *breakpoint)
{
    printf("Set Breakpoint Utility\n");
    printf("Enter Breakpoint Address: ");
    int breakpoint_address;
    scanf_s("%x", &breakpoint_address);
    if(breakpoint_address < INSTRUCTION_MEMORY_LENGTH && breakpoint_address >= 0)
    {
        *breakpoint = breakpoint_address;
    }
    else
    {
        printf("Invalid Breakpoint Address\n");
    }
    (void) getchar();
}