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

int check(exp)
{
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
	
	tag_t tDataset = NULLTAG;
	int iInstance = 1;
	char *tReference_name = "PDF_Reference";
	AE_reference_type_t  tReference_type = NULLTAG;
	tag_t tReferenced_object = NULLTAG;
	
	char *cUser = ITK_ask_cli_argument("-u=");
	char *cPass = ITK_ask_cli_argument("-p=");
	char *cGroup = ITK_ask_cli_argument("-g=");
	char *cDatasetName = ITK_ask_cli_argument("-datasetName=");
	char *cPath = ITK_ask_cli_argument("-path=");
	char *cFileName = ITK_ask_cli_argument("-FileName=");
	check(ITK_init_module(cUser, cPass, cGroup));
	printf("Login Sucessfully\n");
	check(AE_find_dataset2(cDatasetName, &tDataset));
	if (tDataset != NULLTAG)
	{
		printf("dataset find\n");
		check(AE_find_dataset_named_ref2(tDataset, iInstance, &tReference_name, &tReference_type, &tReferenced_object));
		if (tReference_name != NULL && tReference_type != NULLTAG && tReferenced_object != NULLTAG)
		{
			printf("Named references found\n");
			check(AOM_refresh(tDataset, true));
			printf("checkout success\n");
			check(AE_import_named_ref(tDataset, tReference_name, cPath, cFileName, SS_BINARY));
			printf("Named references imported\n");
			check(AOM_save_without_extensions(tDataset));
			check(AOM_refresh(tDataset, false));
			printf("checked in\n");
		}
		else
		{
			printf("Tags not found\n");
		}
	}
	else
	{
		printf("Dataset tags not found\n");
	}
	return 0;
	check(ITK_exit_module(true));
}