#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/syscall.h>
#include <unistd.h>

# ifdef __i386__
#  define __NR_ntasks 359
#  define __NR_task_info 360
#  define __NR_get_addr 361
# elif defined(__ILP32__)
#  define __NR_ntasks (__X32_SYSCALL_BIT + 323)
#  define __NR_task_info (__X32_SYSCALL_BIT + 324)
#  define __NR_get_addr (__X32_SYSCALL_BIT + 325)
# else
#  define __NR_ntasks 323
#  define __NR_task_info 324
#  define __NR_get_addr 325
# endif

#define SYS_ntasks __NR_ntasks
#define SYS_task_info __NR_task_info
#define SYS_get_addr __NR_get_addr

#define PAGE_SWAP_BITS          62
#define PAGE_SWAP               (1ULL << PAGE_SWAP_BITS)

int main(int argc, char *argv[])
{
	if (argc != 3) {
		printf("Usage:\t%s pid address\n", argv[0]);
		return EINVAL;
	}

	pid_t pid = strtol(argv[1], NULL, 10);
	unsigned long addr = strtol(argv[2], NULL, 16);

	unsigned long long phys;
	if(syscall(SYS_get_addr, pid, addr, &phys) == -1) {
		switch (errno) {
		case ESRCH:
			printf("No such process\n");
			break;
		case EINVAL:
			printf("Invalid address\n");
			break;
		case EFAULT:
			printf("Unable to copy memory\n");
			break;
		}
		return errno;
	}

	if (phys & PAGE_SWAP) {
		phys &= ~PAGE_SWAP;
		printf("Swap identifier:\t%llu\n", phys);
	} else {
		printf("Physical address:\t0x%llx\n", phys);
	}
}
