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
//#define
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
void Fun(cItem1, cItem2)
{
	
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
	cItem1 = (char*)malloc(10 * sizeof(char)); //ptr = (cast-type*) malloc(byte-size) || Ass
	cItem2 = (char*)malloc(10 * sizeof(char));
	

	char *buff = NULL;
	char *token = NULL;
	while (fgets(buff, CHAR_MAX, fp) != NULL)
	{

		buff = strtok(buff, "\n"); // 011,12112,121212
		token = strtok(buff, ",");
		while (token != NULL)
		{
			if (iFlag == 1)
			{
				printf("The details of file are : %s\n", buff);

				cItem1 = token;
				printf("value of item is :%s\n", cItem1);

				check(ITEM_find_item(cItem1, &tItem1));
				printf("Item1 tag found\n");

				check(ITEM_find_rev(cItem1, "A", &tRevision1));
				printf("ItemRevision1 tag found\n");

				check(PS_create_bom_view(tViewType, cViewName, cViewDesc, tItem1, &tBomView));
				AOM_save_without_extensions(tBomView);
				AOM_save_without_extensions(tItem1);
				printf("Bom view creatted\n");

				check(PS_create_bvr(tBomView, cRevName, cRevDesc, false, tRevision1, &tBvr));
				AOM_save_without_extensions(tBvr);
				AOM_save_without_extensions(tRevision1);
				printf("BOM view REvision Created\n");

				check(BOM_create_window(&tWindow));
				printf("BOM window created\n");
				check(BOM_save_window(tWindow));
				printf("BOM window saved\n");

				check(BOM_set_window_top_line(tWindow, tItem1, tRevision1, NULLTAG, &tTopBomLine));
				printf("BOM window top line created\n");
				token = strtok(NULL, ",");
				//printf("value of item1 is :%s\n", cItem2);
				//fgetc(buff);
				iFlag = 0;
				continue;
			}
			else
			{
				cItem2 = token;
				printf("value of item is :%s\n", cItem2);
				check(ITEM_find_item(cItem2, &tItem2));
				printf("Item2 tag found\n");
				check(ITEM_find_rev(cItem2, "A", &tRevision2));
				printf("ItemRevision2 tag found\n");

				check(BOM_line_add(tTopBomLine, tItem2, tRevision2, NULLTAG, &tNewLine));
				printf("BOM line added\n");
				check(BOM_save_window(tWindow));
				AOM_save_without_extensions(tRevision2);

				printf("BOM window again saved\n");
				token = strtok(NULL, ",");
			}
			
			//iFlag = 1;
		}
	}
			
}
		//Fun();
	//fgetc(fp);

int ITK_user_main(int argc, char* argv[])
{
	check(ITK_init_module("hello", "hello", "dba"));// printf("Login Sucessfull");
	printf("Login Sucessfull\n");
	char *cItem1 = NULL;
	char *cItem2 = NULL;
	FILE *fp;
	int iFlag = 1;
	fp = fopen("D:\\rec.txt", "r");
	char *buff[CHAR_MAX];
	while (fgets(buff, CHAR_MAX, fp) != EOF)
	{

		printf("The details of file are : %s\n", buff);
		if (iFlag == 1)
		{
			cItem1 = strtok(buff, ",");
			printf("value of item is :%s\n", cItem1);
			
		}
		cItem1 = strtok(buff, ",");
		printf("value of item is :%s\n", cItem1);
		cItem2 = strtok(NULL, ",");
		printf("value of item1 is :%s\n", cItem2);
	}
	Fun(cItem1, cItem2);
	return 0;
}