#include <ae/dataset.h>
#include <ae/datasettype.h>
#include<stdio.h>
#include<stdlib.h>
#include<tc\emh.h>
#include<tccore\item.h>
#include<tccore\aom.h>
#include<tcinit\tcinit.h>
#include<tc/tc_startup.h>
#include<ss\ss_const.h>

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
	int iFail = 0;
	char *cError = NULL;
	tag_t tdataset = NULLTAG;
	int instance = 1;
	char *tReference_name = NULL;
	AE_reference_type_t  tReference_type = NULLTAG;
	tag_t tReferenced_object = NULLTAG;
	const char *cUser = ITK_ask_cli_argument("-u=");
	const char *cPass = ITK_ask_cli_argument("-p=");
	const char *cGroup = ITK_ask_cli_argument("-g=");
	const char *cDataset = ITK_ask_cli_argument("-dataset=");
	const char *cPath= ITK_ask_cli_argument("-path=");

	if (cUser != NULL && cPass != NULL && cGroup != NULL)
	{
		check(ITK_init_module(cUser, cPass, cGroup));
		printf("Login Sucessfully\n");
		check(AE_find_dataset2(cDataset, &tdataset));
		if (tdataset != NULLTAG)
		{
			printf("dataset find\n");
			check(AE_find_dataset_named_ref2(tdataset, instance, &tReference_name, &tReference_type, &tReferenced_object));
			if (tReference_name != NULLTAG && tReference_type != NULLTAG && tReferenced_object != NULLTAG)
			{
				printf("Named references found\n");
				AOM_refresh(tdataset, true);
				printf("Checked out\n");
				check(AE_export_named_ref(tdataset, tReference_name, cPath));
				printf("Named references exported\n");

			}
			else
			{
				printf("Tags not found\n");
			}
		}
		else
		{
			printf("Dataset Tag not found\n");
		}
	}
	else
	{
		printf("Enter all values(itemid,itemName,itemType,revId\n)");
	}
	return 0;
}