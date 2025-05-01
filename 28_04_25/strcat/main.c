#include <string.h>
#include <stdio.h>

int main(void)
{
	char str0[20];
	
	char *str1 = "hello ";
	printf("str1: %s\n", str1);
	char *str2 = "world!";
	printf("str2: %s\n", str2);
	strcat(str0,str1);
	printf("str0: %s\n", str0);
	strcat(str0,str2);
	printf("str0: %s\n", str0);
	strcat(str0,str1);
	printf("str0: %s\n", str0);
	strcat(str0,str2);
	printf("str0: %s\n", str0);

	return 0;
}
