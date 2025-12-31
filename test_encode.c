/*
┌───────────────────────────┐
│ PROJECT: LSB STEGNOGRAPHY │
│ NAME: MANU H P            │
│ START DATE: 06/11/2025    │
│ END DATE: 12/11/2025      │
└───────────────────────────┘
*/
#include <stdio.h>  // Include standard input output header
#include <string.h> // Include string header
#include "encode.h" // Include programmer defined "encode" header
#include "decode.h" // Include programmer defined "decode" header
#include "types.h"  // Include programmer defined "types" header

OperationType check_operation_type(char *argv[]) // Function to check operation type encodeing ---> "-e" or decoding ---> "-d"
{

    if (argv[1] == NULL) // If user didn't pass any operation type
    {
        return e_unsupported; // return e_unsupported
    }
    if (strcmp(argv[1], "-e") == 0) // If user passed operation type "-e"
    {
        return e_encode; // return e_encode
    }
    else if (strcmp(argv[1], "-d") == 0) // If user passed operation type "-d"
    {
        return e_decode; // return e_descode
    }
    else
    {
        return e_unsupported; // If user passed operation type other than "-e" or "-d", return e_unsupported
    }
}

int main(int argc, char *argv[]) // main function collecting inputs through command line arguments
{
    int ret = check_operation_type(argv); // Collect return value from check_operation_type() to ret
    if (ret == 0)                         // If check_operation_type() returns e_encode, i.e., 0
    {
        EncodeInfo encInfo; // Declare "encInfo" variable of type structure "EncodeInfo"
        if (argc >= 4)      // If argument count is greater than or equal to four
        {
            if (read_and_validate_encode_args(argv, &encInfo) == e_failure) // Read validate for only for encoding arguments
            { 
                return 0; // return 0 if read_and_validate_encode_args() is not done successfully
            }
            else
            {
                do_encoding(&encInfo); // call do_encoding if read_and_validate_decode_args() is done successfully
            }
        }
        else
        {
            // Error message
            printf("\033[1;97m%s: \033[1;91mEncoding:\033[0m \033[1;97m%s -e <.bmp file> <.txt file> [output file]\n", argv[0], argv[0]);
        }
    }
    else if (ret == 1) // If check_operation_type() returns e_decode, i.e., 1
    {
        DecodeInfo decInfo; // Declare "decInfo" variable of type structure "DecodeInfo"
        if (argc >= 3)      // If argument count is greater than or equal to three
        {
            if (read_and_validate_decode_args(argv, &decInfo) == e_failure) // Read validate for only for decoding arguments
            {
                return 0; // return 0 if read_and_validate_decode_args() is not done successfully
            }
            else
            {
                do_decoding(&decInfo); // call do_decoding if read_and_validate_decode_args() is done successfully
            }
        }
        else
        {
            // Error message
            printf("\033[1;97m%s: \033[1;91mDecoding:\033[0m \033[1;97m%s -d <.bmp file> [output file]\n", argv[0], argv[0]);
        }
    }
    else // If check_operation_type() returns e_unsupported, i.e., 3 print error message
    {
        // Error message if
        printf("\033[1;97m%s: \033[1;91mEncoding:\033[0m \033[1;97m%s -e <.bmp file> <.txt file> [output file]\n", argv[0], argv[0]); // Print error message here
        printf("\033[1;97m%s: \033[1;91mDecoding:\033[0m \033[1;97m%s -d <.bmp file> [output file]\n", argv[0], argv[0]);             // Print error message here
        return 0;
    }
    return 0;
}