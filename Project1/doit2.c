/*
 * doit.c
 *
 *  Created on: Jan 22, 2014
 *      Author: student
 * thanks to: http://stackoverflow.com/questions/4788374/writing-a-basic-shell for help with this assignment
 * http://stackoverflow.com/questions/18927793/how-to-use-strtok
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/resource.h>

#define MAX_LENGTH 128
#define MAX_ARGS 32
#define CHARS " \t\r\n"

int main(int argc, char *argv[]){

	//variable declarations
	
	pid_t pid;//child process id
		struct rusage usage;//usage stats holder
		struct timeval starttime; //wall-clock start time
		struct timeval endtime; //wall-clock end time

	char *cmd; //the incoming command string (post-tokenize)
  	char line[MAX_LENGTH]; //array with the max line length
	char *line2[MAX_ARGS];

	long double systime = 0;
	long double usrtime = 0;
	int nivcsw = 0;
	int nvcsw = 0;
	int minflts = 0;
	int majflts = 0;
	
	int i; //the counter

	char *a; //this is a nothing variable


  	while (1) {
    		printf("==>");
			//handle line length

			// Parse and execute command
			
    		fgets(line, MAX_LENGTH, stdin);

			cmd = strtok(line,CHARS);
					line2[0] = cmd;

			if(cmd == NULL) {
				continue;
			}

			for (i=1;i<MAX_ARGS;i++){
						char *arg2 = strtok(NULL,CHARS);
						if (arg2 == NULL){
							line2[i]=NULL;
							break;
						}
						else{
							line2[i] = arg2;
						}
					}
    		
		
      		// Clear errors
     		 errno = 0;

      			if (strcmp(line2[0], "cd") == 0) {

        			if (chdir(line2[1])!= 0) fprintf(stderr, "directory does not exist.\n");
        			else chdir(line2[1]);

      			} 
				else if (strcmp(cmd, "exit") == 0) {
        			break;

      			}
      		
				else {
		
				//do it like the italian job...or doit did...or whatever
		
				//setup vars and prepare to fork (more memory intensive, but whatever, I don't like subtraction.)
		
		
				//set the wall clock time:
				gettimeofday(&starttime,NULL);
		
				//fork a new process and handle errors

				pid = fork();
         			if (pid == -1) {
             			perror("Error forking");
             			return -1;
         			}	
         	
	 				else if (pid > 0){
             
					 // parent process
					 wait(0); //wait until the child process completes before continuing
								 //to avoid any crashing...I hope

					 //now fill the vars for printing stats:
						getrusage(RUSAGE_CHILDREN, &usage);

					//getrusage(RUSAGE_CHILDREN, &usage);
		 
					}

					else{ //execute the process
					
					
					if (execvp(cmd, &line2) < 0){
						perror("error in execvp somewhere");
						exit(1);
					}
					else{
					exit(0);
					}
					}

					//Return wall-clock time
					gettimeofday(&endtime,NULL);
					printf("\nWall clock time in milliseconds: %0.3f milliseconds\n", (float)((endtime.tv_sec*1000 + endtime.tv_usec/1000) - (starttime.tv_sec*1000 + starttime.tv_usec/1000))); 
			
					//Return other statistics from getrusage()
					printf ("CPU time: %ld milliseconds user, %ld milliseconds system\n", ((usage.ru_utime.tv_sec * 1000 + usage.ru_utime.tv_usec/1000) - usrtime), ((usage.ru_stime.tv_sec * 1000 + usage.ru_stime.tv_usec/1000) - systime));
		 
					printf("Number of times process was pre-empted involuntarily: %d \n", (int)(usage.ru_nivcsw - nivcsw));
		 
					printf("Number of times process voluntarily gave up CPU: %d \n", (int)(usage.ru_nvcsw - nvcsw));
		 
					printf("Total number of page faults: %d \n", (int)((usage.ru_majflt + usage.ru_minflt) - (majflts+minflts)));
		 
					printf("Number of page faults able to be satisfied by kernel: %d \n", (int)(usage.ru_minflt) - (minflts));

						systime = (usage.ru_utime.tv_sec * 1000 + usage.ru_utime.tv_usec/1000);
						usrtime = (usage.ru_stime.tv_sec * 1000 + usage.ru_stime.tv_usec/1000);
						nivcsw = (int)usage.ru_nivcsw;
						nvcsw = (int)usage.ru_nvcsw;
						minflts = (int)usage.ru_minflt;
						majflts = (int)usage.ru_majflt;

					}

  	}
	return 0;
}






