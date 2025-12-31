#ifndef COMMON_H // The preprocessor directive #ifndef(if not defined) ---> Checks if COMMON_H macro previously present or not: If not present, code is processed else code below till #endif is ignored
#define COMMON_H // defines the macro COMMON_H
// common to encode and decode
/* Magic string to identify whether stegged or not */
#define MAGIC_STRING "#*" // After copy of .bmp file data for 54 + 16 bytes Maximum 3times for entering pasword

#endif // End of header guard means it is used to end conditional preprocessor directives like #if condtion, #ifdef macro, #ifndef macro
// The #endif directive's purpose is to signal where the conditional block ends, similar to how a closing curly brace } ends a regular C if statement.
