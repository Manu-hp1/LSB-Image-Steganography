#include "decode.h" // Include programmer defined "decode" header
#include "common.h" // Include programmer defined "common" header
#include "types.h"  // Include programmer defined "types" header
#include <stdio.h>  // Include standard input output header
#include <string.h> // Include string header
#include <stdlib.h> // Include standard library header

Status open_decoding_dest_file(DecodeInfo *decInfo) // Function used to open all files needed for decoding
{
  printf("INFO: Opened steged_beautiful.bmp\n");
  decInfo->fptr_dest_image = fopen(decInfo->dest_image_fname, "r"); // Use where u want acces decoded.bmp
  // Do Error handling
  if (decInfo->fptr_dest_image == NULL) // Do Error handling
  {
    perror("fopen");                                                                                    // print a system error message
    fprintf(stderr, "\033[1;91mERROR: \033[1;97mUnable to open file *%s\n", decInfo->dest_image_fname); // print error message
    return e_failure;                                                                                   // return e_failure
  }
  return e_success; // No failure, return success
}

Status open_decoding_secret_file(DecodeInfo *decInfo) // Function used to open secret file needed for decoding
{
  int i = 0;
  char str[100];                          // Declare str of size 100
  while (decInfo->decode_secret_fname[i]) // Loop to copy decInfo->decode_secret_fname to str character by character
  {
    str[i] = decInfo->decode_secret_fname[i]; // copy decInfo->decode_secret_fname to str character by character
    i++;
  }

  str[i] = '\0';                  // Store last charcter in str as '\0'
  char *token = strtok(str, "."); // Divide secret filname for validation purpose
  i = 0;
  while (token[i]) // Loop to store filename to str
  {
    str[i] = token[i]; // Store only filename of secret filename entered by user
    i++;
  }
  strcat(str, decInfo->extn_decode_secret_file); // Merge filename and extension decoded earlier
  decInfo->decode_secret_fname = str;            // Copy secret file name to decInfo->decode_secret_fname

  decInfo->fptr_decode_secret = fopen(decInfo->decode_secret_fname, "w"); // Open secret file now
  if (decInfo->fptr_decode_secret == NULL)                                // Do Error handling
  {
    perror("fopen");                                                                                      // // print a system error message
    fprintf(stderr, "\033[1;91mERROR: \033[1;97mUnable to open file %s\n", decInfo->decode_secret_fname); // print error message
    return e_failure;                                                                                     // return e_failure
  }
  printf("INFO: Opened secret_msg.txt\nINFO: Done. Opened all required files\n");
  return e_success; // No failure, return e_failure
}
Status read_and_validate_decode_args(char **argv, DecodeInfo *decInfo) // Function used read validate for only for decoding arguments
{
  if (argv[2][0] != '.')
  {
    if (strstr(argv[2], ".bmp"))
    {
      /*Step 2: Store sourece filename*/
      decInfo->dest_image_fname = argv[2];
    }
    else
    {
      printf("\033[1;91mERROR: \033[1;97mInvalid file extension\n");
      return e_failure; // If file not contains .bmp
    }
  }
  else
  {
    printf("\033[1;91mERROR: \033[1;97mInvalid filename, file extension without filename\n");
    return e_failure; // If file startsh with .
  }

  if (argv[3] == NULL)
  {
    // Create own default file name .bmp
    printf("\033[1;97mINFO: Output File not mentioned. Creating decoded.txt as default\n");
    decInfo->decode_secret_fname = "decoded"; // Store decoded to decInfo->decode_secret_fname
  }
  else
  {
    char *extn[4] = {".txt", ".c", ".h", ".s"}; /* Complete this first */
    int i = 0;
    while (i < 4)
    {
      if (argv[3][0] != '.')
      {
        /*Implement for .c .h .sh*/
        if (strstr(argv[3], extn[i]))
        {
          /*Step 2: Store sourece filename*/
          decInfo->decode_secret_fname = argv[3];
          strcpy(decInfo->extn_decode_secret_file, extn[i]);
        }
      }
      else
      {
        return e_failure; // If file name starts with '.'
      }
      i++;
    }
    // printf("decInfo->extn_decode_secret_file = %s\n", decInfo->extn_decode_secret_file);
    if (decInfo->extn_decode_secret_file[0] != '.')
    {
      printf("\033[1;97mPass output file with extension .c or .sh or .h or .txt\n");
      return e_failure;
    }
  }
  return e_success; // Return e_success if all conditions are true
}

Status decode_int_from_lsb(int *size, char *image_buffer) // Function to decode integer ---> 32bytes using bitwise operation
{
  *size = 0;                    // clear previous bits
  for (int i = 0; i <= 31; i++) // Loop to extract bits
  {
    *size = (*size << 1) | (image_buffer[i] & 1); // extract LSB and shift left
  }
  return e_success;
}

/* Encode a byte into LSB of image data array */
Status decode_byte_from_lsb(char *data /* 8bytes of data*/, char *image_buffer) // Function to decode character ---> 8bytes using bitwise operations(OPERATORS: & | ^ ~ << >>)
{
  *data = 0;                  // clear previous bits
  for (int i = 0; i < 8; i++) // Loop to extract bits
  {
    *data = (*data << 1) | (image_buffer[i] & 1); // extract LSB and shift left
  }
}

