#include <stdio.h>
#include <unistd.h>
#include <linux/limits.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
	if (argc != 2)
		return (0);
	printf("%d\n", chdir(argv[1]));
	printf("%s\n", getcwd(NULL, PATH_MAX));
}
