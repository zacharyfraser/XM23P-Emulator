/**
 * @file main.c
 * @brief Parse xme files for S-Records, then load program into memory.
 * 
 * @author Zachary Fraser
 * @date 2024-05-18
 */

#include "main.h"
#include "parse_records.h"
#include "load_memory.h"
#include "display_memory.h"
#include "operating_system.h"
#include "utilities.h"

FILE *file;
char input_record[MAX_RECORD_LENGTH];
s_record_t s_record;

int error_status = 0;

program_t program = {.breakpoint = -1};

int breakpoint = -1;

/**
 * @brief Parse every file provided in argv
 * 
 * @param argc Number of entrypoint arguments
 * @param argv entrypoint arguments - argv[0] is name of loader executable,
 *      argv[1+] is name of xme executable file
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
