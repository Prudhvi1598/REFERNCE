#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include<tcinit/tcinit.h>
#include<tc/tc_startup.h>
#include<tccore/workspaceobject.h>
#include<tccore/aom_prop.h>
#include<bom/bom.h>
#include "Header.h"

#define LOGIN ITK_init_module("izn","izn","dba")
#define size 3

int ITK_user_main(int argc, char** argv)
{
	int iFail = 0;
	int i=0;
	tag_t titem = NULLTAG;
	char *cvalue;
	FILE *ptr;
	char *citem[size];
	char *line[100];

	iFail = LOGIN;
	if (iFail == 0)
	{
		printf("TC Login successful!\n---------------------\n\n");
		CHECK(fopen("C:\Users\T50361\Desktop\Assignments\Read.txt", "r"));
		//ptr = fopen("Read.txt", "r");
		fgets(line, 100, ptr);
		if (ptr != NULL)
		{
			printf("File opened to Read!\n\n");
			char* token = strtok(line, "-");
			citem[i] = token;
			while (token != NULL)
			{
				token = strtok(NULL, "-");
				//citem[++i] = token;
				ITEM_find_item(token, &titem);
				//iFail = AOM_set_value_string(titem, "object_desc", "Hi");
				iFail = WSOM_set_description2(titem, "Hi");
				if (iFail == 0)
					printf("Object desc set successful!\n");
				else
					printf("Object desc set failed!\n");
			}
			/*for (int j = 0; j < size; j++)
			{
				printf("%s\n", citem[j]);
				ITEM_find_item(citem[j], &titem);
				iFail=AOM_set_value_string(titem, "object_desc", "Hi");
				iFail=WSOM_set_description2(titem, "Hi");
				
				titem = NULLTAG;
				cvalue = NULL;
			}*/
		}
		else
		{
			printf("File not found\n\n");
		}
			
	}
	else
	{
		printf("TC Login Failed!\n-------------------------\n\n");
	}
	return ITK_ok;
}
