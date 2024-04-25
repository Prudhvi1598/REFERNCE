/*
-------UPDATING THE NAME OF THE NAMED REFERENCES----

LOGIN INTO TEAMCENTER MODULE
FIND DATASET TAG BY GIVING NAME OF DATASET
FIND ALL NAMED REFERENCES OF DATASET
COMPARE THE NAMED REFERENCE OF THE DATASET WITH THE NAME YOU WANT TO CHANGE
	IF COMPARISION SUCESS
		SET VALUE
	ELSE ITERATE LOOP

	-u=hello -p=hello -g=dba -datasetname=Names -oldname=ITK2.pdf -newname=ITK3.pdf 
*/


#include<iostream>
#include<tc/tc_startup.h>
#include<tcinit\tcinit.h>
#include<tc/emh.h>
#include<tccore/item.h>
#include<tccore/project.h>
#include<tccore/aom.h>
#include<tccore/aom_prop.h>
#include<ae/dataset.h>
#define REFERENCE_TYPE "PDF_Reference"
#define OBJECT_STRING "object_string"
#define NAME "original_file_name"
using namespace std;
int check(int exp);
int ITK_user_main(int argc, char* argv[])
{
	tag_t tDatasetTag = NULLTAG;
	int iCount = 0;
	char *cValue = NULL;
	
	tag_t *tReferencedObjects = NULL;
	char *cUser = ITK_ask_cli_argument("-u=");
	char *cPass = ITK_ask_cli_argument("-p=");
	char *cGroup = ITK_ask_cli_argument("-g=");
	const char *cDatasetName = ITK_ask_cli_argument("-datasetname=");
	
	const char *cOldName = ITK_ask_cli_argument("-oldname=");
	const char *cNewName = ITK_ask_cli_argument("-newname=");
	if (cUser != NULL && cPass != NULL && cGroup != NULL)
	{
		check(ITK_init_module(cUser, cPass, cGroup));
		cout << "Login sucessful\n" << endl;
		check(AE_find_dataset2(cDatasetName, &tDatasetTag));
		if (tDatasetTag != NULLTAG)
		{
			cout << "Dataset tag found" << endl;
			check(AE_ask_all_dataset_named_refs2(tDatasetTag, REFERENCE_TYPE, &iCount, &tReferencedObjects));
			if (iCount != 0 && tReferencedObjects != NULL)
			{
				cout << "Dataset references found" << endl;
				for (int i = 0; i < iCount; i++)
				{
					check(AOM_ask_value_string(tReferencedObjects[i], OBJECT_STRING, &cValue));
					if (cValue != NULL)
					{
						cout << "the value is: " << cValue << endl;
						if (strcmp(cValue, cOldName) == 0)
						{
							AOM_refresh(tReferencedObjects[i], true);
							check(AOM_set_value_string(tReferencedObjects[i], NAME, cNewName));
							cout << "value updated" << endl;
							AOM_save_without_extensions(tReferencedObjects[i]);
							AOM_refresh(tReferencedObjects[i], false);
							break;
						}
						else
						{
							cout << "Comparision failed" << endl;
						}
					}
				}
			}
		}
	}
	else 
	{
		cout << "Enter valid credentials" << endl;
	}
	return 0;
	ITK_exit_module(true);
}

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

