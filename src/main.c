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

FILE *file;
char input_record[MAX_RECORD_LENGTH];
s_record_t s_record;

int error_status = 0;
/* Instruction and Data Memory Spaces 
    Populated from S1 and S2 Records  */
byte_t instruction_memory[INSTRUCTION_MEMORY_LENGTH];
byte_t data_memory[DATA_MEMORY_LENGTH];

/* Executable Name and Program Starting Address 
        Populated from S0 and S9 Records        */
byte_t executable_name[MAX_RECORD_LENGTH];
byte_t starting_address[ADDRESS_LENGTH];
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
    if(argc < 2)
    {
        printf("No File Supplied - Terminating Program");
        return EXIT_FAILURE;
    }
    for (int i = 1; i < argc; i++)
    {
        /* Open File for Reading */
        if (fopen_s(&file, argv[i], "r") != 0)
        {
            /* Print Error, return non-zero from main */
            printf("Error opening file.");
            return EXIT_FAILURE;
        }
        
        while (fgets(input_record, sizeof(input_record), file))
        {
            error_status = parse_record(input_record, &s_record);
            if (error_status != 0)
            {
                printf("Invalid Line: %s\n", 
                    input_record);

                continue;
            }

            /* Process Record based on type */
            switch (s_record.type)
            {
            case NAME_TYPE:
                /* Load name from record data into executable name */
                error_status = load_record_name(&s_record, executable_name);
                if(error_status != 0)
                {
                    printf("Error loading data into executable_name");
                }
                break;

            case INSTRUCTION_TYPE:
                /* Load Instructions from record data to instruction memory */
                error_status = load_record_data(&s_record, instruction_memory);
                if(error_status != 0)
                {
                    printf("Error loading data into instruction_memory");
                }
                break;

            case DATA_TYPE:
                /* Load Data from record data into data memory */
                error_status = load_record_data(&s_record, data_memory);
                if(error_status != 0)
                {
                    printf("Error loading data into data_memory");
                }
                break;

            case ADDRESS_TYPE:
                /* Load Starting Address from record address 
                    into instruction memory */
                error_status = 
                    load_record_address(&s_record, starting_address);

                if(error_status != 0)
                {
                    printf("Error loading address into starting_address");
                }
                break;

            default:
                /* If invalid type, print warning and move to next record */
                printf("Invalid Record Type: %c", s_record.type);
                continue;
                break;
            }
            /* Debug Print Record Members */
#ifdef DEBUG
            printf("Valid Line: %s", input_record);
            printf("Type: %c\n", s_record.type);
            printf("Length: %d\n", s_record.length);
            printf("Address: ");
            for(int j = 0; j < ADDRESS_LENGTH; j++)
            {
                printf("%02x", s_record.address[j]);
            }
            printf("\nData: ");
            for(int j = 0; j < s_record.length - 3; j++)
            {
                printf("%02x", s_record.data[j]);
            }
                printf("\n\n");
#endif
        }
        while(1)
        {   
            int start_address, ending_address;
            char address_type;
            while(1)
            {
                printf("Please Enter Address Type - I or D: ");
                scanf_s("%c", &address_type, 1);
                if(address_type != 'I' && address_type != 'D')
                {
                    printf("Invalid Type - Press Enter");
                    (void) getchar();
                    continue;
                }

                break;
            }

            printf("\nPlease Enter Starting and ending addresses: ");
            scanf_s("%x %x", &start_address, &ending_address);
            if(address_type == 'I')
            {
                display_memory(instruction_memory, start_address, ending_address);
            }
            else if (address_type == 'D')
            {
                display_memory(data_memory, start_address, ending_address);
            }
            (void)getchar();
        }
        
        
        
    }

    return EXIT_SUCCESS;
}
