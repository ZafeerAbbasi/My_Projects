/*
# ##############################################################################
# File: my_cp.c                                                                #
# Project: 1                                                                   #
# Created Date: Saturday, July 1st 2023, 9:57:35 am                            #
# Author: Zafeer Abbasi                                                        #
# ----------------------------------------------                               #
# Last Modified: Saturday, July 1st 2023, 2:24:58 pm                           #
# Modified By: Zafeer Abbasi                                                   #
# ----------------------------------------------                               #
# Copyright (c) 2023 Zafeer.A                                                  #
# ----------------------------------------------                               #
# HISTORY:                                                                     #
# Date      	By          	Comments                                       #
# ##########	########    	################################################
 */


#include <stdio.h>
#include <stdlib.h>

#define BLOCKSIZE 4096

int main(int argc, char **argv)
{
    //If not enough arguements are given
    if(argc != 3)
    {
        printf("Invalid number of parameters, must be 2\n");
        exit(0);
    }
    
    FILE *source, *dest;
    source = fopen(argv[1], "rb"); //opening source file in rb mode = read binary mode
    //Check if open was successful
    if(source == NULL)
    {
        printf("Unable to open the file: %s, please make sure the file exists\n", argv[1]);
        exit(0);
    }
    
    dest = fopen(argv[2], "wb"); //opening destination file in wb mode = write binary
    //check if open was successful
    if(dest == NULL)
    {
        printf("Unable to open the file: %s, please make sure you have permission to write\n", argv[2]);
        exit(0);
    }

    //At this point we have successfully opened the source and destination files
    
    //Char buffer to store the read data
    char *buffer = (char *)malloc(sizeof(char) * BLOCKSIZE);
    if(buffer == NULL)
    {
        printf("Unable to allocate buffer memory for copying ... \n");
        exit(0);
    }

    //Set file pointer fp to end of the file
    fseek(source, 0l, 2);
    
    //Get the value of the fp so we can get the size of the file in bytes
    long rem_size = ftell(source);
    
    //set file pointer to start so we can start reading
    fseek(source, 0l, 0);
    
    while(rem_size > 0)
    {
        size_t bytes_read = 0;
        
        if(rem_size < BLOCKSIZE)
        {
            bytes_read = fread((void *)buffer, (size_t)1, rem_size, source);
        }
        else
        {
            bytes_read = fread((void *)buffer, (size_t)1, BLOCKSIZE, source);
        }

        //write data to dest file
        fwrite((void *)buffer, (size_t)1, bytes_read, dest);
        
        //decrement the remaining size
        rem_size -= bytes_read;
    }

    //At this point all the bytes from source have been copied to dest
    fclose(source);
    fclose(dest);

    //free allocated memory
    free(buffer);
    buffer = NULL;

    printf("File has been copied successfully!\n");

    
    return 0;
}