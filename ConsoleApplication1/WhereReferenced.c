/*
-----To find the item where it is used-----

Login in into TEam center Module
Find Item Revision
By using API find where it is referenced(WSOM_where_referenced2)
you will get tags of all referencers and relations based on
Iterate the loop to get all the names of referencers
then Use API(AOM_ask_value_string) to get the property values(In below scenario Iam printing object_name)
print the values

*/
#include<tc/tc_startup.h>
#include<tcinit\tcinit.h>
#include<conio.h>
#include<stdio.h>
#include<stdlib.h>
#include<tc\emh.h>
#include<tccore\item.h>
#include<tccore\aom.h>
#include<sa\user.h>
#include<sa\group.h>
#include<tccore\aom_prop.h>
#include<ps\ps.h>
int ITK_user_main(int argc, char* argv[])
{
	int iFail = 0;
	int i;
	char *cError = NULL;
	tag_t tItemRevision = NULLTAG;
	/*char *user = ITK_ask_cli_argument("-u=");
	char *pass = ITK_ask_cli_argument("-p=");
	char *group = ITK_ask_cli_argument("-g=");*/
	int iDepth = -1;
	int *iLevels;
	int iReferencers = 0;
	char **cRelations = NULL;
	tag_t *tReferencers = NULLTAG;
	char *cValues = NULL;
	char *cRelValues = NULL;


	tag_t *tParents = NULLTAG;
	char *cPvalues = NULL;
	iFail = ITK_init_module("hello", "hello", "dba");//Login into Teamcenter based on parameters passed
	if (iFail == ITK_ok)
	{
		printf("LoginSucessfully\n");
		iFail = ITEM_find_rev("000217","A", &tItemRevision);//Finding item revision
		if (iFail == ITK_ok)
		{
			printf("IemRev find Find\n");
			iFail = WSOM_where_referenced2(tItemRevision, iDepth, &iReferencers, &iLevels,&tReferencers, &cRelations);//here we are finding the tags of all the referencers
			if (iFail == ITK_ok)
			{
				for (i = 0; i < iReferencers; i++)
				{
					iFail = AOM_ask_value_string(tReferencers[i], "object_name", &cValues);//here by passing tags of referencers we are getting the property values i.e., Object_name
					printf("WHERE REFERENCED\n");
					if (iFail == ITK_ok)
					{
						
						printf("%d %s %s\n", iLevels[i],cValues, cRelations[i]);//printing the level and referencers along with the relation how it is referenced
			
					}
					else//if if condition fails else block will execute
					{

						EMH_ask_error_text(iFail, &cError);
						printf("The error is :%s\n", cError);
					}
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