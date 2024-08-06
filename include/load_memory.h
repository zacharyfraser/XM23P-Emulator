/**
 * @file load_memory.h
 * @brief Load program instructions and data into memory - 
 *                                      Architecture agnostic
 * 
 * @author Zach Fraser
 * @date 2024-05-19
 */

#ifndef LOAD_MEMORY_H
#define LOAD_MEMORY_H

#include <stddef.h>

#include "definitions.h"

/* Returns true if type is a valid record type */
#define IS_VALID_TYPE(type) \
                    type == NAME_TYPE || \
                    type == INSTRUCTION_TYPE || \
                    type == DATA_TYPE || \
                    type == ADDRESS_TYPE\

/* S_Record States*/
enum
{
    S_STATE,
    TYPE_STATE,
    LENGTH_STATE,
    ADDRESS_STATE,
    DATA_STATE,
    CHECKSUM_STATE,
    END_STATE
};

/* Function Prototypes */
int initialize_register_file(word_t register_file[CONSTANT_SELECT][REGISTER_FILE_LENGTH]);
int parse_record(char *input_record, s_record_t *s_record);
int copy_byte_array(
    byte_t *source, byte_t *destination, int length, int offset);
int load_record_address(s_record_t *s_record, int *destination);
int load_record_data(s_record_t *s_record, byte_t *destination);
int load_record_name(s_record_t *s_record, byte_t *destination);

#endif