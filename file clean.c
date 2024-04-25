#include <stdio.h>
#include <ctype.h>
#include<tcinit/tcinit.h>

#define LOGIN ITK_init_module("izn","izn","dba")

int ITK_user_main(int argc, char** argv)
{
	int iFail = 0;
	iFail = LOGIN;
	if (iFail == 0)
	{
		printf("TC Login successful!\n---------------------\n\n");
		FILE *inputFile, *outputFile;
		char* inputFileName= "C:\\Users\\T50361\\Desktop\\Assignments\\test.txt"; 
		char* outputFileName= "C:\\Users\\T50361\\Desktop\\Assignments\\new.txt";
		char c;
		char lastChar;

		// Open the input file for reading
		inputFile = fopen(inputFileName, "r");
		if (inputFile == NULL) {
			printf("Error opening input file.\n");
			return 1;
		}

		// Open the output file for writing
		outputFile = fopen(outputFileName, "w");
		if (outputFile == NULL) {
			printf("Error opening output file.\n");
			fclose(inputFile);
			return 1;
		}

		// Read characters from the input file and write cleaned text to the output file
		while ((c = fgetc(inputFile)) != EOF) 
		{
			if ((isalpha(c) || isdigit(c)) && (c!='P' && c!='C')|| (c == ',' && lastChar != ',' && lastChar != '\0')) 
			{
				fputc(c, outputFile);
				lastChar = c;
			}
			if (c == '\n') {
				fputc('\n', outputFile);
			}
		}

		// Close the files
		fclose(inputFile);
		fclose(outputFile);

		printf("Cleaning completed. Cleaned text saved in %s\n", outputFileName);
	}
	else
	{
		printf("TC Login Failed!\n-------------------------\n\n");
	}
	return 0;
}
