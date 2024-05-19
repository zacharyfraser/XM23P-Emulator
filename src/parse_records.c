/**
 * @file parse_records.c
 * @brief Parsing S-Records
 * 
 * @author Zachary Fraser
 * @date 2024-05-18
 */

#include "parse_records.h"

/**
 * @brief Parse input record into s_record
 * @param input_record Character string containing s_record
 * @param s_record Record struct to populate with s_record
 * 
 * @return int [0 = Success, < 0 = Failure]
 */
int parse_record(char* input_record, s_record_t *s_record)
{
    char* byte_pointer;
    int state = S_STATE;
    byte_t sum = 0x00;
    int return_status = 0;
    int byte = 0;

    byte_pointer = input_record;

    /* Check for Null pointers */
    if(input_record == NULL || s_record == NULL)
    {
        state = END_STATE;
        return_status = -1;
    }

    while(state != END_STATE)
    {
        switch (state)
        {
        case S_STATE:
            if(*byte_pointer != 'S')
            {
                /* Input record is not an s-record */
                return_status = -2;
                state = END_STATE;
            }
            else
            {
                /* Increment pointer and move to next state */
                *byte_pointer++;
                state++;
            }
            break;

        case TYPE_STATE:
            if(!(IS_VALID_TYPE(*byte_pointer)))
            {
                /* Invalid Record Type */
                return_status = -3;
                state = END_STATE;
            }
            else
            {
                /* Assign Record Type to struct */
                s_record->type = *byte_pointer - '0';
                /* Increment pointer and move to next state */
                *byte_pointer++;
                state++;
            }
            break;

        case LENGTH_STATE:
            byte = Read_Byte(byte_pointer);
            printf("Length: %02x\n", byte);
            if(byte > MAX_RECORD_LENGTH)
            {
                /* Record too long */
                return_status = -4;
                state = END_STATE;
            }
            else
            {
                /* Assign length to struct */
                s_record->length = (byte_t) byte;
                /* Add byte to sum */
                sum += (byte_t) byte;
                /* Move to next state */
                state++;
            }            
            break;

        case ADDRESS_STATE:
            printf("Address: ");
            for (int i = 0; i < ADDRESS_LENGTH; i++)
            {
                byte = Read_Byte(byte_pointer);
                printf("%02x", byte);
                /* Read Address byte into 0-indexed address and increment pointer */
                s_record->address[i] = (byte_t) byte;
                /* Add byte to sum */
                sum += (byte_t) byte;
            }
            /* Move to next state */
            printf("\n");
            state++;
            break;

        case DATA_STATE:
            /* Assign Data to struct until length met - account for address and checksum */
            printf("Data: ");
            for (int i = ADDRESS_LENGTH; i < s_record->length - 1; i++)
            {
                byte = Read_Byte(byte_pointer);
                printf("%02x", byte);
                /* Store Data in 0-indexed Data Array */
                s_record->data[i - ADDRESS_LENGTH] = (byte_t) byte;
                /* Add byte to sum and increment pointer */
                sum += (byte_t) byte;
            }
            /* Move to next state */
            printf("\n");
            state++;
            break;

        case CHECKSUM_STATE:
            byte = Read_Byte(byte_pointer);
            printf("Checksum: %02x\n", byte);
            /* Add byte to sum */
            sum += (byte_t) byte;
            printf("Sum: %02x\n", sum);
            /* Check if checksum is correct */
            if(sum != CHECKSUM_VALUE)
            {
                /* Incorrect checksum, something went wrong */
                return_status = -5;
            }
            /* Move to next state */
            state++;
            break;
        
        default:
        /* Impossible State */
        printf("Invalid State: %d", state);
        return_status = -6;
        state = END_STATE;
            break;
        }
    }

    return return_status;
}
