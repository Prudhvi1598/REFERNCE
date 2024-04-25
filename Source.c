#include<stdio.h>
#include<time.h>
#include<tcinit/tcinit.h>
#include<tc/tc_startup.h>
#include<tccore/aom.h>
#include<tccore/aom_prop.h>
#include<sa/user.h>
#include<tc/folder.h>
#include<bom/bom.h>
#include<qry/qry.h>
#include<tc/envelope.h>
#include <fclasses/tc_time.h>
#include <fclasses/tc_date.h>
#include <fclasses/tc_string.h>
#include "Header.h"


#define LOGIN ITK_init_module("izn","izn","dba")

int ITK_user_main(int argc, char** argv)
{
	int iFail = 0;
	int found = 0;
	tag_t query = NULLTAG;
	tag_t envelop = NULLTAG;
	tag_t* results = NULL;
	tag_t tag_rev = NULLTAG;
	tag_t user = NULLTAG;
	tag_t tag_home = NULLTAG;
	tag_t new_folder = NULLTAG;
	char *entries[] = { "Created After" };
	char *item_ids = (char*)malloc(sizeof(char) * 50);
	char* date_str = NULL;
	char* value = NULL;
	char* cResult;
	cResult = (char*)malloc(50 * sizeof(char));
	time_t tTime;
	struct tm *tmp;


	iFail = LOGIN;
	if (iFail == 0)
	{
		printf("TC Login successful!\n---------------------\n\n");

		time(&tTime);
		tmp = localtime(&tTime);
		printf("\n\n The current time and date is %s\n", ctime(&tTime));
		strftime(cResult, 50, "%d-%b-%Y", tmp);
		printf("\nReq date is %s\n\n", cResult);

		char** ValueArray;
		ValueArray = (char**)malloc(50 * sizeof(char**));
		ValueArray[0] = (char*)malloc(sizeof(char)*(strlen(cResult) + 1));
		tc_strcpy(ValueArray[0], cResult);

		CHECK(QRY_find2("Item...", &query));
		CHECK(QRY_execute(query, 1, entries, ValueArray, &found, &results));
		CHECK(FL_create2("Mail", NULL, &new_folder));
		for (int i = 0; i < found; i++)
		{
			CHECK(AOM_ask_value_string(results[i], "object_string", &value));
			if(i==0)
				tc_strcpy(item_ids, value);
			else
			{
				tc_strcat(item_ids, ",");
				tc_strcat(item_ids, value);
			}
			FL_insert(new_folder, results[i], 999);
			AOM_save_without_extensions(new_folder);
		}
		CHECK(MAIL_create_envelope("Mail", item_ids, &envelop));
		CHECK(SA_find_user2("izn", &user));
		CHECK(MAIL_add_envelope_receiver(envelop, user));
		CHECK(FL_insert(envelop, new_folder, 1));
		CHECK(MAIL_send_envelope(envelop));
	    CHECK(AOM_save_without_extensions(envelop));
		MEM_free(item_ids);
		MEM_free(ValueArray);
	}
	else
	{
		printf("TC Login Failed!\n-------------------------\n\n");
	}
	return ITK_ok;
}