/*
 * doit.c
 *
 *  Created on: Jan 22, 2014
 *      Author: student
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/resource.h>

int main(int argc, char *argv[]){

	//variable declarations

	pid_t pid;//child process id
	struct rusage usage;//usage stats holder
	struct timeval starttime; //wall-clock start time
	struct timeval endtime; //wall-clock end time
	

	//set the wall-clock time:
	gettimeofday(&starttime,NULL);

	printf("capturing input and preparing to fork...\n");


	argv[argc] = NULL;
	

	printf("forking...\n");

	//fork a new process and handle errors

	pid = fork();
         if (pid == -1) 
         {
             perror("Error forking");
             return -1;
         }
	 else if (pid > 0)
         {
             // parent process
             wait(pid); //wait until the child process completes before continuing
				   //to avoid any crashing...I hope
	     getrusage(RUSAGE_CHILDREN, &usage);
		 
         }
         else 
         {
	
	printf("executing arguments\n");

		//execute the procedure
             execvp(argv[1], argv + 1);
             exit(0);
         }
		 
		 //Return wall-clock time
			gettimeofday(&endtime,NULL);
			printf("Wall clock time in milliseconds: %0.3f milliseconds\n", (float)((endtime.tv_sec*1000 + endtime.tv_usec/1000) - (starttime.tv_sec*1000 + starttime.tv_usec/1000))); 
			
		 //Return other statistics from getrusage()
	     printf ("CPU time: %0.3f milliseconds user, %0.3f milliseconds system\n", (float)(usage.ru_utime.tv_usec)/1000, (float)(usage.ru_stime.tv_usec)/1000);
		 
		 printf("Number of times process was pre-empted involuntarily: %d \n", (int)usage.ru_nivcsw);
		 
		 printf("Number of times process voluntarily gave up CPU: %d \n", (int)usage.ru_nvcsw);
		 
		 printf("Total number of page faults: %d \n", (int)(usage.ru_majflt + usage.ru_minflt));
		 
		 printf("Number of page faults able to be satisfied by kernel: %d \n", (int)(usage.ru_minflt));

     return 0;
	
	
}
