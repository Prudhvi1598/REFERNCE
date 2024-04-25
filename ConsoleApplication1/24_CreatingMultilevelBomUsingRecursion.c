/*
first create a single level bom
 
 give a child id to add sub childs



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
#include<fclasses\tc_string.h>


#define OBJECT_NAME "bl_indented_title"
//FUNCTION TO CHECK IFAIL 
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
int Func(rec)
{
	
	//char buff[CHAR_MAX];

	char *cItem1 = NULL;
	char *cItem2 = NULL;


	tag_t tItemTag = NULLTAG;
	char *cValue = NULL;


	tag_t tItem1 = NULLTAG;
	tag_t tItem2 = NULLTAG;

	tag_t tRevision1 = NULLTAG;
	tag_t tRevision2 = NULLTAG;


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

	int iFlag = 1;

	char *cChildValue = NULL;

	
	cItem1 = (char*)malloc(10 * sizeof(char)); //ptr = (cast-type*) malloc(byte-size) || Ass
	cItem2 = (char*)malloc(10 * sizeof(char));
	//cItem3 = (char*)malloc(100 * sizeof(char));
	FILE *fp;
	fp = fopen("D:\\log.txt", "r");
	printf("File opened in read mode\n");
	while (fscanf(fp, "%[^\n]", cItem1) != EOF)
	{
		if (iFlag == 1)
		{
		
			strcpy(cItem1, cItem1);

			check(ITEM_find_item(cItem1, &tItem1));     //Finding parent item tag
			if (tItem1 != NULLTAG)
				printf("Item1 tag found\n");

			check(ITEM_find_rev(cItem1, "A", &tRevision1)); //Finding parent item Revision tag
			if (tRevision1 != NULLTAG)
				printf("ItemRevision1 tag found\n");

			check(PS_create_bom_view(tViewType, cViewName, cViewDesc, tItem1, &tBomView));//Creating BOM view of default type
			AOM_save_without_extensions(tBomView);                                        //Saving Bom view
			AOM_save_without_extensions(tItem1);                                        //Saving Item1
			printf("Bom view creatted\n");

			check(PS_create_bvr(tBomView, cRevName, cRevDesc, false, tRevision1, &tBvr));//Creating BOM view Revision
			AOM_save_without_extensions(tBvr);                                            //Saving BOM view Revision
			AOM_save_without_extensions(tRevision1);                                      //Saving Parent Revision
			printf("BOM view Revision Created\n");

			check(BOM_create_window(&tWindow));                                          //Creating the BOM window
			printf("BOM window created\n");
			check(BOM_save_window(tWindow));                                            //Saving the BOM window
			printf("BOM window saved\n");

			check(BOM_set_window_top_line(tWindow, tItem1, tRevision1, NULLTAG, &tTopBomLine));      //Setting the Bom top line
			printf("BOM window top line created\n");



		}
		else
		{
			strcpy(cItem2, cItem1);                         //Assigning the cItem1 to the cItem2
			printf("value of item2 is :%s\n", cItem2);
			check(ITEM_find_item(cItem2, &tItem2));
			if (tItem2 != NULLTAG)
				printf("Item tag found\n");

			check(ITEM_find_rev(cItem2, "A", &tRevision2));
			if (tRevision2 != NULLTAG)
				printf("ItemRevision tag found\n");

			check(BOM_line_add(tTopBomLine, tItem2, tRevision2, NULLTAG, &tNewLine));
			printf("BOM line added\n");
			check(BOM_save_window(tWindow));
			AOM_save_without_extensions(tRevision2);

			printf("BOM window again saved\n");


		}
		iFlag++;

		fgetc(fp);//Get rid of the new line character
	}
	return 0;
	check(ITK_exit_module(true));
}

int ITK_user_main(int argc, char* argv[])
{
	
	check(ITK_init_module("hello", "hello", "dba"));// printf("Login Sucessfull");
	printf("Login Sucessfull\n");
	
	check(Func());
	return 0;
}