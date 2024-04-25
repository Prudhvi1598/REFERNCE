#include<conio.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<tc\emh.h>
#include<tc/tc_startup.h>
#include<tcinit\tcinit.h>
#include<tccore\aom.h>
#include<tccore\aom_prop.h>
#include<tccore\grm.h>
#include<tccore\tctype.h>
#include<tccore\item.h>

#define ITEM_ID "item_id"
#define OBJECT_NAME	"object_name"
#define TYPE "Item"

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
	char *cItemid = NULL;
	char *cItemName = NULL;
	tag_t tItemid = NULLTAG;
	tag_t tCreateInput = NULLTAG;
	tag_t tBusinessObject = NULLTAG;
	tag_t tType = NULLTAG;
	FILE *fp;
	fp = fopen("D:\\CreatingMultipleItems.txt", "r");
	char cBuff[100];
	char* cFileContent;
	char *cUser = ITK_ask_cli_argument("-u=");
	char *cPass = ITK_ask_cli_argument("-p=");
	char *cGroup = ITK_ask_cli_argument("-g=");
	check(ITK_init_module(cUser, cPass, cGroup));

	char *cToken = NULL;
	char *cArray[100];
	int i=0;
	int size = 100;
	char *ptr;
	char a[5][20];
	printf("Login Sucessfull\n");
	while (fgets(cBuff, 100, fp) != NULL)      //Reading file line by line
	{
		ptr= strtok(cBuff, ",");
		while (ptr != NULL) 
		{
			*a[i] = ptr;
			i++;
			printf("%s\n", ptr);
			ptr = strtok(NULL, ",");
		}
		
		
	}
	int j;
	for (j = 0; i; j++)
	{
		printf("%s\n", a[j]);
	}
	return 0;

}
