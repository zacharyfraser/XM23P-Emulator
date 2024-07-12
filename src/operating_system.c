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
        printf("l - Load Program\n");
        printf("v - Restart Program\n");
        printf("g - Run\n");
        printf("b - Set Breakpoint\n");
        printf("m - Memory Dump\n");
        printf("w - Memory Write\n");
        printf("r - Register Dump\n");
        printf("s - Register Set\n");
        printf("x - Exit\n");
        printf("h - Help\n");
}

/**
 * @brief Select and run a utility
 * 
 * @param program - Pointer to program structure
 */
void run_operating_system(program_t *program)
{
    int exit = 0;
    display_utilities();
    while(exit != 1)
    {
        char utility;
        printf("User> ");
        scanf_s(" %c", &utility, 1);
        switch(utility)
        {
        case LOAD:
            load_memory(program, NULL);
            break;
        case RESTART:
            restart_program(program);
            break;
        case RUN:
            run(program);
            break;
        case SET_BREAKPOINT:
            set_breakpoint(&program->breakpoint);
            break;
        case MEMORY_DUMP:
            memory_dump(program->instruction_memory, program->data_memory);
            break;
        case MEMORY_WRITE:
            memory_write(program->instruction_memory, program->data_memory);
            break;
        case REGISTER_DUMP:
            register_dump(program->register_file[REGISTER]);
            break;
        case REGISTER_SET:
            register_set(program->register_file[REGISTER]);
            break;
        case EXIT:
            exit = 1;
            break;
        case HELP:
            display_utilities();
            break;
        default:
            break;
        }
        printf("\n");
    }
}