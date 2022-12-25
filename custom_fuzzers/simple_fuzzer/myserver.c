#include <stdio.h>
#include <string.h>

int main()
{
	int to_copy = 0;
	char buffer[100];

	while(1)
	{
		int n = read(0, buffer, 100);
		if(strncmp(buffer, "VERSION", 7) == 0)
		{
			puts("wow\n");
		}
		else if(strncmp(buffer, "ECHO", 4) == 0)
		{
			printf("BOF - %d\n", strlen(buffer));
			memcpy(&to_copy, buffer, strlen(buffer));
			return 0;
		}
	}


	return 0x0;
}