#include <stdio.h>  // Include standard input output header
#include <string.h> // Include string header
#include "encode.h" // Include programmer defined "encode" header
#include "types.h"  // Include programmer defined "types" header
#include "common.h" // Include programmer defined "common" header

/* Function Definitions */

/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */
uint get_image_size_for_bmp(FILE *fptr_image) // Function to find size of source file
{
    uint width, height; // Declare width, height

    fseek(fptr_image, 18, SEEK_SET); // Seek to 18th byte

    fread(&width, sizeof(int), 1, fptr_image); // Read the width (an int) ---> 1024 ---> 24 position

    fread(&height, sizeof(int), 1, fptr_image); // Read the height (an int) ---> 768 ---> 26 position
    
    // printf("height = %d\n", height);

    return (width * height * 3) + 55; // Return image capacity, here 55 bytes
}

/*
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */
Status open_files(EncodeInfo *encInfo) // Function used to open all files needed for encoding purpose
{
    printf("\033[1;97mINFO: Opening required files\n");
    // Src Image file
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r"); // Open source file ex: beautiful.bmp in read mode
    // Do Error handling
    if (encInfo->fptr_src_image == NULL)
    {
        perror("fopen");                                                                               // print a system error message
        fprintf(stderr, "\033[1;91mERROR: Unable to open file %s\033[0m\n", encInfo->src_image_fname); // print error message
        return e_failure;                                                                              // return e_failure
    }
    printf("INFO: Opened %s\n", encInfo->src_image_fname);
    // Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r"); // Open secret file ex: secret.txt in read mode
    // Do Error handling
    if (encInfo->fptr_secret == NULL)
    {
        perror("fopen");                                                                            // print a system error message
        fprintf(stderr, "\033[1;91mERROR: Unable to open file %s\033[0m\n", encInfo->secret_fname); // print error message
        return e_failure;                                                                           // return e_failure
    }
    printf("INFO: Opened %s\n", encInfo->secret_fname);
    // Stego Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w"); // Open output file in write mode
    // Do Error handling
    if (encInfo->fptr_stego_image == NULL)
    {
        perror("fopen");                                                                                 // print a system error message
        fprintf(stderr, "\033[1;91mERROR: Unable to open file %s\033[0m\n", encInfo->stego_image_fname); // print error message
        return e_failure;                                                                                // return e_failure
    }
    printf("INFO: Opened %s\n", encInfo->stego_image_fname);

    return e_success; // No failure return e_success
}

Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo) // Function used read validate for only for encoding arguments
{
    if (argv[2][0] != '.') // Validate first source file doesn't start with '.'
    {
        if (strstr(argv[2], ".bmp")) // Validate whether ".bmp" present in source file
        {
            /*Step 2: Store sourece filename*/
            // Validate only ".bmp" present at the last of the filename ex: don't consider beautiful.bmpabc, consider only beautiful.bmp
            char str[50];                 // Declare str of size 50
            strcpy(str, argv[2]);         // copy argv[2] to str
            char *ret = strtok(str, "."); // divide str and store name before '.' occurs
            ret = strtok(NULL, ".");      // store name after '.'
            int i = 0;
            while (ret[i]) // Loop to check only ".bmp" present at the last of the filename ex: don't consider beautiful.bmpabc, consider only beautiful.bmp
            {
                if (ret[i] == 'p' && ret[i + 1] != '\0')
                {
                    printf("\033[1;91mERROR: \033[1;97mInvalid source file without .bmp extension\n");
                    return e_failure; // return e_failure
                }
                i++; // Increment i
            }
            encInfo->src_image_fname = argv[2]; // copy source filename to encInfo->src_image_fname
        }
        else
        {
            printf("\033[1;91mERROR: \033[1;97mInvalid source file without .bmp extension\n");
            return e_failure; // If file not contains .bmp
        }
    }
    else
    {
        printf("\033[1;91mERROR: \033[1;97mInvalid source file without filename\n");
        return e_failure; // If file startsh with '.'
    }
    char *extn[4] = {".txt", ".c", ".h", ".s"}; /* Complete this first */
    int i = 0;
    strcpy(encInfo->extn_secret_file, "0");
    while (i < 4)
    {
        if (argv[3][0] != '.')
        {
            /*Implement for .c .h .sh*/
            if (strstr(argv[3], extn[i]))
            {
                /*Step 2: Store sourece filename*/
                encInfo->secret_fname = argv[3];            // store secret file name to encInfo->secret_fname
                strcpy(encInfo->extn_secret_file, extn[i]); // Store extension to encInfo->extn_secret_file
            }
        }
        else
        {
            printf("\033[1;91mERROR: \033[1;97mInvalid secret file extension\n");
            return e_failure; // If file name starts with '.'
        }
        i++;
    }

    if (strcmp(encInfo->extn_secret_file, "0") == 0) // Validate if user passed secret file without extension
    {
        printf("\033[1;91mERROR: \033[1;97mInvalid secret file without extension\n");
        return e_failure; // return e_failure
    }

    if (argv[4] == NULL)
    {
        printf("\033[1;91mINFO: Output File not mentioned. Creating steged_img.bmp as default\n");
        // Create own default file name .bmp
        encInfo->stego_image_fname = "stego_imag.bmp"; // Store default output filename "stego_imag.bmp" to encInfo->stego_image_fname
    }
    else
    {
        if (argv[4][0] != '.') // Validate output file doesn't start with '.'
        {
            if (strstr(argv[4], ".bmp")) // Validate whether ".bmp" present in output file
            {
                /*Step 2: Store sourece filename*/
                // Validate only ".bmp" present at the last of the filename ex: don't consider beautiful.bmpabc, consider only beautiful.bmp
                char str[50];                 // Declare str of size 50
                strcpy(str, argv[4]);         // copy argv[2] to str
                char *ret = strtok(str, "."); // divide str and store name before '.' occurs
                ret = strtok(NULL, ".");      // store name after '.'
                int i = 0;
                while (ret[i]) // Loop to check only ".bmp" present at the last of the filename ex: don't consider beautiful.bmpabc, consider only beautiful.bmp
                {
                    if (ret[i] == 'p' && ret[i + 1] != '\0')
                    {
                        printf("\033[1;91mERROR: \033[1;97mInvalid source file without .bmp extension\n");
                        return e_failure; // return e_failure
                    }
                    i++; // Increment i
                }
                encInfo->stego_image_fname = argv[4]; // copy source filename to encInfo->src_image_fname
            }
            else
            {
                printf("\033[1;91mERROR: \033[1;97mInvalid output file extension\n");
                return e_failure; // If file not contains .bmp return e_failure
            }
        }
        else
        {
            printf("\033[1;91mERROR: \033[1;97mInvalid output file without filename\n");
            return e_failure; // If file starts with '.' return e_failure
        }
    }
    return e_success; // Return e_success if all conditions are true
}

