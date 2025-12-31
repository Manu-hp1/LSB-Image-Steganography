#ifndef DECODE_H // The preprocessor directive #ifndef(if not defined) ---> Checks if DECODE_H macro previously present or not: If not present, code is processed else code below till #endif is ignored
#define DECODE_H // defines the macro DECODE_H

#include "types.h" // Contains user defined types

#define MAX_DECODED_DATA_SIZE 1000 // Define MAX_DECODED_DATA_SIZE ----> 1000
#define MAX_DECODE_FILE_SUFFIX 5 // Maximum extension size to be stored

typedef struct _DecodeInfo
{
  /* Source Image info */
  char *dest_image_fname; // Declare character pointer "dest_image_fname" to store dest fiename   ---> ex: stego_imag.bmp;
  FILE *fptr_dest_image;  // Declare file pointer "fptr_dest_image" ---> store address of stego_imag.bmp file

  /* Secret File Info */

  char *decode_secret_fname;                            // Declare character pointer "decode_secret_fname" to store secret fiename ---> ex: decoded.txt
  FILE *fptr_decode_secret;                             // Declare file pointer "fptr_decode_secret" ---> store address of decoded.txt file
  int extn_size;                                        // Declare extn_size to store size of secret file extension
  char extn_decode_secret_file[MAX_DECODE_FILE_SUFFIX]; // Declare extn_decode_secret_file array with size "MAX_DECODE_FILE_SUFFIX" to store secrete file extension (.txt .bmp .h .c)
  char decode_secret_data[MAX_DECODED_DATA_SIZE];       // Declare decode_secret_data array with size "MAX_DECODED_DATA_SIZE" to store 1 by 1 character for encoding process and go for next character bcoz we don't know about the size, so we're going with this
  long size_decode_secret_file;                         // Declare long integer size_decode_secret_file, to store the size of secret file

} DecodeInfo; // Alternate for struct _DecodeInfo

/* Encoding function prototype */

/* Read and validate Decode args from argv */
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo); // Function used read validate for only for decoding arguments

/* Perform the encoding */
Status do_decoding(DecodeInfo *decInfo); // Function used to do all remaining function calls inside --> last function called in main function

/* Get File pointers for i/p and o/p files */
Status open_decoding_dest_file(DecodeInfo *decInfo); // Function used to open all files needed for decoding

Status open_decoding_secret_file(DecodeInfo *decInfo); // Function used to open secret file needed for decoding

/* Store Magic String */
Status decode_magic_string(const char *magic_string, DecodeInfo *decInfo); // Function to decode magic string

Status decode_secret_file_extn_size(int *size, DecodeInfo *decInfo);  // Function to decode size of secret file extension size

/* Encode secret file extenstion */
Status decode_secret_file_extn(char *file_extn, DecodeInfo *decInfo); // Function to decode size of secret file extension 

/* Encode secret file size */
Status decode_secret_file_size(long *file_size, DecodeInfo *decInfo); // Function to decode secret file size

/* Encode secret file data*/
Status decode_secret_file_data(DecodeInfo *decInfo); // Function to decode secret file data

/* Encode function, which does the real encoding */
Status decode_int_from_lsb(int *size, char *image_buffer); // Function to decode integer ---> 32bytes using bitwise operation

/* Encode a byte into LSB of image data array */
Status decode_byte_from_lsb(char *data , char *image_buffer);  // Function to decode character ---> 8bytes using bitwise operations(OPERATORS: & | ^ ~ << >>)

#endif // End of header guard means it is used to end conditional preprocessor directives like #if condtion, #ifdef macro, #ifndef macro
// The #endif directive's purpose is to signal where the conditional block ends, similar to how a closing curly brace } ends a regular C if statement.
