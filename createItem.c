
#include<stdio.h>
#include<tc/tc_startup.h>
#include<tcinit/tcinit.h>
#include<tccore/item.h>
#include<tccore/aom.h>
#include<tc/folder.h>
#include<sa/user.h>


int ITK_user_main(int argc, char* argv[]) {
	/* Variables */
	int iFail = 0;
	tag_t titem = NULLTAG;
	tag_t  trev = NULLTAG;
	tag_t  tuser = NULLTAG;
	tag_t  tfolder = NULLTAG;
	/* to login */
	iFail = ITK_init_module("infodba", "infodba", "dba");
	if (iFail== ITK_ok)
	{
		printf("Login Successful!\n...........................\n");
		/* Find user Home Folder to create Item */
		SA_find_user2("izn", &tuser);
		if (tuser != NULLTAG)
		{
			printf("User Found!\n\n");
			SA_ask_user_home_folder(tuser, &tfolder);
			if (tfolder != NULLTAG)
			{
				printf("Home Folder Found!\n\n");
				ITEM_create_item("", "KumaraSwamy1", "Item", "A", &titem, &trev);
				if (titem != NULLTAG)
				{
					ITEM_save_item(titem);
					printf("Item created successfully!\n\n");
					iFail=FL_insert(tfolder, titem, 999);
					AOM_save_without_extensions(tfolder);
					if (iFail == 0)
					{
						printf("Item Saved in Home Folder!\n\n");
					}
					else
						printf("Item cannot be Saved in Home Folder!\n\n");
				}
				else
				{
					printf("Item creation Failed!\n\n");
				}
			}
			else
			{
				printf("Home Folder NotFound!\n\n");
			}
		}
		else
		{
			printf("User NotFound!\n\n");
		}
	}
	else
	{
		printf("Login Unsuccessful!\n");
	}
		return ITK_ok;
}
