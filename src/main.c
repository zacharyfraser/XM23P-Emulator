/**
 * @file main.c
 * @brief Parse xme files for S-Records, then load program into memory.
 * 
 * @author Zachary Fraser
 * @date 2024-05-18
 */

#include "main.h"
#include "parse_records.h"

FILE *file;
char input_record[MAX_RECORD_LENGTH];
s_record_t s_record;

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
    for (int i = 1; i < argc; i++)
    {
        /* Open File for Reading */
        if (fopen_s(&file, argv[i], "r") != 0)
        {
            /* Print Error to stderr, return non-zero from main */
            perror("Error opening file.");
            return EXIT_FAILURE;
        }
        

        while (fgets(input_record, sizeof(input_record), file))
        {
            int record_status = parse_record(input_record, &s_record);
            if (record_status != 0)
            {
                perror("Invalid Line: %s Error: %d\n", input_record, record_status);
                continue;
            }
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
        }
        
    }

    return EXIT_SUCCESS;
}