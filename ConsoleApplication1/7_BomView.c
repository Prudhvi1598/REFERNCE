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
	char buff[50];

	char *cItem1 = NULL;
	char *cItem2 = NULL;
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

	change(ITK_init_module("hello", "hello", "dba"));// printf("Login Sucessfull");
	printf("Login Sucessfull\n");
	//fp = fopen("D:\\log.txt", "r");
	//printf("File opened in read mode\n");

	////cItem1 = (char*)malloc(10 * sizeof(char)); //ptr = (cast-type*) malloc(byte-size)
	////cItem2 = (char*)malloc(10 * sizeof(char));

	////fgets(buff, CHAR_MAX, fp);
	//

	////cItem3 = (char*)malloc(100 * sizeof(char));
	//while (fgets(buff, 50,fp)!=NULL)
	//{
	//	
	//	printf("The details of file are : %s\n", buff);
	//	cItem1 = strtok(buff, "\n");
	//	printf("value of item1 is :%s\n", cItem1);
	//	cItem2 = strtok(NULL, "\n");
	//	printf("value of item2 is :%s\n", cItem2);
	//	//cItem3 = strtok(NULL, ",");
	//	//printf("value of item1 is :%s\n", cItem3);
	//}
	//change(ITEM_find_item(cItem1,&tItem1));
	//printf("Item1 tag found\n");
	//change(ITEM_find_item(cItem2, &tItem2));
	//printf("Item2 tag found\n");
	////change(ITEM_find_item(cItem3, &tItem3));
	////printf("Item3 tag found\n");

	change(ITEM_find_rev(cItem1, "A", &tRevision1));
	printf("ItemRevision1 tag found\n");
	//change(ITEM_find_rev(cItem2, "A", &tRevision2));
	//
	//printf("ItemRevision2 tag found\n");
	////change(ITEM_find_rev(cItem3, "A", &tRevision3));
	////printf("ItemRevision3 tag found\n");

	//change(PS_create_bom_view(tViewType, cViewName, cViewDesc, tItem1, &tBomView));//Creating BOM view of default type
	//AOM_save_without_extensions(tBomView);
	//AOM_save_without_extensions(tItem1);
	//printf("Bom view creatted\n");

	//change(PS_create_bvr(tBomView, cRevName, cRevDesc, false, tRevision1, &tBvr));//Creating BOM view Revision
	//AOM_save_without_extensions(tBvr);                                            //Saving BOM view Revision
	//AOM_save_without_extensions(tRevision1);                                      //Saving Parent Revision
	//printf("BOM view REvision Created\n");

	//change(BOM_create_window(&tWindow));                                          //Creating the BOM window
	//printf("BOM window created\n");
	//change(BOM_save_window(tWindow));                                            //Saving the BOM window
	//printf("BOM window saved\n");

	//change(BOM_set_window_top_line(tWindow, tItem1, tRevision1, NULLTAG, &tTopBomLine));      //Setting the Bom top line
	//printf("BOM window top line created\n");

	//change(BOM_line_add(tTopBomLine, tItem2, tRevision2, NULLTAG, &tNewLine));               
	//printf("BOM line added\n");
	//change(BOM_save_window(tWindow));
	//AOM_save_without_extensions(tRevision2);

	//printf("BOM window again saved\n");
	//change(ITK_exit_module(true));
	//fclose(fp);
	return 0;
}