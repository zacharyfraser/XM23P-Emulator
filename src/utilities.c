/**
 * @file utilities.c
 * @brief Implementation of utilities available to the user
 * 
 * @author Zach Fraser
 * @date 2024-06-05
 */

#include "utilities.h"

/**
 * @brief Memory Dump Utility - Prompts user for starting and ending addresses, and prints memory between addresses.
 * Select either instruction or data memory.
 * Prints memory in chunks of 16 bytes.
 * 
 * @param instruction_memory
 * @param data_memory
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
    /* Could be simplified with 2D Array */
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
 * @param instruction_memory - Pointer to start of program instruction memory
 * @param data_memory - Pointer to start of program data memory
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
            instruction_memory[address + 1] = (byte_t) (word >> 8);
            instruction_memory[address] = (byte_t) (word & EIGHT_BITS);
        }
        else
        {
            printf("Invalid Address\n");
        }
        break;
    case DATA_MEMORY:
        if(address + 1 < DATA_MEMORY_LENGTH)
        {
            data_memory[address + 1] = (byte_t) (word >> 8);
            data_memory[address] = (byte_t) (word & EIGHT_BITS);
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
 * @brief Register Dump Utility - Print contents of register file to console
 * 
 * @param register_file - Pointer to program register file
 */
void register_dump(word_t *register_file)
{
    printf("Register Dump Utility\n");
    for(int i = 0; i < REGISTER_FILE_LENGTH; i++)
    {
        printf("R%d: %04x\n", i, register_file[i]);
    }
}

/**
 * @brief Register Set Utility - Modify register file contents
 * 
 * @param register_file - Pointer to program register file
 */
void register_set(word_t *register_file)
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
        register_file[register_number] = (word_t) (register_value);
    }
    else
    {
        printf("Invalid Register Number\n");
    }
}

/**
 * @brief Set Breakpoint Utility - Set the address to stop the program counter.  
 * Execution stops after previous instruction.
 * 
 * @param breakpoint - Pointer to breakpoint register
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
}

/**
 * @brief Run Utility - Start the pipelined instruction execution
 * 
 * @param program - Program context struct
 */
void run(program_t *program)
{
    printf("Run Utility\n");
    int pause_cycle = 0;
    /* Start XM23P Pipelined Instruction Cycle */
    program->cycle_state = CYCLE_START;
    /* Loop Until Breakpoint Reached */
    while(pause_cycle == 0)
    {
        switch(program->cycle_state)
        {
            case CYCLE_START:
                /* Initialize with NOOP */
                program->instruction_register = INSTRUCTION_NOOP;
                /* FETCH_0 */
                fetch_instruction(program, 0);
                /* DECODE_0 */
                decode_instruction(&program->instruction, program->instruction_register);
                program->cycle_state = CYCLE_WAIT_0;
                break;
            case CYCLE_WAIT_0:
                /* Set Current Instruction Address for Debugging */
                program->instruction.address = program->PROGRAM_COUNTER - 4;
                /* FETCH_1 */
                fetch_instruction(program, 1);
                /* EXECUTE_0 */
                execute_instruction(&program->instruction, program);
                program->cycle_state = CYCLE_WAIT_1;

                /* Check for Breakpoint - PC Incremented in previous cycle */
                if(program->PROGRAM_COUNTER - 2 == (program->breakpoint & 0xFFFE))
                {
                    pause_cycle = 1;
                    /* Decrement PC for resuming execution */
                    program->PROGRAM_COUNTER -= 2;
                    continue;
                }
                break;
            case CYCLE_WAIT_1:
                /* FETCH_0 */
                fetch_instruction(program, 0);
                /* DECODE_0 */
                decode_instruction(&program->instruction, program->instruction_register);
                program->cycle_state = CYCLE_WAIT_0;
                break;
            default:
                break;
        }
        program->clock_cycles++;
    }
}

/**
 * @brief Restart program by resetting program counter to starting address
 * 
 * @param program 
 */
void restart_program(program_t *program)
{
    program->PROGRAM_COUNTER = (word_t)program->starting_address;
}

/**
 * @brief Load Memory Utility - Loads data and instructions from xme file into memory
 * 
 * @param program Context struct for the program
 * @param supplied_path Path to the xme file - NULL if not supplied
 */
void load_memory(program_t *program, char *supplied_path)
{
    printf("Load Memory Utility\n");
    /* Clear Program */
    memset(program, 0, sizeof(program_t));
    initialize_register_file(program->register_file);
    int error_status = 0;
    char program_path[MAX_PATH_LENGTH];
    char input_record[MAX_RECORD_LENGTH];
    FILE *file;
    s_record_t s_record;

    /* If Loaded From OS*/
    if(supplied_path == NULL)
    {
    printf("Load Memory Utility\n");
    printf("Enter Program Path: ");
    scanf_s("%s", program_path, MAX_PATH_LENGTH);
    }
    else
    /* If Supplied to Executable */
    {
        strcpy_s(program_path, MAX_PATH_LENGTH, supplied_path);
        printf("Loading Program From: %s\n", program_path);
    }

    /* Check for errors opening file */
    if(fopen_s(&file, program_path, "r") != 0)
    {
        printf("Error Opening File\n");
        return;
    }

    /* Parse Each Record in File*/
    while(fgets(input_record, sizeof(input_record), file))
    {
        error_status = parse_record(input_record, &s_record);
        if(error_status != 0)
        {
            printf("Invalid Line: %s", input_record);
            continue;
        }
        switch (s_record.type)
        {
        case NAME_TYPE:
            /* Load name from record data into executable name */
            error_status = load_record_name(&s_record, program->executable_name);
            if(error_status != 0)
            {
                printf("Error loading data into executable_name");
            }
            break;
        case INSTRUCTION_TYPE:
            /* Load Instructions from record data to instruction memory */
            error_status = load_record_data(&s_record, program->instruction_memory);
            if(error_status != 0)
            {
                printf("Error loading data into instruction_memory");
            }
            break;
        case DATA_TYPE:
            /* Load Data from record data into data memory */
            error_status = load_record_data(&s_record, program->data_memory);
            if(error_status != 0)
            {
                printf("Error loading data into data_memory");
            }
            break;

        case ADDRESS_TYPE:
            /* Load Starting Address from record address 
                into starting_address */
            error_status = load_record_address(&s_record, &program->starting_address);

            if(error_status != 0)
            {
                printf("Error loading data into starting_address");
            }
            break;

        default:
            /* If invalid type, print warning and move to next record */
            printf("Invalid Record Type: %c", s_record.type);
            continue;
            break;
        }
    }
    fclose(file);
    /* Load Starting Address into Program Counter */
    program->PROGRAM_COUNTER = (word_t)program->starting_address;
}