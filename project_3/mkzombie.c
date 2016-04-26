#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	int child = fork();
	if (!child)
		return 0;
	printf("%d %d\n", getpid(), child);
	while (1);

	return 0;
}
