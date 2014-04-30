//zparnold: Zach Arnold

#include <sys/syscall.h>
#include <stdio.h>
// These values MUST match the unistd_32.h modifications

#define __NR_cs3013_syscall1 349
#define __NR_cs3013_syscall2 350
#define __NR_cs3013_syscall3 351

long testCall1 ( void) {
return (long) syscall(__NR_cs3013_syscall1);
}

long testCall2 ( void) {
return (long) syscall(__NR_cs3013_syscall2);
}

long testCall3 ( void) {
return (long) syscall(__NR_cs3013_syscall3);
}

int main () {
printf("The return values of the system calls are:\n");
printf("\tcs3013_syscall1: %ld\n", testCall1());
printf("\tcs3013_syscall2: %ld\n", testCall2());
printf("\tcs3013_syscall3: %ld\n", testCall3());
return 0;
}
