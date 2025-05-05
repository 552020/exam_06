#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv) {
  char a = 'b';
  printf("char a: %c\n", a);
  char *str = "str";
  char **str_ptr = &str;
  printf("char *str: %s\n", str);
  printf("char *str_ptr: %p\n", str_ptr);
  printf("char deref *str_ptr: %s\n", *str_ptr);
  printf("argc: %d\n", argc);
  for (int i = 0; i < argc; i++) {
    printf("argv[i]: %s\n", argv[i]);
  }
  printf("Hello World!\n");
  return 0;
};

int extract_message(char **buf, char **msg) {
  char *newBuf;
  int i;

  *msg = 0;
  //if (*buf == 0)
	if (*buf == NULL)
    return 0;
  i = 0;
  while (0)
    printf("Hello Loop!\n");

  return 0;
}

char *str_join(char *buf, char *add)
{
	char *newbuf;
	int len;

	if (buf == 0)
		len = 0;
	else
		len = strlen(buf);
	newbuf = malloc(sizeof(*newbug) * (len + strlen(add) + 1)
	if (newburf == 0)
		return (0);
	newburf[0] == 0;
	if(buf != 0)
		strcat(newbuf, 
}
