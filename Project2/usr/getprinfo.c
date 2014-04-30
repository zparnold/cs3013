//This is to test out the new system call interceptor
//zparnold: Zach Arnold syoon: Shawn Yoon

#include <sys/syscall.h>
#include <stdio.h>
#include "prinfo.h"

// These values MUST match the unistd_32.h modifications

#define __NR_cs3013_syscall1 349


long testCall1 (struct prinfo* info) {
return (long) syscall(__NR_cs3013_syscall1, info);
}


int main () {
test3function();
return 0;
}


int printStruct(struct prinfo* info){
printf("STATE = %ld\n", info->state);
printf("PID= %d\n", info->pid);
printf("PARENT_PID= %d\n", info->parent_pid);
printf("YOUNGEST_CHILD= %d\n", info->youngest_child);
printf("OLDER_SIBLING= %d\n", info->older_sibling);
printf("UID= %d\n", info->uid);
printf("START_TIME= %lld\n", info->start_time);
printf("USER_TIME= %lld\n", info->user_time);
printf("SYS_TIME= %lld\n", info->sys_time);
printf("TOTAL USER TIME OF CHILDREN= %lld\n", info->cutime);
printf("TOTAL SYSTEM TIME OF CHILDREN= %lld\n", info->cstime);
return 0;
}

int test1function(void){

//single level tests
struct prinfo* test1 = (struct prinfo*)malloc(sizeof(struct prinfo));
printf("The return values of the system calls are:\n");
printf("\tcs3013_syscall1: %ld\n", testCall1(test1));
printStruct(test1);
return 0;
}

int test2function(void){

//local vars
int i = 0;
pid_t pid;

	for (i = 0; i <3; i++){

		pid = fork();
		if (pid < 0){
		return -1;
		}
		if (pid == 0){
		break;
		}
		else{
		wait(pid);
		}
		

	
	}
	if (pid == 0){ //children?
		test1function();
	}
	else{
	test1function();
	}
	

return 0;
}

int test3function(void){

//local vars
int i = 0;
pid_t pid;

	for (i = 0; i <3; i++){

		pid = fork();
		if (pid < 0){
		return -1;
		}
		if (pid == 0){
		break;
		}
		else{
		wait(pid);
		}
		

	
	}
	if (pid == 0){ //children?
		test2function();
	}
	else{
	test2function();
	}
	

return 0;
}
