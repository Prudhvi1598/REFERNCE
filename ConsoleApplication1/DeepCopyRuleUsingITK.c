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
#include<bom/bom.h>
#include<fclasses/tc_string.h>
#define OBJECT_TYPE "object_type"
int check(int exp);
int check(int exp) {
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
	tag_t tItem = NULLTAG;
	tag_t tNewRev = NULLTAG;
	int iCount = 0;
	tag_t *tRevList = NULL;
	int iDeepCopiedObj = 0;
	tag_t *tDeepCopiedObjs = NULL;
	int iSecCount = 0;
	tag_t *tSecObjects = NULL;
	tag_t tag = NULLTAG;
	const char *copy_option = { ITEM_no_copy };
	check(ITK_init_module("hello", "hello", "dba"));
	ITEM_find_item("000360", &tItem);
	if (tItem != NULLTAG)
	{
		ITEM_list_all_revs(tItem, &iCount, &tRevList);
		if (tRevList != NULL)
		{
			ITEM_copy_rev(tRevList[iCount-1], NULL, &tNewRev);
			if (tNewRev != NULLTAG)
			{
				GRM_list_secondary_objects_only(tRevList[iCount - 1], NULLTAG, &iSecCount, &tSecObjects);
				for (int i = 0; i < iSecCount; i++)
				{
					tag = tSecObjects[i];
					ITEM_perform_deepcopy_using(tNewRev, ITEM_revise_operation, tRevList[iCount - 1], 1, tag,copy_option[0],"",NULL,&iDeepCopiedObj,&tDeepCopiedObjs);
				}
				
				
				printf("Deepcopy performed\n");
				
				
			}
		}
		
	}
	return 0;
}