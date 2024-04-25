/*
--------TASK-----



*/

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
#include<base_utils\Mem.h>
//#define
//FUNCTION TO CHECK IFAIL 
int change(exp) {
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
	FILE *fp;
	int iFaill = 0;
	int i = 0;
	char line[10];
	char *lines[10];

	char cItem1[10];
	char cItem2[10];
	//char *cItem3 = NULL;

	tag_t tItemTag = NULLTAG;
	char *cValue = NULL;
	char cSearchId = NULL;

	tag_t tItem1 = NULLTAG;
	tag_t tItem2 = NULLTAG;
	//tag_t tItem3 = NULLTAG;

	tag_t tRevision1 = NULLTAG;
	tag_t tRevision2 = NULLTAG;
	//tag_t tRevision3 = NULLTAG;

	tag_t tViewType = NULLTAG;
	const char *cViewName = NULL;
	const char *cViewDesc = NULL;
	tag_t tParentItem = NULLTAG;
	tag_t tBomView = NULLTAG;

	const char *cRevName = NULL;
	const char *cRevDesc = NULL;
	tag_t tBvr = NULLTAG;

	tag_t tWindow = NULLTAG;

	tag_t tTopBomLine = NULLTAG;

	tag_t tNewLine = NULLTAG;

	char *cVAlue = NULL;

	change(ITK_init_module("hello", "hello", "dba"));// printf("Login Sucessfull");
	printf("Login Sucessfull\n");
	fp = fopen("D:\\log.txt", "r");
	if (fp == NULL)
	{
		printf("file not opened\n");
		//exit(0);
	}
	fgets(cItem1, 10, fp);
	cItem1[strcspn(cItem1, "\n")] = 0;
	fgets(cItem2, 10, fp);
	cItem2[strcspn(cItem2, "\n")] = 0;
	printf("File opened in read mode\n");

	/*cItem1 = (char*)malloc(10 * sizeof(char));
	cItem2 = (char*)malloc(10 * sizeof(char));
	cItem3 = (char*)malloc(10 * sizeof(char));*/
	
	while (fp!=EOF)
	{
		if (i == 0)
		{
			fscanf(fp, "%s", &cItem1);
		}
		else if(i==1)
		{
			fscanf(fp, "%s", &cItem2);
			break;
		}
		i++;
	}
	printf("value of item1 is :%s", cItem1);
	printf("value of item2 is :%s", cItem2);
	//cItem3 = strtok(NULL, "\n");
		//printf("value of item1 is :%s\n", cItem3);
	
	change(ITEM_find_item(cItem1, &tItem1));
	AOM_ask_value_string(tItem1,"object_name",cVAlue);
	printf("%s\n", cVAlue);
	printf("\nItem1 tag found\n");
	change(ITEM_find_item(cItem2, &tItem2));
	printf("Item2 tag found\n");
	//change(ITEM_find_item(cItem3, &tItem3));
	//printf("Item3 tag found\n");

	change(ITEM_find_rev(cItem1, "A", &tRevision1));
	printf("ItemRevision1 tag found\n");
	change(ITEM_find_rev(cItem2, "A", &tRevision2));
	printf("ItemRevision2 tag found\n");
	//change(ITEM_find_rev(cItem3, "A", &tRevision3));
	//printf("ItemRevision3 tag found\n");

	/*change(PS_create_bom_view(tViewType, cViewName, cViewDesc, tItem1, &tBomView));//Creating BOM view of default type
	AOM_save_without_extensions(tBomView);
	AOM_save_without_extensions(tItem1);
	printf("Bom view creatted\n");

	change(PS_create_bvr(tBomView, cRevName, cRevDesc, false, tRevision1, &tBvr));
	AOM_save_without_extensions(tBvr);
	AOM_save_without_extensions(tRevision1);
	printf("BOM view REvision Created\n");

	change(BOM_create_window(&tWindow));
	printf("BOM window created\n");
	change(BOM_save_window(tWindow));
	printf("BOM window saved\n");

	change(BOM_set_window_top_line(tWindow, tItem1, tRevision1, NULLTAG, &tTopBomLine));
	printf("BOM window top line created\n");

	change(BOM_line_add(tTopBomLine, tItem2, tRevision2, NULLTAG, &tNewLine));

	change(BOM_save_window(tWindow));
	AOM_save_without_extensions(tRevision2);
	printf("BOM line added\n");
	printf("BOM window again saved\n");
	fclose(fp);
	return 0;*/
}