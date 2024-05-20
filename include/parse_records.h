/**
 * @file parse_records.h
 * @brief Parsing S-Records
 * 
 * @author Zach Fraser
 * @date 2024-05-18
 */
#ifndef FILE_H
#define FILE_H

#include <stdint.h>
#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#define byte_t unsigned char
#define MAX_RECORD_LENGTH 32

#define ADDRESS_LENGTH 2 /* 2 Byte Memory Addressing */
#define CHECKSUM_LENGTH 1
#define CHECKSUM_VALUE 0xFF /* Expected value of sum of record bytes */

/* Returns true if type is a valid record type */
#define IS_VALID_TYPE(type) \
                    type == NAME_TYPE || \
                    type == INSTRUCTION_TYPE || \
                    type == DATA_TYPE || \
                    type == ADDRESS_TYPE\

/* Read two characters and interpret them as a single byte. Increases character pointer */
#define Read_Byte(character_pointer) ( \
    ((isdigit(*character_pointer) ? (*character_pointer++ - '0') \
    : (toupper(*character_pointer++) - 'A' + 10)) << 4) + \
    \
    (isdigit(*character_pointer) ? (*character_pointer++ - '0') \
    : (toupper(*character_pointer++) - 'A' + 10)) \
)

/* Valid Record Types - Stored as Characters*/
enum
{
NAME_TYPE           = '0',
INSTRUCTION_TYPE    = '1',
DATA_TYPE           = '2',
ADDRESS_TYPE        = '9'
};

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

/* S-Record Struct */
typedef struct s_record_t
{
byte_t type;
byte_t length;
byte_t address[2];
byte_t data[MAX_RECORD_LENGTH];
}s_record_t;

int parse_record(char* input_record, s_record_t *s_record);

#endif

