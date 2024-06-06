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
        printf("g - Run\n");
        printf("x - Exit\n");
}

/**
 * @brief Select and run a utility
 * 
 */
void run_operating_system(program_t *program)
{
    int end = 0;
    while(end != 1)
    {
        char utility;
        display_utilities();
        printf("Please Enter Utility: ");
        scanf_s("%c", &utility, 1);
        printf("\n");
        switch(utility)
        {
            case MEMORY_DUMP:
                memory_dump(program->instruction_memory, program->data_memory);
                break;
            case MEMORY_WRITE:
                memory_write(program->instruction_memory, program->data_memory);
                break;
            case REGISTER_DUMP:
                register_dump(program->register_file);
                break;
            case REGISTER_SET:
                register_set(program->register_file);
                break;
            case SET_BREAKPOINT:
                set_breakpoint(&program->breakpoint);
                break;
            case RUN:
                run(program);
                break;
            case EXIT:
                end = 1;
                break;
            default:
                printf("\nInvalid Utility - Press Enter");
                break;
        }
        printf("\n");
        (void) getchar();
    }
}