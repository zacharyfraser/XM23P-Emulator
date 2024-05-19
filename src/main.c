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
            if (parse_record(input_record, &s_record) != 0)
            {
                printf("Invalid Line: >%s<\n", input_record);
                continue;
            }
            printf("Valid Line: >%s<\n", input_record);
            printf("Type: >%c<\n", s_record.type);
            printf("Length: >%c<\n", s_record.length);
            printf("Address: >%s<\n", s_record.address);
            printf("Data: >%s<\n", s_record.data);
        }
        
    }

    return EXIT_SUCCESS;
}