Status decode_magic_string(const char *magic_string, DecodeInfo *decInfo) // Function to decode magic string
{
  printf("INFO: Decoding Magic String Signature\n");
  fseek(decInfo->fptr_dest_image, 54, SEEK_SET);
  char arr[8];                      // Declare the arr with size 8.
  char magic[strlen(MAGIC_STRING)]; // "magic" array to store extracted magic string
  char ch;                          // "ch" to store one character in string
  int i;
  for (i = 0; i < strlen(MAGIC_STRING); i++) // Run the loop strlen(magic_string)
  {
    fread(arr, 1, 8, decInfo->fptr_dest_image); // Read the 8 byte of data from src file.
    decode_byte_from_lsb(&ch, arr);             // Call the encode_byte_to_lsb(magic_string[i], arr);
    magic[i] = ch;
  }
  magic[i] = '\0'; // Store last character as '\0'
  if (strcmp(magic, MAGIC_STRING) == 0)
  {
    return e_success; // return success
  }
  return e_failure; // return failure
}

Status decode_secret_file_extn_size(int *size, DecodeInfo *decInfo) // Function to decode size of secret file extension size
{
  /*Cursor in 56 position*/
  char arr[32];                                // Declare the arr with size 32
  fread(arr, 1, 32, decInfo->fptr_dest_image); // Read the 32 byte of data from src file
  decode_int_from_lsb(size, arr);              // Call the encode_int_from_lsb(strlen(encInfo -> extn_secret_file), arr)
  return e_success;
}

Status decode_secret_file_extn(char *file_extn, DecodeInfo *decInfo) // Function to decode size of secret file extension
{
  printf("INFO: Decoding Output File Extenstion\n");
  char arr[8];
  char ch;
  int i;                                   // Declare the arr with size 8.
  for (i = 0; i < decInfo->extn_size; i++) // Run the loop strlen(file_extn)
  {
    fread(arr, 1, 8, decInfo->fptr_dest_image); // Read the 8 byte of data from src file.
    decode_byte_from_lsb(&ch, arr);             // Call the encode_byte_from_lsb(file_extn[i], arr);
    file_extn[i] = ch;
  }
  file_extn[i] = '\0';
  return e_success;
}

Status decode_secret_file_size(long *file_size, DecodeInfo *decInfo) // Function to decode secret file size
{
  printf("INFO: Decoding secret_msg.txt File Size\n");
  int copy;
  char arr[32];                                // Delcrae the arr with size 32
  fread(arr, 1, 32, decInfo->fptr_dest_image); // Read the 32 byte of data from src file
  decode_int_from_lsb(&copy, arr);             // Call the encode_int_to_lsb(file_size), arr)
  *file_size = copy;
  return e_success;
}

Status decode_secret_file_data(DecodeInfo *decInfo) // Function to decode secret file data

{
  printf("INFO: Decoding secret_msg.txt File Data\n");
  char arr[8];
  char ch;
  for (int i = 0; i < decInfo->size_decode_secret_file; i++)
  {
    fread(arr, 1, 8, decInfo->fptr_dest_image); // Read 8 bytes
    decode_byte_from_lsb(&ch, arr);             // Extract 1 byte (char)
    decInfo->decode_secret_data[i] = ch;        // Store it properly
  }

  decInfo->decode_secret_data[decInfo->size_decode_secret_file] = '\0'; // Null terminate the string

  fprintf(decInfo->fptr_decode_secret, "%s", decInfo->decode_secret_data); // Write to output file

  return e_success;
}

Status do_decoding(DecodeInfo *decInfo) // Function used to do all remaining function calls inside --> last function called in main function
{
  printf("\033[1;97mINFO: ## Decoding Procedure Started ##\nINFO: Opening required files\n");
  if (open_decoding_dest_file(decInfo) == e_failure) // Call open files ---> all neccessary files to access its contents
  {
    return e_failure; // If couldn't open files
  }
  if (decode_magic_string(MAGIC_STRING, decInfo) == e_failure) // call decode_magic_string)
  {
    /*From here encoding starts byte to LSB*/
    return e_failure;
  }
  printf("INFO: Done\n");
  if (decode_secret_file_extn_size(&decInfo->extn_size, decInfo) == e_failure) // call decode_secret_file_extn_size()
  {
    return e_failure;
  }
  if (decode_secret_file_extn(decInfo->extn_decode_secret_file, decInfo) == e_failure) // call decode_secret_file_extn()
  {
    return e_failure;
  }
  printf("INFO: Done\n");
  // printf("decInfo->extn_decode_secret_file = %s\n", decInfo->extn_decode_secret_file);
  if (open_decoding_secret_file(decInfo) == e_failure) // call open_decoding_secret_file()
  {
    return e_failure; // If couldn't open secret file
  }

  if (decode_secret_file_size(&decInfo->size_decode_secret_file, decInfo) == e_failure) // decode_secret_file_size()
  {
    return e_failure;
  }
  printf("INFO: Done\n");
  if (decode_secret_file_data(decInfo) == e_failure) // call decode_secret_file_data()
  {
    return e_failure;
  }
  printf("INFO: Done\nINFO: ## Decoding Done Successfully ##\n");
}
