/**
 * @file operating_system.c
 * @brief Implementation of utilities operating system
 * 
 * @author Zach Fraser
 * @date 2024-06-05
 */

#include "operating_system.h"

/**
 * @brief Print available utilities to the console
 * 
 */
void display_utilities()
{
        printf("Available Utilities:\n");
        printf("m - Memory Dump\n");
        printf("w - Memory Write\n");
        printf("r - Register Dump\n");
        printf("s - Register Set\n");
        printf("b - Set Breakpoint\n");
        printf("x - Exit\n");
}

/**
 * @brief Select and run a utility
 * 
 */
void run_operating_system(byte_t *instruction_memory, byte_t *data_memory, byte_t *register_file, int *breakpoint)
{
    int end = 0;
    while(end != 1)
    {
        char utility;
        display_utilities();
        printf("Please Enter Utility: ");
        scanf_s("%c", &utility, 1);
        switch(utility)
        {
            case MEMORY_DUMP:
                memory_dump(instruction_memory, data_memory);
                break;
            case MEMORY_WRITE:
                memory_write(instruction_memory, data_memory);
                break;
            case REGISTER_DUMP:
                register_dump(register_file);
                break;
            case REGISTER_SET:
                register_set(register_file);
                break;
            case SET_BREAKPOINT:
                set_breakpoint(breakpoint);
                break;
            case EXIT:
                end = 1;
                (void) getchar();
                break;
            default:
                printf("\nInvalid Utility - Press Enter");
                (void) getchar();
                break;
        }

    }
}