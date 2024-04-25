/*Task2:

create dataset and attach to IR through ITK code*/

#include <ae/dataset.h>
#include <ae/datasettype.h>
#include<stdio.h>
#include<stdlib.h>
#include<tc\emh.h>
#include<tccore\item.h>
#include<tccore\aom.h>
#include<tcinit\tcinit.h>
#include<tccore\grm.h>
//#ifendif DATASET_H
#define DATASET_H
int ITK_user_main(int argc, char* argv[]) 
{
	int iFail = 0;
	char *cError = NULL;
	tag_t tRev = NULLTAG;

	tag_t *tNewDataset = NULLTAG;
	tag_t *tdataset_type = NULL;           
	tag_t tDataset =NULLTAG;

	tag_t tRelation = NULLTAG;
	tag_t tRelation_type = NULLTAG;
	iFail = ITK_init_module("hello", "hello", "dba");
	if (iFail == ITK_ok)
	{
		printf("Login Sucessfully\n");
		iFail = AE_find_datasettype2("PDF", &tDataset);
		if (iFail == ITK_ok)
		{
			printf("dataset found\n");
			iFail = AE_create_dataset_with_id(tDataset, "hi", "newdesc", "001112", "A", &tNewDataset);
			AE_save_myself(tNewDataset);
			if (iFail == ITK_ok)
			{
				printf("dataset created sucessfully");
				iFail = ITEM_find_rev("000141", "A", &tRev);
				if (iFail == ITK_ok)
				{
					printf("Item revision find\n");
					iFail = GRM_find_relation_type("IMAN_specification", &tRelation_type);
					if (iFail == ITK_ok)
					{
						printf("relation find\n");
						iFail = GRM_create_relation(tRev, tNewDataset, tRelation_type, NULLTAG, &tRelation);
						if (iFail == ITK_ok)
						{
							printf("Relation created\n");
							iFail = GRM_save_relation(tRelation);
						}
						else
						{

							EMH_ask_error_text(iFail, &cError);
							printf("The error is :%s\n", cError);
						}
					}
					else
					{

						EMH_ask_error_text(iFail, &cError);
						printf("The error is :%s\n", cError);
					}
				}
				else
				{

					EMH_ask_error_text(iFail, &cError);
					printf("The error is :%s\n", cError);
				}
			}
			else 
			{

				EMH_ask_error_text(iFail, &cError);
				printf("The error is :%s\n", cError);
			}
		}
		else 
		{

			EMH_ask_error_text(iFail, &cError);
			printf("The error is :%s\n", cError);
		}
	}
	else {

		EMH_ask_error_text(iFail, &cError);
		printf("The error is :%s\n", cError);
	}
	return iFail;
}


/*iFail = AE_create_dataset_with_id(tdataset_type, "hi", "newdesc", "001112", "A", &tNewDataset);
		if (iFail == ITK_ok) {
			printf("creation sucess\n");
		}*/