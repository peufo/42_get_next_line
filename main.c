#include "get_next_line.h"
#include <fcntl.h>
#include <stdio.h>

int main(void)
{

	int fd = open("test.txt", O_RDONLY);

	char *values[10];
	int	index = 0;

	while ((values[index] = get_next_line((fd))))
	{
		printf("%d -> \"%s\"\n", index, values[index]);
		index++;
	}
	printf("%d -> \"%s\"\n", index, values[index]);

}