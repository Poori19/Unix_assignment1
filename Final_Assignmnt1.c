
/* -------------------------------- */
/*  Author:   Poornima Byre Gowda   */
/*.......CS 590 Assignment .........*/ 
/*.......Date: JUNE 28TH 2016 ......*/
/* -------------------------------- */

/* PROGRAM DESCRIPTION */

/*

1.Copy	one	file	(sourceFile)	to	another	(targetFile)

     /mycp sourceFile targetFile
	 
	• If	targetFile is	an	existing	file,	then	it	is	overwritten
	• If targetFile is	a	directory,	then	sourceFile is	copied	to	
	targetFile/sourceFile
	• Your	program	must	be	able	to	handle	the	following	errors	gracefully:
	- sourceFile does	not	exist (output error	message)
	- targetFile exists	(warn	the	user	since	it	will	be	overwritten)
	- fail with	an	error	message	if	the	sourceFile	and	targetFile	are	the	same
	(can’t	copy	a	file	to	itself)
	
	
2. Copy multiple	files	(file1,	file2,	...)to	the	target	directory (targetDir)

	Usage:	./mycp file1 file2 file3 targetDir

	• Your program	must	be	able	to	handle	the	following	errors	gracefully
	- any	one	of	the	input	files	does	not	exist
	- targetDir does	not	exist

*/

