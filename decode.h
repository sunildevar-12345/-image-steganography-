#ifndef DECODE_H
#define DECODE_H

#include "types.h" // Contains user defined types

/* 
 * Structure to store information required for
 * encoding secret file to source Image
 * Info about output and intermediate data is
 * also stored
 */

#define MAX_SECRET_BUF_SIZE 1
#define MAX_IMAGE_BUF_SIZE (MAX_SECRET_BUF_SIZE * 8)
#define MAX_FILE_SUFFIX 4

typedef struct 
{
	//output.bmp as source file
	char *src_image_fname;// output.bmp
	FILE *fptr_src_image;// after open the file it returns the address so to store the we need file pointer
	int file_extn_size;
	char file_extn[100];
	int file_data_size;// secret data size
	int magic_str_len;
	char magic_string[100];

	//output 
	char *output_fname;// output.txt or the user given name is stored here
	FILE *fptr_output_fname;// storeing the address of output_fname

}DecodeInfo;

//read and validation of decode arguments//
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo);

// do decoding //
Status do_decoding(DecodeInfo *decInfo);

Status open_files_decode(DecodeInfo *decInfo);

Status decode_magic_str_len(DecodeInfo *decInfo);

Status decode_magic_string(DecodeInfo *decInfo);

char decode_byte_to_lsb(FILE *fptr_src_image);

Status decode_file_size (FILE *fptr_src_image,int *file_size);

Status decode_file_extn(DecodeInfo *decInfo);

Status decode_secret_data(DecodeInfo *decInfo);


Status destination_open_file(DecodeInfo *decInfo);

#endif
