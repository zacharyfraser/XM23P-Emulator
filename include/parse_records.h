/**
 * @file parse_records.h
 * @brief Parsing S-Records
 * 
 * @author Zach Fraser
 * @date 2024-05-18
 */
#ifndef PARSE_RECORDS_H
#define PARSE_RECORDS_H

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

int parse_record(char* input_record, s_record_t *s_record);

#endif

