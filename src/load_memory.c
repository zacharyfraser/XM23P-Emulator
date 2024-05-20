/**
 * @file load_memory.c
 * @brief load record arrays into memory arrays
 * 
 * @author Zach Fraser
 * @date 2024-05-19
 */

#include "load_memory.h"

/**
 * @brief Copy source array to destination
 * 
 * @param source Pointer to source array
 * @param destination Pointer to destination array
 * @param length Number of bytes in array
 * @param offset Index of destination array to start copying
 * @return int [0 = Success, < 0 = Failure]
 */
int copy_byte_array(
    byte_t *source, byte_t *destination, int length, int offset)
{
    int error_status = 0;
    for (int i = 0; i < length; i++)
    {
        /* Move to starting address in array */
        if(source != NULL && destination != NULL)
        {
            destination[offset] = *source++;
            destination++;
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
    return copy_byte_array(s_record->address, destination, ADDRESS_LENGTH, 0);
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
    int memory_offset = 0;
    /* Convert Byte array to integer sum */
    for (int i = 0; i < ADDRESS_LENGTH; i++)
    {
        memory_offset += s_record->address[i] << 
            (8 * (ADDRESS_LENGTH - i - 1));
    }
    
    /* Data length = record length - length of address 
            - length of checksum checksum */
    return copy_byte_array
        (
        s_record->data, destination, 
        s_record->length - ADDRESS_LENGTH - CHECKSUM_LENGTH,
        memory_offset
        );
}

/**
 * @brief Copy record name member to destination array, add null terminator
 * 
 * @param s_record Pointer to S_Record
 * @param destination Pointer to destination array
 * @return int [0 = Success, < 0 = Failure]
 */
int load_record_name(s_record_t *s_record, byte_t *destination)
{
    int error_status = copy_byte_array
        (
        s_record->data, destination, 
        s_record->length - ADDRESS_LENGTH,
        0
        );

        if(!error_status)
        {
            /* This could throw an exception if the destination array is 
                    smaller than the index. */
            destination[
                s_record->length - ADDRESS_LENGTH - CHECKSUM_LENGTH] = NUL;
        }

    return error_status;
}

