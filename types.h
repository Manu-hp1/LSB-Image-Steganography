#ifndef TYPES_H    // The preprocessor directive #ifndef(if not defined) ---> Checks if TYPES_H macro previously present or not: If not present, code is processed else code below till #endif is ignored
#define TYPES_H    // defines the macro TYPES_H
#include <stdio.h> // Include standard input output header
/* User defined types */
typedef unsigned int uint;

/* Status will be used in fn. return type */
typedef enum // typedef used to give alternate name for enum here
{
    e_success, // e_success is first member of Status with default value 0
    e_failure  // e_failure is second member of Status with default value 1 with reference of previous member's value
} Status;      // Status is alternate name for this enum

typedef enum // typedef used to give alternate name for enum here
{
    e_encode,     // e_encode is first member of OperationType with default value 0
    e_decode,     // e_decode is second member of OperationType with default value 1 with reference of previous member's value
    e_unsupported // e_unsupported is second member of OperationType with default value 1 with reference of previous member's value
} OperationType;  // OperationType is alternate name for this enum

#endif // End of header guard means it is used to end conditional preprocessor directives like #if condtion, #ifdef macro, #ifndef macro
// The #endif directive's purpose is to signal where the conditional block ends, similar to how a closing curly brace } ends a regular C if statement.
