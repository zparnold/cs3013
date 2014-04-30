// CS-3013, Project 2 -- Getting process info
// 	C-Term 2014

//	prinfo.h

#ifndef _LINUX_PRINFO_H
#define _LINUX_PRINFO_H

#ifdef __KERNEL__
//	The following header files are for loadable kernel modules
#include <linux/types.h>
#include <linux/pid.h>

#else
//	The following header files are for user-space programs.
#include <sys/types.h>
#include <unistd.h>

#endif

struct prinfo {
	long state;		// current state of process
	pid_t pid;		// process ID of this process
	pid_t parent_pid;	// process ID of parent
	pid_t youngest_child;	// process ID of youngest child
	pid_t younger_sibling;	// process ID of next younger sibling
	pid_t older_sibling;	// process ID of next older sibling
	uid_t uid;		// user ID of process owner
	long long start_time;	// process start time in nanosecond since boot time
	long long user_time;	// CPU time spent in user mode (microseconds)
	long long sys_time;	// CPU time spend in system mode (microseconds)
	long long cutime;	// total user time of children (microseconds)
	long long cstime;	// total system time of children (microseconds)
};	// struct prinfo


#endif
