#include<stdio.h>
#include<tcinit/tcinit.h>
#include<tc/tc_startup.h>
#include<tccore/aom.h>
#include<sa/user.h>
#include<tc/folder.h>
#include<bom/bom.h>
#include<qry/qry.h>
#include<tc/envelope.h>
#include <fclasses/tc_time.h>
#include <fclasses/tc_date.h>


#define LOGIN ITK_init_module("infodba","infodba","dba")

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
	char *entries[] = { "Created Before","Created After" };
	char *values[] = { "06-Sep-2023 06:00","06-Sep-2023 17:00" };


	iFail = LOGIN;
	if (iFail == 0)
	{
		printf("TC Login successful!\n---------------------\n\n");
		QRY_find2("Item...", &query);
		QRY_execute(query, 2, entries, values, &found, &results);
		FL_create2("Mail", NULL, &new_folder);
		for (int i = 0; i < found; i++)
		{
			FL_insert(new_folder, results[i], 999);
			AOM_save_without_extensions(new_folder);
		}
		MAIL_create_envelope("Items", "new Items", &envelop);
		MAIL_initialize_envelope2(envelop, "subject", "comments");
		SA_find_user2("izn", &user);
		MAIL_add_envelope_receiver(envelop, user);
		FL_insert(envelop, new_folder,999);
		MAIL_send_envelope(envelop);
	}
	else
	{
		printf("TC Login Failed!\n-------------------------\n\n");
	}
	return ITK_ok;
}