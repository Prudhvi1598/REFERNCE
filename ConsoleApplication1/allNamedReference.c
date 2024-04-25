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
int ITK_user_main(int argc, char* argv[])
{
	int iFail = 0;
	int i = 0;
	char *cError = NULL;
	char *values = NULL;
	tag_t tdataset = NULLTAG;
	int instance = 0;
	char **tReference_name = NULL;
	tag_t *tReferenced_object = NULLTAG;
	AE_reference_type_t  tReference_type = NULLTAG;
	//const char *destPathName = "D:\\import";

	iFail = ITK_init_module("hello", "hello", "dba");
	if (iFail == ITK_ok)
	{
		printf("Login Sucessfully\n");
		iFail = AE_find_dataset2("ITK_TA", &tdataset);
		if (iFail == ITK_ok)
		{
			printf("dataset find\n");
			iFail = AE_find_dataset_named_ref2(tdataset, instance, &tReference_name, &tReference_type, &tReferenced_object);
			
			if (iFail == ITK_ok)
			{
				printf("Named references found\n");
				iFail = AE_ask_all_dataset_named_refs(tdataset, tReference_name, &instance, &tReferenced_object);
				if (iFail == ITK_ok)
				{
					for (i = 0; i < instance; i++)
					{
						iFail = AOM_ask_value_string(tReferenced_object[i], "ref_list", &values);
						printf("named references are:%s", values[i]);
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
	return iFail;
}


/*iFail = AE_ask_all_dataset_named_refs(tdataset, tReference_name, &instance, &referenced_object);
			if (iFail == ITK_ok)
			{
				for (i = 0; i < instance; i++)
				{
					iFail = AOM_ask_value_string(referenced_object[i], "object_type", &values);
					printf("named references are:%s", values[i]);
				}
			}*/