uint get_file_size(FILE *fptr)
{
    fseek(fptr, 0, SEEK_END); // Seek to end of file
    return ftell(fptr);       // return position of last character present in file(position of EOF)
}

Status check_capacity(EncodeInfo *encInfo) // Function to check whether the source file as capacity to store secret file data
{
    encInfo->image_capacity = get_image_size_for_bmp(encInfo->fptr_src_image); // Store size of source file (beautiful.bmp file --> 2359351) to store image_capacity
    printf("INFO: Checking for %s size\n", encInfo->secret_fname);
    encInfo->size_secret_file = get_file_size(encInfo->fptr_secret); // Store size of secret file to encInfo->size_secret_file
    if (encInfo->size_secret_file < 1)                               // Check whether the secret file is not an empty file
    {
        printf("\033[1;91mERROR: \033[1;97mEntered secret file doesn't contain any data\n");
        return e_failure; // return e_failure
    }
    printf("INFO: Done. Not Empty\n");
    encInfo->copied_capacity = ((strlen(MAGIC_STRING) + 4 + strlen(encInfo->extn_secret_file) + 4 + encInfo->size_secret_file) * 8) + 54; // Store size of content that should be encoded in source file to encInfo->copied_capacity
    printf("INFO: Checking for beautiful.bmp capacity to handle secret.txt\n");
    if (encInfo->image_capacity > encInfo->copied_capacity) // Check capacity
    {
        return e_success; // No failure return e_success
    }
    return e_failure; // return e_failure
}

Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image) // Copy 54 bytes of header Info of sour
{
    printf("INFO: Copying Image Header\n");
    char copy[54];                                       // Declare copy of size 54 bytes
    rewind(fptr_src_image);                              // Rewind the position
    fread(copy, 1, 54, fptr_src_image);                  // Read 54 bytes from source file
    fwrite(copy, 1, 54, fptr_dest_image);                // Write 54 bytes to output file
    if (ftell(fptr_src_image) == ftell(fptr_dest_image)) // If all the header content is copied successfully
    {
        return e_success; // No failure return e_success
    }
    return e_failure;
}

