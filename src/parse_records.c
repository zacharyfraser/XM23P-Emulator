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
    char* character_pointer;
    int state = S_STATE;
    byte_t sum = 0x00;
    int return_status = 0;
    int byte = 0;

    character_pointer = input_record;

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
            /* Check that first character is an 'S' */
            if(*character_pointer != 'S')
            {
                /* Input record is not an s-record */
                return_status = -2;
                state = END_STATE;
            }
            else
            {
                /* Increment pointer and move to next state */
                *character_pointer++;
                state++;
            }
            break;

        case TYPE_STATE:
            /* Check that next character is a valid type [0, 1, 2, 9] */
            if(!(IS_VALID_TYPE(*character_pointer)))
            {
                /* Invalid Record Type */
                return_status = -3;
                state = END_STATE;
            }
            else
            {
                /* Assign Record Type to struct */
                s_record->type = *character_pointer;
                /* Increment pointer and move to next state */
                *character_pointer++;
                state++;
            }
            break;

        case LENGTH_STATE:
            /* Read next two characters as a byte.  
                    Increments pointer 2 characters */
            byte = Read_Byte(character_pointer);
            /* Check if length is less than max record length */
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
            for (int i = 0; i < ADDRESS_LENGTH; i++)
            {
                /* Read next two characters as a byte.  
                        Increments pointer 2 characters */
                byte = Read_Byte(character_pointer);
                /* Read Address byte into address */
                s_record->address[i] = (byte_t) byte;
                /* Add byte to sum */
                sum += (byte_t) byte;
            }
            /* Move to next state */
            state++;
            break;

        case DATA_STATE:
            /* Assign Data to struct until length met - 
                        account for address and checksum */
            for (int i = ADDRESS_LENGTH; i < s_record->length - 1; i++)
            {
                /* Read next two characters as a byte.  
                        Increments pointer 2 characters */
                byte = Read_Byte(character_pointer);
                /* Store Data in 0-indexed Data Array */
                s_record->data[i - ADDRESS_LENGTH] = (byte_t) byte;
                /* Add byte to sum and increment pointer */
                sum += (byte_t) byte;
            }
            /* Move to next state */
            state++;
            break;

        case CHECKSUM_STATE:
            /* Read next two characters as a byte.  
                    Increments pointer 2 characters */
            byte = Read_Byte(character_pointer);
            /* Add byte to sum */
            sum += (byte_t) byte;
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
        /* Impossible State - Code Error */
        return_status = -6;
        state = END_STATE;
            break;
        }
    }

    /* Return Success (0) or Failure (< 0) */
    return return_status;
}
