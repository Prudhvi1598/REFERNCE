#include<stdio.h>
#include<tccore/aom.h>
#include<tcinit/tcinit.h>
#include<tc/tc_startup.h>
#include<ae/dataset.h>
#include<ae/datasettype.h>

#define user "infodba"
#define pwd "infodba"
#define grp "dba"
#define SS_TEXT   010000

int ITK_user_main(int argc, char** argv)
{
	int iFail = 0;
	tag_t tdataset = NULLTAG;
	iFail = ITK_init_module(user, pwd, grp);
	if (iFail == ITK_ok)
	{
		printf("TC Login Successful!");
		AE_find_dataset2("PDF", &tdataset);
		if(tdataset != NULLTAG)
		{
			printf("\n\ndataset found!");
			AOM_refresh(tdataset, true);
			AE_import_named_ref(tdataset, "PDF_Reference", "C:\\Users\\T50361\\Desktop\\Assignments\\ME-setup.pdf", "ITK1234", SS_TEXT);
			AOM_save_without_extensions(tdataset);
			AOM_refresh(tdataset, false);
			printf("\n\n NamedReference Added Succesfully!\n\n");
		}
		else
		{
			printf("\n\nDataset NOtFound\n\n");
		}
	}
	else
	{
		printf("TC Login Unsuccessful");
	}
	
	return ITK_ok;
}