Status encode_int_to_lsb(int size, char *image_buffer) // Function to encode integer ---> 32bytes using bitwise operation
{
    for (int i = 31; i >= 0; i--) // Loop to encode int to lsb of 32 bytes
    {
        image_buffer[31 - i] = (image_buffer[31 - i] & (~1)) | ((size >> i) & 1); //  encode int to lsb of 32 bytes
    }
    return e_success; // No failure return e_success
}

Status encode_byte_to_lsb(char data /* 8bytes of data*/, char *image_buffer) // Encode character ---> 8bytes using bitwise operations & | ^ ~ << >> for int collect int data
{
    for (int i = 7; i >= 0; i--) // Loop to encode int to lsb of 8 bytes
    {
        image_buffer[7 - i] = (image_buffer[7 - i] & (~1)) | ((data >> i) & 1); //  encode int to lsb of 8 bytes
    }
    return e_success; // No failure return e_success
}

Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo) // Function to encode magic string
{
    printf("INFO: Encoding Magic String Signature\n");
    char arr[8];                                   // Declare the arr with size 8.
    for (int i = 0; i < strlen(MAGIC_STRING); i++) // Run the loop strlen(magic_string)
    {
        fread(arr, 1, 8, encInfo->fptr_src_image);    // Read the 8 byte of data from src file.
        encode_byte_to_lsb(magic_string[i], arr);     // Call the encode_byte_to_lsb(magic_string[i], arr);
        fwrite(arr, 1, 8, encInfo->fptr_stego_image); // Write the 8 byte of data in dest file.
    }
    if (ftell(encInfo->fptr_src_image) == ftell(encInfo->fptr_stego_image)) // If encoding is done is done successfully
    {
        return e_success; // No failure return e_success
    }
    return e_failure; // return e_failure
}

Status encode_secret_file_extn_size(int size, EncodeInfo *encInfo) // Function to encode size of secret file extension size
{
    char arr[32];                                                           // Declare the arr with size 32
    fread(arr, 1, 32, encInfo->fptr_src_image);                             // Read the 32 byte of data from src file
    encode_int_to_lsb(size, arr);                                           // Call the encode_int_to_lsb(strlen(encInfo -> extn_secret_file), arr)
    fwrite(arr, 1, 32, encInfo->fptr_stego_image);                          // Write the 32 byte data in dest file.
    if (ftell(encInfo->fptr_src_image) == ftell(encInfo->fptr_stego_image)) // If encoding is done is done successfully
    {
        return e_success; // No failure return e_success
    }
    return e_failure; // return e_failure
}

