#include <stdio.h>
#include<conio.h>
//#include <string.h>
#include<fclasses/tc_string.h>

void read_numbers(FILE *fp, int *arr, int size) {
	char line[100]; // Declare a buffer to store a line of text
	int *num, i = 0; // Declare variables to store a number and an index

	// Read a line of text from the file
	if (fgets(line, sizeof(line), fp) != NULL) {
		// Tokenize the line using commas as the delimiter
		char *token = tc_strtok(line, ",");
		while (token != NULL) {
			// Convert the token to an int
			scanf(token, "%s", &num);
			// Store the number in the array
			arr[i] = *num;
			i++;
			// Get the next token
			token = tc_strtok(NULL, ",");
		}
		// Recursively call the function to read the next line
		read_numbers(fp, arr, size);
	}
}

int ITK_user_main(int argc, char* argv[]) {
	int arr[10]; // Declare an array to store the numbers
	FILE *fp;

	fp = fopen("D:\\rec.txt", "r"); // Open the text file for reading

	// Call the function to read the numbers
	read_numbers(fp, arr, 10);

	printf("The numbers are: ");
	for (int i = 0; i < 10; i++) {
		printf("%d ", arr[i]);
	}
	printf("\n");

	fclose(fp); // Close the file

	return 0;
}