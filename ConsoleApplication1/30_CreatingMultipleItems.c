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
	fp = fopen("D:\\CreatingMultipleItems.txt","r");
	char cBuff[100];
	char* cFileContent;
	char *cUser = ITK_ask_cli_argument("-u=");
	char *cPass = ITK_ask_cli_argument("-p=");
	char *cGroup = ITK_ask_cli_argument("-g=");
	check(ITK_init_module(cUser, cPass, cGroup));
	//char cArray[100];
	printf("Login Sucessfull\n");
	while (fgets(cBuff, 100, fp) != NULL)      //Reading file line by line
	{
		cFileContent = strtok(cBuff, "\n");    //Removing new line character from a line
		cItemid = strtok(cFileContent, ",");    //Tokenizing
		printf("%s\n", cItemid);

		cItemName = strtok(NULL, ",");
		printf("%s\n", cItemName);
		tag_t tItemTag = NULLTAG;
		check(ITEM_find_item(cItemid, &tItemTag));//Finding Item Tag
		if(tItemTag != NULLTAG)                   //checking whether the item already exist
		{
			printf("%s id already exist\n", cItemid);
			continue;
		}
		check(TCTYPE_ask_type(TYPE, &tType));//Retrieves the type tag from the provided type name.
		if (tType != NULLTAG)
		{
			check(TCTYPE_construct_create_input(tType, &tCreateInput));//Here we construct item
			if (tCreateInput != NULLTAG)
			{
				check(AOM_set_value_string(tCreateInput, ITEM_ID, cItemid));//Here we are updating the item_id value
				printf("Item Id set Sucess\n");
				check(AOM_set_value_string(tCreateInput, OBJECT_NAME, cItemName));//Here we are updating the OBJECT_NAME value
				printf("Item Name set Sucess\n");
				check(TCTYPE_create_object(tCreateInput, &tBusinessObject));//Here we are creating the object based on the above input
				if (tBusinessObject != NULLTAG)
				{
					printf("object created\n");
					AOM_save_without_extensions(tBusinessObject);//Saving the BUsiness object
				}
				
			}
		}
		
	}
	fclose(fp);
	return 0;

}
