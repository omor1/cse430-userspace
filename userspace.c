#include <sys/syscall.h>
#include <unistd.h>
#include "task_info.h"

# ifdef __i386__
#  define __NR_ntasks 359
#  define __NR_task_info 360
# elif defined(__ILP32__)
#  define __NR_ntasks (__X32_SYSCALL_BIT + 323)
#  define __NR_task_info (__X32_SYSCALL_BIT + 324)
# else
#  define __NR_ntasks 323
#  define __NR_task_info 324
# endif

#define SYS_ntasks __NR_ntasks
#define SYS_task_info __NR_task_info

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
	size_t num;
	syscall(SYS_ntasks, &num);
	struct task_info tasks[num];
	long syscall_ret = syscall(SYS_task_info, tasks, num);
	if (syscall_ret > 0)
		num = syscall_ret;
	printf("  PID TTY          TIME CMD\n");
	for(size_t i = 0; i < num; i++) {
		printf("%5d ", tasks[i].pid);
		if (strcmp(tasks[i].tty_name, "NULL tty") == 0)
			printf("%-8s ", "?");
		else
			printf("%-8s ", tasks[i].tty_name);
		unsigned long long secs = tasks[i].nsecs / 1000000000;
		printf("%02llu:%02llu:%02llu ", secs/3600, secs/60, secs);
		printf("%s\n", tasks[i].comm);
	}
	return 0;
}
