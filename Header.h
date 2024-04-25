#pragma once

#include<stdio.h>
#include<tc/emh.h>
#include <stdlib.h>
#include <tccore/aom.h>
#include <epm/epm.h>
#include <sa/tcfile.h>

#define EXIT_FAILURE 1 
#define CHECK(X) (report_error( __FILE__, __LINE__, #X, (X)))
static void report_error(char *file, int line, char *function, int return_code)
{
	if (return_code != ITK_ok)
	{
		char *error_message_string;
		EMH_ask_error_text(return_code, &error_message_string);
		//EMH_get_error_text(return_code,0,0, &error_message_string);
		printf("ERROR: %d ERROR MSG: %s\n", return_code, error_message_string);
		printf("FUNCTION: %s\nFILE: %s LINE: %d\n", function, file, line);
		if (error_message_string) MEM_free(error_message_string);
		printf("\nExiting program!\n");
		exit(EXIT_FAILURE);
	}
}