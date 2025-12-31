#ifndef ENCODE_H // The preprocessor directive #ifndef(if not defined) ---> Checks if ENCODE_H macro previously present or not: If not present, code is processed else code below till #endif is ignored
#define ENCODE_H // defines the macro ENCODE_H

#include "types.h" // Contains user defined types

/*
 * Structure to store information required for
 * encoding secret file to source Image
 * Info about output and intermediate data is
 * also stored
 */

#define MAX_SECRET_BUF_SIZE 1                        // #define MAX_SECRET_BUF_SIZE to store 1 by 1 character for encoding process and go for next character bcoz we don't know about the size, so we're going with this
#define MAX_IMAGE_BUF_SIZE (MAX_SECRET_BUF_SIZE * 8) // Define MAX_IMAGE_BUF_SIZE (MAX_SECRET_BUF_SIZE * 8) ----> 1 * 8 = 8bytes
#define MAX_FILE_SUFFIX 4                            // Define MAX_FILE_SUFFIX with value 4 ----> Used for storing extension maximum extension size to be stored is 4

typedef struct _EncodeInfo
{
    /* Source Image info */
    char *src_image_fname; // Declare character pointer "src_image_fname" to store source fiename ---> ex: beautiful.bmp
    FILE *fptr_src_image;  // Declare file pointer "fptr_src_image" ---> store address of beautiful.bmp file
    uint image_capacity;   // Declare image_capacity to store size of source file (beautiful.bmp file --> 2359351)
    uint copied_capacity;  // Declare copied_capacity to store size of content that should be encoded in source file

    /* Secret File Info */
    char *secret_fname;                     // Declare character pointer "secret_fname" to store secret fiename ---> ex: secret.txt
    FILE *fptr_secret;                      // Declare file pointer "fptr_secret" ---> store address of secret.txt file
    char extn_secret_file[MAX_FILE_SUFFIX]; // Declare extn_secret_file array with size "MAX_FILE_SUFFIX" to store secrete file extension (.txt .bmp .h .c)
    char secret_data[MAX_SECRET_BUF_SIZE];  // Declare secret_data array with size "MAX_SECRET_BUF_SIZE" to store 1 by 1 character for encoding process and go for next character bcoz we don't know about the size, so we're going with this
    long size_secret_file;                  // Declare long integer size_secret_file, to store the size of secret file

    /* Stego Image Info */
    char *stego_image_fname; // Declare character pointer stego_image_fname ---> Used to store destination filename
    FILE *fptr_stego_image;  // Declare file pointer "fptr_stego_image" ---> store address of destination file

} EncodeInfo; // Alternate for struct _EncodeInfo

/* Encoding function prototype */

/* Read and validate Encode args from argv */
Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo); // Function used read validate for only for encoding arguments

/* Perform the encoding */
Status do_encoding(EncodeInfo *encInfo); // Function used to do all remaining function calls inside --> last function called in main function

/* Get File pointers for i/p and o/p files */
Status open_files(EncodeInfo *encInfo); // Function used to open all files needed for encoding purpose

/* check capacity */
Status check_capacity(EncodeInfo *encInfo); // Function to check whether the source file as capacity to store secret file data

/* Get image size */
uint get_image_size_for_bmp(FILE *fptr_image); // Function to find size of source file

/* Get file size */
uint get_file_size(FILE *fptr); // Function to get size of secret file

/* Copy bmp image header */
Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image); // Function to copy 54 bytes of data of header from source file to destination file

/* Store Magic String */
Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo); // Function to encode magic string

Status encode_secret_file_extn_size(int size, EncodeInfo *encInfo); // Function to encode size of secret file extension size

/* Encode secret file extenstion */
Status encode_secret_file_extn(const char *file_extn, EncodeInfo *encInfo); // Function to encode size of secret file extension

/* Encode secret file size */
Status encode_secret_file_size(long file_size, EncodeInfo *encInfo); // Function to encode secret file size

/* Encode secret file data*/
Status encode_secret_file_data(EncodeInfo *encInfo); // Function to encode secret file data

/* Encode function, which does the real encoding */
Status encode_int_to_lsb(int size, char *image_buffer); // Function to encode integer ---> 32bytes using bitwise operation

/* Encode a byte into LSB of image data array */
Status encode_byte_to_lsb(char data, char *image_buffer); // Function to encode character ---> 8bytes using bitwise operations(OPERATORS: & | ^ ~ << >>)

/* Copy remaining image bytes from src to stego image after encoding */
Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest, long size); // Fuction to copy remaining data from source file to destionation file

#endif // End of header guard means it is used to end conditional preprocessor directives like #if condtion, #ifdef macro, #ifndef macro
// The #endif directive's purpose is to signal where the conditional block ends, similar to how a closing curly brace } ends a regular C if statement.
