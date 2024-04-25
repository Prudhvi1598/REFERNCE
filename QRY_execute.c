#include<stdio.h>
#include<tcinit/tcinit.h>
#include<tc/tc_startup.h>
#include<bom/bom.h>
#include<qry/qry.h>
#include<tccore/workspaceobject.h>
#include "Header.h"

#define LOGIN ITK_init_module("izn","izn","dba")

int ITK_user_main(int argc, char** argv)
{
	int iFail = 0;
	int ifound = 0;
	tag_t tag_query = NULLTAG;
	tag_t *tag_results=NULL;
	char *entries[] = { "Name" };
	char *values[] = { "Home" };
	char *char_id;

	iFail = LOGIN;
	if (iFail == 0)
	{
		printf("TC Login successful!\n---------------------\n\n");
		CHECK(QRY_find2("0_foldercontents", &tag_query));
		CHECK(QRY_execute(tag_query, 1, entries, values, &ifound, &tag_results));
		if (ifound != 0)
		{
			printf("Query fetched succesfully!\n\n");
			for (int i = 0; i < ifound; i++)
			{
				WSOM_ask_id_string(tag_results[i], &char_id);
				printf("%s\n", char_id);
			}
		}
		else
		{
			printf("Query fetch failed!\n\n");
		}
	}
	else
	{
		printf("TC Login Failed!\n-------------------------\n\n");
	}
	return ITK_ok;
}
