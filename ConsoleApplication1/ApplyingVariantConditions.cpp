#include<iostream>
#include<tc/tc_startup.h>
#include<tcinit\tcinit.h>
#include<tc/emh.h>
#include<tccore/item.h>
#include<tc/envelope.h>
#include<sa/user.h>
#include<tc/folder.h>
#include<bom/bom.h>
#include<tc/tc_macros.h>
using namespace std;

int ITK_user_main(int argc, char* argv[])
{
	tag_t tItemTag = NULLTAG;
	int iCount = 0;
	int *iOptionCount;
	int iEntryCount= 1;
	char **cPaths = NULL;
	tag_t *tBvList = NULL;
	tag_t tWindow = NULLTAG;
	tag_t tBomLine = NULLTAG;
	//tag_t tOptionTag = NULLTAG;
	//tag_t tOptionRevTag = NULLTAG;
	tag_t tDbSos = NULLTAG;
	tag_t tSos = NULLTAG;
	tag_t tConfigRule = NULLTAG;
	const char *cValue = "plastic";
	char *cUser = ITK_ask_cli_argument("-u=");
	char *cPass = ITK_ask_cli_argument("-p=");
	char *cGroup = ITK_ask_cli_argument("-g=");
	char *cItemId = ITK_ask_cli_argument("-itemid=");

	if (cUser != NULL && cPass != NULL && cGroup != NULL)
	{
		ITKCALL(ITK_init_module(cUser, cPass, cGroup));
		cout << "Login sucessful\n" << endl;

		ITKCALL(ITEM_find_item(cItemId, &tItemTag));
		if (tItemTag != NULLTAG)
		{
			cout << "Item tag found\n" << endl;
			ITKCALL(ITEM_list_bom_views(tItemTag, &iCount, &tBvList));
			if (iCount != 0 && tBvList != NULL)
			{
				cout << "Bom views found" << endl;
				ITKCALL(BOM_create_window(&tWindow));
				cout << "Bom window created" << endl;

				for (int i = 0; i < iCount; i++)
				{

					ITKCALL(BOM_set_window_top_line(tWindow, tItemTag, NULLTAG, tBvList[i], &tBomLine));
					BOM_save_window(tWindow);
					
					
				}
			}
			

		}
	}

	return 0;
	ITK_exit_module(true);
}


/*
check(BOM_sos_ask_entries(tSos, &iEntryCount, &iOptionCount, &cPaths));
						if (iEntryCount != 0 && cPaths != NULL)
						{
							cout << "Entries found" << endl;
							for (int i = 0; i < iEntryCount; i++)
							{
								check(BOM_sos_set_entry_string(tSos, iOptionCount[i], "", "plastic", 2));
								cout << "option value set" << endl;
								check(BOM_sos_apply(tSos, true));
								cout << "Stored option set applied" << endl;
								break;
							}
							//BOM_line_ask_sos(tBomLine, &tSos);
							//check(BOM_create_variant_config(NULLTAG, 1, &tSos, &tConfigRule));
							//cout << "variant configuration created" << endl;


						}






						ITKCALL(BOM_line_ask_sos(tBomLine, &tSos));
					if (tSos != NULLTAG)
					{
						cout << "option tag found" << endl;
						ITKCALL(BOM_create_variant_config(NULLTAG, 1, &tSos, &tConfigRule));
						cout << "variant configuration created" << endl;
						//BOM_sos_db_create(tSos, tConfigRule)
						ITKCALL(BOM_sos_db_create("gani", tConfigRule, &tDbSos));
						cout << "db created" << endl;
						ITKCALL(BOM_sos_db_read(tDbSos, tConfigRule));
						cout << " Read the stored option set from the database" << endl;
						ITKCALL(BOM_sos_set_entry_string(tSos, 1, "", cValue, BOM_option_set_by_user));
						cout << " values set" << endl;
						BOM_save_window(tWindow);
						ITKCALL(BOM_variant_config_apply(tConfigRule));

						cout << "Variant rule applied" << endl;


					}
*/
