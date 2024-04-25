#include<stdio.h>
#include<tcinit/tcinit.h>
#include<tccore/item.h>
#include<tccore/aom.h>
#include<tccore/aom_prop.h>
#include<tc/tc_startup.h>

#define user "infodba"
#define pwd "infodba"
#define grp "dba"

int ITK_user_main(int argc, char** argv)
{
	int iFail = 0;
	int ipropcount;
	int ivaluecount;
	char **cpropnames;
	char **cpropvalues;
	FILE *fptr;
	tag_t trevision = NULLTAG;
	iFail = ITK_init_module(user, pwd, grp);
	if (iFail == ITK_ok)
	{
		printf("TC Login Successful!\n\n");
		ITEM_find_rev("000321", "A", &trevision);
		if (trevision != 0)
		{
			printf("ItemRevision found!\n\n");
			fptr = fopen("C:\\Users\\T50361\\Desktop\\Assignments\\Property_list_ITK.txt", "w");
			if (fptr != NULL)
			{
				printf("File opened!\n\n");
				AOM_ask_prop_names(trevision, &ipropcount, &cpropnames);
				for (int i = 0; i < ipropcount; i++)
				{
					AOM_ask_displayable_values(trevision, cpropnames[i], &ivaluecount, &cpropvalues);
					for (int j = 0; j < ivaluecount; j++)
					{
						printf("The %s has value:- %s\n", cpropnames[i], cpropvalues[j]);
						fprintf(fptr, "The %s has value:- %s\n", cpropnames[i], cpropvalues[j]);
					}
				}
				//MEM_free(cpropnames);
				fclose(fptr);
				printf("File saved succesfully!");
			}
			else
			{
				printf("File Not created!\n\n");
			}
		}
		else
		{
			printf("ItemRevision Notfound!\n\n");
		}
	}
	else
	{
		printf("TC Login Unsuccessful");
	}
	return ITK_ok;
}