Status encode_secret_file_extn(const char *file_extn, EncodeInfo *encInfo) // Function to encode size of secret file extension
{
    printf("INFO: Encoding %s File Extenstion\n", encInfo->secret_fname);
    char arr[8];                                // Declare the arr with size 8.
    for (int i = 0; i < strlen(file_extn); i++) // Run the loop strlen(file_extn)
    {
        fread(arr, 1, 8, encInfo->fptr_src_image);    // Read the 8 byte of data from src file.
        encode_byte_to_lsb(file_extn[i], arr);        // Call the encode_byte_to_lsb(file_extn[i], arr);
        fwrite(arr, 1, 8, encInfo->fptr_stego_image); // Write the 8 byte of data in dest file.
    }
    if (ftell(encInfo->fptr_src_image) == ftell(encInfo->fptr_stego_image)) // If encoding is done is done successfully
    {
        return e_success; // No failure return e_success
    }
    return e_failure; // return e_failure
}
Status encode_secret_file_size(long file_size, EncodeInfo *encInfo) // Function to encode secret file size
{
    printf("INFO: Encoding secret.txt File Size\n");
    char arr[32];                                                           // Delcrae the arr with size 32
    fread(arr, 1, 32, encInfo->fptr_src_image);                             // Read the 32 byte of data from src file
    encode_int_to_lsb(file_size, arr);                                      // Call the encode_int_to_lsb(file_size), arr)
    fwrite(arr, 1, 32, encInfo->fptr_stego_image);                          // Write the 32 byte data in dest file.
    if (ftell(encInfo->fptr_src_image) == ftell(encInfo->fptr_stego_image)) // If encoding is done is done successfully
    {
        return e_success; // No failure return e_success
    }
    return e_failure; // return e_failure
}
Status encode_secret_file_data(EncodeInfo *encInfo) // Function to encode secret file data
{
    printf("INFO: Encoding secret.txt File Data\n");
    rewind(encInfo->fptr_secret);                       // Rewind for fptr_secret
    char arr[8];                                        // Declare the arr with size 8.
    for (int i = 0; i < encInfo->size_secret_file; i++) // Run the loop encInfo -> size_secret_file
    {
        fread(encInfo->secret_data, 1, 1, encInfo->fptr_secret); // fread(encInfo -> secret_data, 1, 1, encInfo -> fptr_secret)
        fread(arr, 1, 8, encInfo->fptr_src_image);               // Read the 8 byte of data from src file.
        encode_byte_to_lsb(encInfo->secret_data[0], arr);        // Call the encode_byte_to_lsb(encInfo -> secret_data[0], arr);
        fwrite(arr, 1, 8, encInfo->fptr_stego_image);            // Write the 8 byte of data in dest file.
    }
    if (ftell(encInfo->fptr_src_image) == ftell(encInfo->fptr_stego_image)) // * If all the header content is copied successfully ---> use for remaining upcoming files
    {
        return e_success; // No failure return e_success
    }
    return e_failure; // return e_failure
}

Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest, long size) // Function to copy remaining data to of source to destination
{
    printf("INFO: Copying Left Over Data\n");
    char arr[size];
    int i = size;
    fread(arr, size, 1, fptr_src);   // fread(____, size, 1, fptr_src);
    fwrite(arr, size, 1, fptr_dest); // fwrite(____,1 ,fptr_dest);
    i++;
    if (ftell(fptr_src) == ftell(fptr_dest)) // * If all the header content is copied successfully ---> use for remaining upcoming files
    {
        return e_success; // No failure return e_success
    }
    return e_failure; // return e_failure
}

Status do_encoding(EncodeInfo *encInfo) // Function used to do all remaining function calls inside --> last func called in main function
{
    if (open_files(encInfo) == e_failure) // Call open files ---> all neccessary files to access its contents
    {
        return e_failure; // If couldn't open files return e_failure
    }
    printf("INFO: Done\n");
    printf("INFO: ## Encoding Procedure Started ##\n");
    if (check_capacity(encInfo) == e_failure) // Call check capacity ---> Implement
    {
        return e_failure; // return e_failure
    }
    printf("INFO: Done. Found OK\n");
    if (copy_bmp_header(encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_failure)
    {
        return e_failure; // return e_failure
    }
    printf("INFO: Done\n");
    if (encode_magic_string(MAGIC_STRING, encInfo) == e_failure) // Encode magic string one by one character
    {
        /*From here encoding starts byte to LSB*/
        return e_failure; // return e_failure
    }
    printf("INFO: Done\n");
    if (encode_secret_file_extn_size(strlen(encInfo->extn_secret_file), encInfo) == e_failure)
    {
        return e_failure; // return e_failure
    }
    if (encode_secret_file_extn(encInfo->extn_secret_file, encInfo) == e_failure)
    {
        return e_failure; // return e_failure
    }
    printf("INFO: Done\n");
    if (encode_secret_file_size(encInfo->size_secret_file, encInfo) == e_failure)
    {
        return e_failure; // return e_failure
    }
    printf("INFO: Done\n");
    if (encode_secret_file_data(encInfo) == e_failure)
    {
        return e_failure; // return e_failure
    }
    printf("INFO: Done\n");

    long size = encInfo->image_capacity - encInfo->copied_capacity; // Declare size and initialize with value (ex: 23,59,351 - 366 = 23,58,985 --> copy remaining "23,58,985" data to dest file from src file)

    if (copy_remaining_img_data(encInfo->fptr_src_image, encInfo->fptr_stego_image, size) == e_failure) // call co
    {
        return e_failure; // return e_failure
    }
    printf("INFO: Done\n");
    printf("INFO: ## Encoding Done Successfully ##\n");
}
