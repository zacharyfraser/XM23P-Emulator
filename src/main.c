/**
 * @file main.c
 * @brief Parse xme files for S-Records, then load program into memory.
 * 
 * @author Zachary Fraser
 * @date 2024-05-18
 */

#include "main.h"

program_t program;
/**
 * @brief XM23P CPU Emulator Entry Point
 * 
 * @param argc Number of entrypoint arguments
 * @param argv Entrypoint arguments - argv[0] = executable name, argv[1] = file path
 * @return Exit Status - [0 = success, 1 = failure]
 */
int main(int argc, char **argv)
{
    /* Automatically load file supplied to executable */
    if(argc > 1)
    {
        load_memory(&program, argv[1]);
    }
    
    run_operating_system(&program);
    return EXIT_SUCCESS;
}
