#include<stdio.h>
#include<conio.h>
#include<string.h>
int ITK_user_main(int argc, char* argv[])
{
	FILE *fin;
	char c;
	fin = fopen_s(&fin,"C:\\Users\\T50315\\Desktop\\item.txt", "r");
	if (fin == NULL)
		return 1;

	int max = 0,i = 0;
	do
	{
		c = fgetc(fin);
		if (c != EOF && c != '\n')
			i++;
		else 
		{
			if (i > max) max = i;
			i = 0;
		}
	}
		
	while (c != EOF);
	return max;


	fclose(fin);
}
