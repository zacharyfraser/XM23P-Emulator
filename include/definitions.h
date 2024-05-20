/**
 * @file definitions.h
 * @brief Type and constant definitions common to the project
 * 
 * @author Zach Fraser
 * @date 2024-05-20
 */

#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <ctype.h>

#define KILOBYTE (1 << 10) /* 1024 bytes */
#define byte_t unsigned char
#define NUL '\0'

#define MAX_RECORD_LENGTH 32

#define ADDRESS_LENGTH 2 /* 2 Byte Memory Addressing */
#define CHECKSUM_LENGTH 1
#define CHECKSUM_VALUE 0xFF /* Expected value of sum of record bytes */

/* Read two characters and interpret them as a single byte. 
                                Increases character pointer */
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

/* S-Record Struct */
typedef struct s_record_t
{
byte_t type;
byte_t length;
byte_t address[2];
byte_t data[MAX_RECORD_LENGTH];
}s_record_t;

#endif