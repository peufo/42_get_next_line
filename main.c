#include "get_next_line.h"
#include <fcntl.h>
#include <stdio.h>

void put_str(char *str)
{	
	while (*str)
		write(1, str++, 1);
}

int main(void)
{

	int fd = open("test.txt", O_RDONLY);

	char *values[10];
	int	index = 0;

	while ((values[index] = get_next_line((fd))))
	{
		put_str(values[index]);
	}
	put_str("(end)\n");
	close(fd);
}