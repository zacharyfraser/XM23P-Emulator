/**
 * @file load_memory.c
 * @brief Load S-Records into Memory Arrays
 * 
 * @author Zach Fraser
 * @date 2024-05-19
 */

#include "load_memory.h"

/**
 * @brief Reset register file registers to 0, and constants to default values
 * 
 * @param register_file - Pointer to register file
 * @return int [0 = Success, < 0 = Failure]
 */
int initialize_register_file(word_t register_file[CONSTANT_SELECT][REGISTER_FILE_LENGTH])
{
    int error_status = 0;
    if(register_file != NULL)
    {
        /* Reset Registers */
        for (int i = 0; i < REGISTER_FILE_LENGTH; i++)
        {
            register_file[REGISTER][i] = 0x0000;
        }
        /* Reset Constants */
        register_file[CONSTANT][0] = 0x0000;
        register_file[CONSTANT][1] = 0x0001;
        register_file[CONSTANT][2] = 0x0002;
        register_file[CONSTANT][3] = 0x0004;
        register_file[CONSTANT][4] = 0x0008;
        register_file[CONSTANT][5] = 0x0010;
        register_file[CONSTANT][6] = 0x0020;
        register_file[CONSTANT][7] = 0xFFFF;
    }
    else
    {
        error_status = -1;
    }
    return error_status;
}

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
 * @brief Copy record address member from byte array to int
 * 
 * @param s_record Pointer to S_Record
 * @param destination Pointer to destination int
 * @return int [0 = Success, < 0 = Failure]
 */
int load_record_address(s_record_t *s_record, int *destination)
{
    int error_status = 0;

    if (s_record != NULL && destination != NULL) 
    {
        *destination = (s_record->address[0] << 8) | s_record->address[1];
    } else {
        /* Invalid input */
        error_status = -1;
    }
    return error_status;
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

