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
}

/**
 * @brief Run Utility - Runs the program
 * 
 */
void run(program_t *program)
{
    printf("Run Utility\n");
    program->program_counter = program->starting_address;
    printf("Program Counter: %04x\n", program->program_counter);
    printf("Breakpoint: %04x\n", program->breakpoint);
    while(program->program_counter != (program->breakpoint & 0xFFFE))
    {
        /* Fetch */
        byte_t instruction[2];
        instruction[0] = program->instruction_memory[program->program_counter];
        instruction[1] = program->instruction_memory[program->program_counter + 1];
        /* Decode */
        /* Execute */
        /* Store */
        printf("%04x:  %02x %02x\n", program->program_counter, instruction[1], instruction[0]);
        program->program_counter += 2;
        if(instruction[0] == 0x00 && instruction[1] == 0x00)
        {
            break;
        }
    }
}

/**
 * @brief Load Memory Utility - Loads data and instructions from xme file into memory
 * 
 * @param program Context struct for the program
 * @param supplied_path Path to the xme file - NULL if not supplied
 */
void load_memory(program_t *program, char *supplied_path)
{
    /* Clear Program */
    memset(program, 0, sizeof(program_t));
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
}