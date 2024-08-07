/**
 * @file display_memory.c
 * @brief Utilities for displaying memory
 * 
 * @author Zach Fraser
 * @date 2024-05-22
 */

/**
 * @brief Display memory in addresses between bounds
 * 
 * @param memory_array Memory array to be displayed
 * @param min_address Minimum address bound
 * @param max_address Maximum address bound
 * @return int [0 = SUCCESS, < 0 = FAILURE]
 */

#include "display_memory.h"

/**
 * @brief Convert word stored as a 2-byte array to an integer
 * 
 * @param word Pointer to 2-byte array
 * @param integer Pointer to destination integer
 * @return int [0 = SUCCESS, <0 = FAILURE]
 */
int convert_word_to_int(byte_t *word, int *integer)
{
    int error_status = 0;
    if(word != NULL)
    {
        *integer = (*word++ << 8);
        if(word != NULL)
        {
            *integer |= *word;
        }
        else
        {
            error_status = -1;
        }
        
    }
    else
    {
        error_status = -2;
    }
    
    return error_status;
}

/**
 * @brief Display memory in addresses between bounds
 * 
 * @param memory_array Memory array to be displayed
 * @param min_address Minimum address bound
 * @param max_address Maximum address bound
 * @return int [0 = SUCCESS, < 0 = FAILURE]
 */
int display_memory(byte_t *memory_array,  int min_address, int max_address)
{
    int error_status = 0;
    int location_counter = min_address;

    /* Valid Address Checking */
    if(max_address > DATA_MEMORY_LENGTH)
    {
        error_status = -1;
    }

    if(min_address < 0)
    {
        error_status = -2;
    }

    /* Display Memory */
    while(location_counter < max_address)
    {
        printf("#%04x\t", location_counter);
        for(int i = 0; i < BYTES_PER_LINE; i++)
        {
            printf("%02x ", memory_array[location_counter + i]);
        }
        printf("\n");
        location_counter += BYTES_PER_LINE;
    }

    return error_status;\
}