#include<stdio.h>
#include<tcinit/tcinit.h>
#include<tc/tc_startup.h>
#include<bom/bom.h>
#include<ae/dataset.h>
#include<tccore/aom.h>
#include<tccore/item.h>
#include<tccore/grm.h>

#define LOGIN ITK_init_module("izn","izn","dba")

int ITK_user_main(int argc, char** argv)
{
	int iFail = 0;
	tag_t trev = NULLTAG;
	tag_t tdataset = NULLTAG;
	tag_t trelation = NULLTAG;
	tag_t trelationtype = NULLTAG;
	iFail = LOGIN;
	if (iFail == 0)
	{
		printf("TC Login successful!\n---------------------\n\n");
		ITEM_find_rev("000332", "001", &trev);
		if (trev != NULLTAG)
		{
			printf("ItemRevision Found!\n\n");
			AE_find_dataset2("hi", &tdataset);
			if (tdataset != NULLTAG)
			{
				printf("dataset Found!\n\n");
				GRM_find_relation_type("IMAN_specification", &trelationtype);
				GRM_find_relation(trev, tdataset, trelationtype, &trelation);
				if (trelation != NULLTAG)
				{
					printf("Relation found Succesfully!\n\n");
					iFail=GRM_delete_relation(trelation);
					if (iFail == 0)
					{
						printf("Relation deleted succesfully!");
					}
					else
					{
						printf("Relation deleted failed!");
					}
				}
				else
				{
					printf("Relation Not found!\n\n");
				}
			}
			else
			{
				printf("dataset Not Found!\n\n");
			}
		}
		else
		{
			printf("ItemRevision Not Found!");
		}
	}
	else
	{
		printf("TC Login Failed!\n----------------\n\n");
	}
	return ITK_ok;
}