#include<stdio.h>
#include<tc/tc_startup.h>
#include<tcinit/tcinit.h>
#include<ae/dataset.h>
#include<ae/datasettype.h>
#include<tccore/aom.h>
#include<tccore/item.h>
#include<tccore/grm.h>

#define User  "infodba"
#define Password "infodba"
#define group "dba"

int ITK_user_main(int argc, char* argv[]) 
{
	int iFail = 0;
	tag_t tdatasetTag = NULLTAG;
	tag_t tdataset = NULLTAG;
	tag_t trev = NULLTAG;
	tag_t trelationtype = NULLTAG;
	tag_t tnewrelation = NULLTAG;

	/*TC login */
	iFail = ITK_init_module(User, Password, group);
	if (iFail == ITK_ok)
		printf("TC Login Succesful!\n\n");
	else
		printf("TC Login Failed\n\n");

	/* To find Datasettype Tag */
	iFail=AE_find_datasettype2("PDF", &tdatasetTag);
	if (iFail == ITK_ok && tdatasetTag !=NULLTAG)
		printf("Dataset Tag found Succesful!\n\n");
	else
		printf("Dataset Tag found Unsuccesful\n\n");

	/* To create dataset */
	iFail = AE_create_dataset_with_id(tdatasetTag, "GRM", "Dataset created through ITK", NULL, "A", &tdataset);
	AOM_save_without_extensions(tdataset);     /* To save dataset */
	if (iFail == ITK_ok)
		printf("Dataset Created Succesfully!\n\n");
	else
		printf("Dataset creation Failed\n\n");

	/* To find the Item revision and create GRM */
	iFail = ITEM_find_rev("000273", "A", &trev);
	if (iFail == ITK_ok)
	{
		GRM_find_relation_type("IMAN_specification", &trelationtype); //
		GRM_create_relation(trev, tdataset, trelationtype, NULLTAG, &tnewrelation); //
		GRM_save_relation(tnewrelation);
		if (tnewrelation != NULLTAG)
		{
			printf("GRM created succesfully!");
		}
		else
		{
			printf("GRM creation Failed!");
		}
	}
	else
	{
		printf("ItemRevision NotFound");
	}
 
	return iFail;
} 