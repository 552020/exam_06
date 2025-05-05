// #include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

void fatal_error(void)
{
	write(2, "Fatal error\n", 12);
	exit(1);
}

int main(int argc, char **argv)
{
	fatal_error();
	return 0;
}
