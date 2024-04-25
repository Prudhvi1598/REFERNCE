
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<tc\emh.h>
#include<tccore\item.h>
#include<tccore\aom_prop.h>
#include<tccore\aom.h>
#include<tcinit\tcinit.h>
#include<tc/tc_startup.h>
#include<lov/lov.h>


#include<tccore/grm.h>
#include<lov\lov.h>
int check(exp) {
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
	char *cItem1 = NULL;
	tag_t tItem1 = NULLTAG;
	tag_t tLovTag = NULLTAG;

	LOV_usage_t tLovUsage = NULLTAG;
	char **cValue = NULL;//of
	int iNvalues = 0;
	//logical * //of
	//logical *//of
	char *cLovValue = ITK_ask_cli_argument("-value=");
	tag_t *tValues = NULL;
	check(ITK_init_module("hello", "hello", "dba"));// printf("Login Sucessfull");
	printf("Login Sucessfull\n");

	check(ITEM_find_item("dba-AA-023", &tItem1));     //Finding parent item tag
	if (tItem1 != NULLTAG)
	{

		printf("Item1 tag found\n");
		check(AOM_ask_lov(tItem1, "a4DescProp", &tLovTag));//Here I am Finding the tag of an LOV by passing Item tag & property(for this property lov is attached)
		if (tLovTag != NULLTAG)
		{
			printf("lov tag found\n");
			
			check(LOV_ask_values_string(tLovTag, &iNvalues, &cValue));//Finding all the values in the lov
			if (iNvalues != 0&& cValue!=NULL)
			{
				printf("%d\n", iNvalues);//printing count of values in the lov
				for (int i = 0; i < iNvalues; i++)
				{
					if (strcmp(cValue[i], cLovValue) == 0)//comparing each lov value with the given input
					{
						AOM_refresh(tItem1, true);// checking out the item
						check(AOM_set_value_string(tItem1, "a4DescProp", cLovValue));//if comparision is true then LOV value get updated

						AOM_save_without_extensions(tItem1);//After updating the LOV value saving item
						AOM_refresh(tItem1, false);//checking in the object
						printf("Value updated\n");
						MEM_free(cValue);//freeing memory
						exit(0);
					}
					else
					{
						printf("Value is not in LOv\n");

					}
					
				}
				
				
			}
			else
			{
				printf("no values in LOv\n");
			}
			
		}
		else
		{
			printf("LOv tag not found\n");
		}
		MEM_free(cValue);
	}
	else
	{
		printf("Item tag not found\n");
	}
	return 0;
	check(ITK_exit_module(true));
	printf("Logout sucessful\n");
}
	