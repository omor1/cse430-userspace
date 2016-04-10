#include <stdio.h>
#include <unistd.h>

int main(void)
{
	int tmp = 0x420;
	printf("%d %p\n", getpid(), &tmp);
	while (420);
	return 0;
}
