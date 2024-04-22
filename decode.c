#include<stdio.h>
#include"decode.h"
#include<string.h>


Status read_and_validate_decode_args(char *argv[],DecodeInfo *decInfo)
{
	if(argv[2] != NULL)
	{
		if(strstr (argv[2],".bmp") != NULL)
		{
			if (!strcmp(strstr(argv[2],".bmp"), ".bmp"))
			{
				decInfo->src_image_fname = argv[2];
			}
			else
			{
				printf("argv[2] is not having .bmp extension\n");
				return e_failure;
			}
		}
		else
		{
			printf("argv[2] is not having .bmp extension\n");
			return e_failure;
		}
	}
	else
	{
		printf("Pass the correct argument 3 or 4\n");
		return e_failure;
	}
	
	if(argv[3] != NULL)
	{
		if(strstr(argv[3],".") == NULL)
		{
		
				//strcpy(decInfo->file_extn,strstr(argv[3],"."));
				decInfo->output_fname = argv[3];

		}
		else
		{
			printf("ERROR : Do not pass any extension for output file\n");
			return e_failure;
		}
	}
	else
	{
		
		decInfo->output_fname = "output.txt";
		
		
	}
	return e_success;
}

Status do_decoding(DecodeInfo *decInfo)
{
  	if(open_files_decode(decInfo) == e_success)
 	{
		 printf("open_files_decode is sucessfull\n");
		 printf("ENTER THE MAGIC STRING\n");
		 scanf("%s", decInfo->magic_string);

		 if(decode_magic_str_len(decInfo) == e_success)
		 {
			 printf("decode_magic_string_length is sucessfull\n");
			 
		 	if(decode_magic_string(decInfo) == e_success)
		 	{
				 printf("decode_magic_string is sucessfull\n");

				 if(decode_file_size(decInfo->fptr_src_image,&decInfo->file_extn_size) == e_success)
				 {
					 printf("decode_file_extn_size is sucessfull\n");
					 if(decode_file_extn(decInfo) == e_success)
					 {
						 printf("decode_file_extn is sucessfull\n");
						 if(destination_open_file(decInfo) == e_success)
						 {
							 printf("destination file open successfully\n");
							  if(decode_file_size(decInfo->fptr_src_image,&decInfo->file_data_size) == e_success)
							 {
								 printf("decode file data size is sucessfull\n");
								 if(decode_secret_data(decInfo) == e_success)
								 {
									 printf("decode_secret_data is sucessfull\n");
								 }
								 else
								 {
									 printf("decode_secret_data is failed\n");
									 return e_failure;
								 }
						 	}
						 	else
						 	{
								 printf("decode file data size is failed\n");
								 return e_failure;
							 }
						 }
						 else
						 {
							 printf("destination file open failed\n");
							 return e_failure;
						 }
					 }
					 else
					 {
						 printf("decode_file_extn is failed\n");
						 return e_failure;
					 }
				 }
				 else
				 {
					 printf("decode_file_extn_size is failed\n");
					 return e_failure;
				 }
			 }
			 else
			 {
				 printf("decode_magic_string is failed\n");
				 return e_failure;
			 }
		}
		 else
		 {
			 printf("decode_magic_string_length is failed\n");
			 return e_failure;
		 }
	}
	else
	{
		printf("open_files_decode is failed\n");
		return e_failure;
	}


	return e_success;

}

Status open_files_decode(DecodeInfo *decInfo)
{
    // Src Image file
    decInfo->fptr_src_image = fopen(decInfo->src_image_fname, "r");
    // Do Error handling
    if (decInfo->fptr_src_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->src_image_fname);

    	return e_failure;
    }

        
    // No failure return e_success
    return e_success;
}

Status destination_open_file(DecodeInfo *decInfo)
{
 decInfo->fptr_output_fname = fopen(decInfo->output_fname, "w");
    // Do Error handling
    if (decInfo->fptr_output_fname == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->output_fname);

    	return e_failure;
    }
	return e_success;

}

Status decode_magic_str_len(DecodeInfo *decInfo)
{

	fseek(decInfo->fptr_src_image,54,SEEK_SET);

    decode_file_size (decInfo->fptr_src_image,&decInfo->magic_str_len);
	printf("%d\n", decInfo->magic_str_len);
	return e_success;
}



Status decode_magic_string(DecodeInfo *decInfo)
{
	
	char magic_str[decInfo->magic_str_len+1];
	
	int i;
	for(i=0;i<decInfo->magic_str_len;i++)
	{
		magic_str[i] = decode_byte_to_lsb(decInfo->fptr_src_image);
	}
	magic_str[i] = '\0';

	printf("%s\n", magic_str);
	
	if(!strcmp(magic_str,decInfo->magic_string))
	{
		printf("magicstring  is matched\n");
		return e_success;
	}
	else
	{
		printf("magicstring is not matched\n");
		return e_failure;
	}
	

}

char decode_byte_to_lsb(FILE *fptr_src_image)
{
	char ch = 0;
	char image_buff[8];
	fread(image_buff,1,8,fptr_src_image);

	for(int i=0;i<8;i++)
	{
		ch = ch | ((image_buff[i] & 1) << (7-i));
	}


	return ch;
}

//to decode "extn size and secret file data size" for both tasks this function is used
Status decode_file_size (FILE *fptr_src_image,int *file_size)

{
	
	*file_size = 0;
	
	char image_buff[32];
	fread(image_buff,1,32,fptr_src_image);

	for(int i=0;i<32;i++)
	{
		*file_size = (*file_size | ((image_buff[i] & 1) << (31-i)));
	}

	
	return e_success;
}

Status decode_file_extn(DecodeInfo *decInfo)
{
		
	char image_buff[decInfo->file_extn_size];
	
	int i;
	for(i=0;i<decInfo->file_extn_size;i++)
	{	
		image_buff[i] = decode_byte_to_lsb(decInfo->fptr_src_image);
		
	}	

	if(!strstr(decInfo->output_fname, "."))// new
	{
		strcat(decInfo->output_fname, image_buff);
		return e_success;
	}
	else
	{
		return e_success;
	}


}
Status decode_secret_data(DecodeInfo *decInfo)
{

	char image_buff[decInfo->file_data_size];

	for(int i=0; i<decInfo->file_data_size; i++)
	{
		image_buff[i] = decode_byte_to_lsb(decInfo->fptr_src_image);
	}
	fwrite(image_buff,1,decInfo->file_data_size,decInfo->fptr_output_fname);

	return e_success;
}









