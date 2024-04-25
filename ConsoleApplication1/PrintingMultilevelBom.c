
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<tc\emh.h>
#include<tccore\item.h>
#include<tccore\aom_prop.h>
#include<tccore\aom.h>
#include<tcinit\tcinit.h>
#include<tc/tc_startup.h>
#include<ps\ps.h>
#include<bom\bom.h>
#include<fclasses\tc_string.h>
#include<tccore/grm.h>
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
int Func(tag_t tPBomLineTag);

int ITK_user_main(int argc, char* argv[])
{

	char *cItem1 = NULL;
	char *cItem2 = NULL;

	tag_t tItem1 = NULLTAG;
	tag_t tItem2 = NULLTAG;

	tag_t tRevision1 = NULLTAG;
	tag_t tRevision2 = NULLTAG;
	int count = 0;
	tag_t *tPlist = NULL;
	char *cValue = NULL;
	char *cValue1 = NULL;
	tag_t tWindow = NULLTAG;

	check(ITK_init_module("hello", "hello", "dba"));// printf("Login Sucessfull");
	printf("Login Sucessfull\n");

	check(ITEM_find_item("000244", &tItem1));     //Finding parent item tag
	if (tItem1 != NULLTAG)
	{
		printf("Item1 tag found\n");
		check(ITEM_find_rev("000244", "A", &tRevision1)); //Finding parent item Revision tag
		if (tRevision1 != NULLTAG)
		{
			printf("ItemRevision1 tag found\n");
			tag_t *tPBomViewTags = NULL;
			tag_t tPBomLineTag = NULLTAG;
			int i;
			check(ITEM_list_bom_views(tItem1, &count, &tPBomViewTags));//Finding list of BOM views for an item
			if (tPBomViewTags != NULL)
			{
				for (i = 0; i < count; i++)
				{

					check(BOM_create_window(&tWindow));//creating BOM window
					
					check(BOM_set_window_top_line(tWindow, tItem1, tRevision1, tPBomViewTags[i], &tPBomLineTag));//Setting top line
					if (tPBomLineTag != NULLTAG)
					{
						Func(tPBomLineTag);
					}
					else
					{
						printf("BOMline tag not found\n");
						return 0;
					}
					//Func(tPBomLineTag);

				}
			}
			else
			{
				printf("BOMview tags not found\n");
				return 0;
			}

		}
		else
		{
			printf("ItemRevision1 tag notfound\n");
			return 0;
		}
			
	}
		
	else
	{
		printf("Item tag notfound\n");
		return 0;
	}

	return 0;

}


int Func(tag_t tPBomLineTag)
{
	int count1 = 0;
		
		int i;
		char *cValue = NULL;
		tag_t *tChildren = NULL;
		tag_t tPBomLine = tPBomLineTag;
		check(AOM_ask_value_string(tPBomLine, "bl_indented_title", &cValue));//Asking title of BOM Line
		printf("%s\n", cValue);
	
		check(BOM_line_ask_child_lines(tPBomLine, &count1, &tChildren));//Asking child lines
		if (tChildren !=NULL)
		{
			for (i = 0; i < count1; i++)
			{
				tPBomLine = tChildren[i];//assigning tChildren[i] as topline
				Func(tPBomLine);// calling function recursively to check childs
			}
		}
		return 0;
}