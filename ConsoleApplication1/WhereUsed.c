/*
extern PS_API int PS_where_used_all(
	tag_t         target,           < (I) The object whose parents are sought. 
int           n_levels,         (I) Depth of search. #PS_where_used_all_levels means
										report all grandparents up to top level products
									(for example, those assemblies which are not part of any larger assembly). 
	int*          n_parents,        < (O) Total number of parent Item Revisions found. 
	int**         levels,           < (OF) n_parents Array containing the level of each parent found,
											starting from 1 for an immediate parent of the specified Item Revision. 
	tag_t**       parents            (OF) n_parents Array of tags of Occurrence Groups and/or Item Revisions (if @p target is an Occurrence Group), or
											only Item Revisions (if @p target is an Item Revision). 
	);
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
	int iNlevels=1;
	int iNparents;
	int *iLevels;
	tag_t *tParents = NULL;
	char *cValues = NULL;
	
	/*char *user = ITK_ask_cli_argument("-u=");
	char *pass = ITK_ask_cli_argument("-p=");
	char *group = ITK_ask_cli_argument("-g=");*/
	iFail = ITK_init_module("hello", "hello", "dba");
	if (iFail == ITK_ok)
	{
		printf("LoginSucessfully\n");
		iFail = ITEM_find_rev("000217", "A", &tItemRevision);
		if (iFail == ITK_ok)
		{
			printf("IemRev find Find\n");
			iFail = PS_where_used_all(tItemRevision, iNlevels, &iNparents, &iLevels, &tParents);
			if (iFail == ITK_ok)
			{
				for (i = 0; i < iNparents;i++)
				{
					iFail = AOM_ask_value_string(tParents[i], "object_name", &cValues);
					if (iFail == ITK_ok)
					{
						printf("%s\n", cValues);
					}
					else
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