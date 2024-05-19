/**
 * @file load_memory.c
 * @brief load record arrays into memory arrays
 * 
 * @author Zach Fraser
 * @date 2024-05-19
 */

#include "parse_records.h"
#include "load_memory.h"

/**
 * @brief Copy source array to destination
 * 
 * @param source Pointer to source array
 * @param destination Pointer to destination array
 * @param length Number of bytes in array
 * @return int [0 = Success, < 0 = Failure]
 */
int copy_byte_array(byte_t *source, byte_t *destination, int length)
{
    int error_status = 0;
    for (int i = 0; i < length; i++)
    {
        if(source != NULL && destination != NULL)
        {
            *source++ = *destination++;
        }
        else
        {
            /* Source or destination array was smaller than length */
            error_status = -1;
        }
    }

    return error_status;
}

/**
 * @brief Copy record address member to destination array
 * 
 * @param s_record Pointer to S_Record
 * @param destination Pointer to destination array
 * @return int [0 = Success, < 0 = Failure]
 */
int load_record_address(s_record_t *s_record, byte_t *destination)
{
    return copy_byte_array(s_record->address, destination, ADDRESS_LENGTH);
}

/**
 * @brief Copy record data member to destination array
 * 
 * @param s_record Pointer to S_Record
 * @param destination Pointer to destination array
 * @return int [0 = Success, < 0 = Failure]
 */
int load_record_data(s_record_t *s_record, byte_t *destination)
{
    return copy_byte_array(s_record->data, destination, s_record->length);
}