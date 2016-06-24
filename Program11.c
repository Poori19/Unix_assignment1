#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>

 
#define BUF_SIZE 1000 /* Defining the buffre size */


int main(int argc, char* argv[]) 
{
	int input_fd, temp_fd, output_fd;     /* Input and output file descriptors */
	struct stat st_source;       /* structure of stat to store the source file info*/
	struct stat st_destination;  /* structure of stat to store the destination file info*/
	ssize_t read_bytes ; 		 /*number of bytes read in source file; */
	ssize_t write_bytes ; 		 /*number of bytes written to destination file; */
	char buffer[BUF_SIZE];       /* buffer */ 
	char User_input[2]; 
	
	
	
	/* opening the source file */ 
	 
	 input_fd = open( argv[1], O_RDONLY ); 
	 if (input_fd == -1)
	 {
		 perror("Source File : ");
		 return -1;
	 }

	 
	 /* opening the Destination File or directory */
	 
	 temp_fd = open (argv[2], O_RDONLY |  O_WRONLY, 0666) ;
	 
	 if (temp_fd == -1)
	 {
		output_fd = open (argv[2], O_RDONLY | O_CREAT | O_WRONLY, 0666) ;
	 
		if (output_fd == -1)
		{
		 perror("Destination File/Drirectory: ");
		}
	 }	 
	 else
	 {
		 output_fd = temp_fd;
		 printf(" \n Warning : The Destination file already exits and will be over written \n");
		 
	 }
	 
	 
		

	  if (input_fd != -1) 
	  {
			if (fstat(input_fd, &st_source))
			{
			 printf("\n fstat error: %s \n ", strerror(errno));
			}
	
			if (fstat(output_fd, &st_destination))
			{
			 printf("\n fstat error: %s \n ", strerror(errno));
			}
		
			//printf("I-node number:           %ld\n", (long) st_source.st_ino);
			//printf("I-node number:           %ld\n", (long) st_destination.st_ino);
			
		    /* Finding whether the source and destination file are same or not */ 
			if (st_source.st_ino == st_destination.st_ino)
			{
			printf("Error: Source and Destination file are same\n");
			return -1; 
			}
			
			
			/* The destination is file */ 
		
			if (S_ISREG (st_destination.st_mode))
			{
			
				//printf("\n File is a regular file ");
				printf("\n Do you want to over write the file:" );
				scanf("%s", User_input);
				printf("\n input:  %s \n ",  User_input );
				if ((strcmp(User_input , "y") == 0) | (strcmp(User_input , "Y") == 0))
				{
				while((read_bytes = read (input_fd, &buffer, BUF_SIZE)) > 0)
				{
				write_bytes = write (output_fd, &buffer, (ssize_t) read_bytes);
				if(write_bytes != read_bytes)
				{
				/* Write error */
				perror("write");
				return 2;
				}
			}
			}
			
			}
			
			/* The destination is Directory */ 
			else if (S_ISDIR (st_destination.st_mode))
			{
				printf("\n File is directory ");
			}
			
			
	  }
		
		
		
	 
	 close (input_fd);
	 close (output_fd);
	 return 0;
}