/* Hearders */
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
	int num = argc - 1;
	int input_fd[num], input_fd1, temp_fd, output_fd, copy_fd, File_fd[num];    /* Input and output file descriptors */
	struct stat st_source;       /* structure of stat to store the source file info*/
	struct stat st_destination;  /* structure of stat to store the destination file info*/
	ssize_t read_bytes ; 		 /*number of bytes read in source file; */
	ssize_t write_bytes ; 		 /*number of bytes written to destination file; */
	char buffer[BUF_SIZE];       /* buffer */ 
	char User_input[2], Directory_path[25]; /* char array as buffers */
	
	int  i ;
	
	// exits if the argc is 2 .. i.e. no input file
	 if (argc == 2 )
	{
		printf("Error: No inputs ");
		return (EXIT_FAILURE);
	}
	
	if (argc == 3)
	{
	/* ..........................................................................*/
	/* opening the source file */ 
	 
	 input_fd1 = open( argv[1], O_RDONLY ); 
	 if (input_fd1 == -1)
	 {
	
		 perror("Source File: ");
		 return (EXIT_FAILURE);
	 }
	 
	 /* ..........................................................................*/
	 /* opening the Destination File or directory */
	 
	  temp_fd = open (argv[2], O_RDONLY,666 ) ;
	 
	    if (temp_fd == -1)
		 {       
	            /* Error Handling */
			    if (errno ==  EACCES)
				{
				 perror("\n");
			     printf("There is no permision on this Directory ");
			    }
			    /* if a destination file not present it opens the new destination file */
			    output_fd = open (argv[2], O_RDONLY | O_CREAT | O_WRONLY, 0666) ;
	 
				if (output_fd == -1) // if error occurs
				{
				 perror("\n");
				 return (EXIT_FAILURE);
				}
				
		 } 
		 
		 if (temp_fd != -1)
		 {
			output_fd = temp_fd;
		 }
		 
		/* ...................Getting info of Source and Destination File.........*/
		 
		    if (fstat(input_fd1, &st_source))
			{
			 printf("\n fstat error: %s \n ", strerror(errno));
			}
	
			if (fstat(output_fd, &st_destination))
			{
			 printf("\n fstat error: %s \n ", strerror(errno));
			}
			
			
			
		/* ......................................................................*/
		/* The destination is file */ 
		
			if (S_ISREG (st_destination.st_mode))
			{
			
				//printf("\n File is a regular file ");
				
				/* Finding whether the source and destination file are same or not */ 
				if (st_source.st_ino == st_destination.st_ino)
				{
				printf("Error: Source and Destination file are same\n");
				return (EXIT_FAILURE);
				}
				if (temp_fd != -1)
				{
					printf(" \n Warning : The Destination file already exits and will be over written \n");
					printf("\n Do you want to over write the file:" );
					scanf("%s", User_input);
					// printf("\n input:  %s \n ",  User_input );
					
					/* ......................................................................*/
					/* if the user permits to over write the file. the file is over written  */ 
					if ((strcmp(User_input , "y") == 0) | (strcmp(User_input , "Y") == 0))
					{
					output_fd = open (argv[2], O_RDONLY |  O_WRONLY, 0666) ;
					while((read_bytes = read (input_fd1, &buffer, BUF_SIZE)) > 0)
					{
						write_bytes = write (output_fd, &buffer, (ssize_t) read_bytes);
						if(write_bytes != read_bytes)
						{
						/* Write error */
						perror("write");
						return (EXIT_FAILURE);
						}
					}
					}
					else {return (EXIT_FAILURE);}
			    }
				/* ......................................................................*/
				else 
				{
					/* writing into the new file , if destination file doesnot exist*/
					while((read_bytes = read (input_fd1, &buffer, BUF_SIZE)) > 0)
				{
					write_bytes = write (output_fd, &buffer, (ssize_t) read_bytes);
					if(write_bytes != read_bytes)
					{
					/* Write error */
					perror("write");
					return (EXIT_FAILURE);
					}
			    }
					
				}
			}
		   		  

				  
		   /* ......................................................................*/
		   /* .....................The destination is Directory.................... */ 
		   
		   if (S_ISDIR (st_destination.st_mode))
			{
				//printf("\n Destination is directory \n");
				
				/* ......... checking access permissin of the directory .............*/ 
				
				strcpy(Directory_path,argv[2]);
				if (access(Directory_path,  W_OK) == 0)
				{
					printf("User has write permission \n");
				}
				else 
				{ 
			     printf("User doesnot have write permission \n");
				 return (EXIT_FAILURE) ;
				}
				strcat (Directory_path, "/");			
				strcat (Directory_path, argv[1]);
				
				//printf ("str1: %s\n str2: %s\n str3: %s\n",argv[1],argv[2],Directory_path);
				
				
				/* ..........copying Files to the Directory...............................*/
				copy_fd = open (Directory_path, O_RDONLY | O_CREAT | O_WRONLY, 0666) ;
				
				while((read_bytes = read (input_fd1, &buffer, BUF_SIZE)) > 0)
				{
					write_bytes = write (copy_fd, &buffer, (ssize_t) read_bytes);
					if(write_bytes != read_bytes)
					{
					/* Write error */
					perror("write");
					return (EXIT_FAILURE);
					}
			    }
				close(copy_fd);

			}
		    /* ......................................................................*/
			
			
      close (input_fd1); // closing the fds
	  close (output_fd);
	  return (EXIT_SUCCESS);
	 
	}
	 
	 
	 /*............... copying the multiple files to the directory ................... */
	 
	else 
	{
		
	for( i = 1; i < num  ; i++)
	{
		
      //printf("value of argc: %d\n", argc);
	  //printf("value of argv: %s\n", argv[i]);
	  input_fd[i] = open( argv[i], O_RDONLY, 666 ); 
		  if (input_fd[i] == -1)
		 {
			 printf("\n Source File %s ", argv[i]);
			 perror(" ");
			 return (EXIT_FAILURE);
		 }
	  
    }
     
	 /* openning the Destination File or directory */
	 
	    output_fd = open (argv[num], O_RDONLY , 0666) ;
		 if (output_fd == -1)
		 {
			  if (errno ==  EACCES)
				{ printf("\n There is no permission to access this directory \n"); }
			  if (errno ==  ENOENT)
			    { printf("\n Directory doesnot exist \n"); }
			
			 return (EXIT_FAILURE);
		 }		
		 
		 
           if (fstat(output_fd, &st_destination))
			{
			 printf("\n  %s \n ", strerror(errno));
			 return (EXIT_FAILURE);
			}
            
			if (S_ISREG (st_destination.st_mode))
			{
				printf(" Enter the Directory address \n");
                return (EXIT_FAILURE);
			}
			
			/* Writing Multiple files to the Directory */ 
			
			else if (S_ISDIR (st_destination.st_mode))
			{
					strcpy(Directory_path,argv[2]);
					if (access(Directory_path,  W_OK) == 0)
					{
						printf("User has write permission \n");
					}
					else 
					{ 
					 printf("User doesnot have write permission \n");
					 return (EXIT_FAILURE) ;
					}
					strcpy( Directory_path, "" );
					for(i=1 ; i < num ; i++)
					{
					strcpy(Directory_path,argv[num]);
					
					strcat (Directory_path, "/");
					strcat (Directory_path, argv[i]);
					
					//printf ("str1: %s\n str2: %s\n str3: %s\n",argv[1],argv[2],Directory_path);
					
					File_fd[i] = open (Directory_path, O_RDONLY | O_CREAT | O_WRONLY, 0666) ;
					
					while((read_bytes = read (input_fd[i], &buffer, BUF_SIZE)) > 0)
					{
						write_bytes = write (File_fd[i], &buffer, (ssize_t) read_bytes);
						if(write_bytes != read_bytes)
						{
						/* Write error */
						perror("write");
						return (EXIT_FAILURE);
						}
					}
					close(File_fd[i]);
					strcpy( Directory_path, "" );
					}	
                				
			}
  
  // closing all file fds
	for( i = 1; i < num ; i++)
	{
		 close (input_fd[i]);
	}
	}

	return (EXIT_SUCCESS);	
	
}
