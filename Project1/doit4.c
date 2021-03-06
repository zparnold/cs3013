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
#include <sys/wait.h>
#include <sys/types.h>


#define MAX_LENGTH 128
#define MAX_ARGS 32
#define CHARS " \t\r\n"

int main(int argc, char *argv[]){

	//variable declarations
	char *cmd; //the incoming command (first one only)
  	char line[MAX_LENGTH]; //array with the max line length
	char *line2[MAX_ARGS]; //another array to contain the args
	
	pid_t pid;//child process id
		struct rusage usage;//usage stats holder
		struct rusage parallelusage; //usage holder for parallel processes
		struct timeval starttime; //wall-clock start time
		struct timeval endtime; //wall-clock end time

		
	//typedef struct {int num; pid_t pid;} job;
	
	//job jobs[MAX_LENGTH];

	long double systime = 0;
	long double usrtime = 0;
	int nivcsw = 0;
	int nvcsw = 0;
	int minflts = 0;
	int majflts = 0;
	
	int i; //the counter
	int nextflag = 0;
	int nextproc = 1;
	int wait3returnstatus;
	
	//struct queue_structure *queue_pointer = malloc(sizeof(struct queue_structure));
  //int option,number,dequeue_number;
  //queue_pointer->queue_front = queue_pointer->queue_rear = -1;

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
			//strtok the command to be executed
			for (i=1;i<MAX_ARGS;i++){
						char *arg2 = strtok(NULL,CHARS);
						if (arg2 == NULL){
							line2[i]=NULL;
							//set the next flag if I got an ampersand
							if(strcmp(line2[i-1],"&")==0){
							nextflag = 1;
							}
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
				
				//need to handle jobs command!!
				else if (nextflag == 0) {}
		
	}	
	return 0;
}






