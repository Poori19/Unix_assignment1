/* Author:   Poornima Byre Gowda */
/* CS 590 ASsignment */ 



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
	
	
	int input_fd, temp_fd, output_fd,copy_fd;     /* Input and output file descriptors */
	struct stat st_source;       /* structure of stat to store the source file info*/
	struct stat st_destination;  /* structure of stat to store the destination file info*/
	ssize_t read_bytes ; 		 /*number of bytes read in source file; */
	ssize_t write_bytes ; 		 /*number of bytes written to destination file; */
	char buffer[BUF_SIZE];       /* buffer */ 
	char User_input[2], Directory_path[25]; 
	
	
	/* ..........................................................................*/
	/* opening the source file */ 
	 
	 input_fd = open( argv[1], O_RDONLY ); 
	 if (input_fd == -1)
	 {
		 perror("Source File : ");
		 return -1;
	 }
	 /* ..........................................................................*/
	 
	 /* ..........................................................................*/
	 /* opening the Destination File or directory */
	 
	  temp_fd = open (argv[2], O_RDONLY ) ;
	 
	    if (temp_fd == -1)
		 {
			 perror("Directory : ");
			 return -1;
		 }                        
	 
	  if (temp_fd != -1)
	  {
		    output_fd = temp_fd;
		  
			if (fstat(input_fd, &st_source))
			{
			 printf("\n fstat error: %s \n ", strerror(errno));
			}
	
			if (fstat(output_fd, &st_destination))
			{
			 printf("\n fstat error: %s \n ", strerror(errno));
			}
		  
	   
	  
	       if (S_ISREG (st_destination.st_mode))
		   {
			   printf("\n Destination is File \n ");
			   
		   }
	        /* The destination is Directory */ 
		   if (S_ISDIR (st_destination.st_mode))
			{
				printf("\n Destination is directory \n");
				strcpy(Directory_path,argv[2]);
				strcat (Directory_path, "/");
				strcat (Directory_path, argv[1]);
				printf ("str1: %s\n str2: %s\n str3: %s\n",argv[1],argv[2],Directory_path);
				copy_fd = open (Directory_path, O_RDONLY | O_CREAT | O_WRONLY, 0666) ;
				
				while((read_bytes = read (input_fd, &buffer, BUF_SIZE)) > 0)
				{
					write_bytes = write (copy_fd, &buffer, (ssize_t) read_bytes);
					if(write_bytes != read_bytes)
					{
					/* Write error */
					perror("write");
					return -1;
					}
			    }
				close(copy_fd);

			}
			
			
	  }
	return (EXIT_SUCCESS);
}
