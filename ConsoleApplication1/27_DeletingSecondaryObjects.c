#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<tc\emh.h>
#include<tccore\item.h>
#include<tccore\aom_prop.h>
#include<tccore\aom.h>
#include<tcinit\tcinit.h>
#include<tc/tc_startup.h>
#include<tccore/grm.h>
#define OBJECT_TYPE "object_type"
int check(exp) {
	int iFail = 0;
	char *cError = NULL;
	if (exp != iFail)
	{
		EMH_ask_error_text(iFail, &cError);
		printf("The error is :%s\n", cError);
		exit(0);
	}
	return 0;
}
int ITK_user_main(int argc, char* argv[])
{
	char *cItem1 = NULL;
	tag_t tItemRev = NULLTAG;
	int iCount = 0;
	int i;
	tag_t *tSecondaryObjects = NULL;
	char *cValue = NULL;
	char *cInputValue = "DocumentRevision";
	char *cInputRelation = "IMAN_specification";
	char *cUser = ITK_ask_cli_argument("-u=");
	char *cPass = ITK_ask_cli_argument("-p=");
	char *cGroup = ITK_ask_cli_argument("-g=");
	check(ITK_init_module(cUser, cPass, cGroup));
	printf("Login Sucessfull\n");

	char *cItem = ITK_ask_cli_argument("-itemid=");
	check(ITEM_find_rev(cItem, "A", &tItemRev));
	if (tItemRev != NULLTAG)
	{
		printf("Item rev tag found\n");
		check(GRM_list_secondary_objects_only(tItemRev, NULLTAG, &iCount, &tSecondaryObjects));
		if (iCount != 0)
		{
			for (i = 0; i < iCount; i++)
			{
				check(AOM_ask_value_string(tSecondaryObjects[i], OBJECT_TYPE, &cValue));
				
					if (cValue != NULL)
					{
						printf("%s\n", cValue);

						if (strcmp(cValue, cInputValue) == 0)
						{
							
							tag_t tRelationType = NULLTAG;
							check(GRM_find_relation_type(cInputRelation, &tRelationType));
							if (tRelationType != NULLTAG)
							{
								printf("relation type tag found\n");
								tag_t tRelation = NULLTAG;
								check(GRM_find_relation(tItemRev, tSecondaryObjects[i], tRelationType, &tRelation));
								if (tRelation != NULLTAG)
								{
									check(GRM_delete_relation(tRelation));
									printf("relation deleted\n");
									break;
								}
							}
						}
					}
				
			}
			
			MEM_free(cValue);
		}
		MEM_free(tSecondaryObjects);
	}
	return 0;
	ITK_exit_module(true);
}