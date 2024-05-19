/**
 * @file parse_records.h
 * @brief Parsing S-Records
 * 
 * @author Zach Fraser
 * @date 2024-05-18
 */
#ifndef FILE_H
#define FILE_H

#define byte_t unsigned char
#define MAX_RECORD_LENGTH 32

#define ADDRESS_LENGTH 2 /* 2 Byte Memory Addressing */
#define CHECKSUM_VALUE 0xFF /* Expected value of sum of record bytes */

/* Returns true if type is a valid record type */
#define IS_VALID_TYPE(type) \
                    type == NAME_TYPE || \
                    type == INSTRUCTION_TYPE || \
                    type == DATA_TYPE || \
                    type == ADDRESS_TYPE\
                            

/* Valid Record Types */
enum
{
NAME_TYPE = 0,
INSTRUCTION_TYPE = 1,
DATA_TYPE = 2,
ADDRESS_TYPE = 9
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
byte_t address[4];
byte_t data[MAX_RECORD_LENGTH];
}s_record_t;

int parse_record(char* input_record, s_record_t *s_record);

#endif

