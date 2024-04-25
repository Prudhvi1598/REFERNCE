#include<iostream>
#include<string>
#include<cstring>

#include<conio.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<tc\emh.h>
#include<tc/tc_startup.h>
#include<tcinit\tcinit.h>
#include<tccore\aom_prop.h>
#include<tccore\item.h>
#include<tccore/aom.h>
#include<tccore/grm.h>
#include<epm/epm_task_template_itk.h>
#include<epm/epm.h>
#include<fclasses/tc_string.h>

//int check(int exp);
using namespace std;
int check(int exp) {
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
	tag_t tItemTag = NULLTAG;
	int iCount = 0;
	tag_t tRevTag = NULLTAG;
	int tTaskCount = 0;
	tag_t *tTasks = NULL;
	tag_t *tSecObjects = NULL;
	int i;
	tag_t ProcessTag = NULLTAG;
	int iTargetAttaType = EPM_target_attachment;
	char *cUser = ITK_ask_cli_argument("-u=");
	char *cPass = ITK_ask_cli_argument("-p=");
	char *cGroup = ITK_ask_cli_argument("-g=");
	const char *cSearchId = ITK_ask_cli_argument("-searchid=");
	const char *cRevId = ITK_ask_cli_argument("-revid=");
	const char *cTempName = ITK_ask_cli_argument("-tempname=");
	char *cFilePath= ITK_ask_cli_argument("-log=");
	const char *cSyslog = ".SYSLOG";
	//char *cfileName= ITK_ask_cli_argument("-filename=");
	
	
	char *cFullpath = NULL;
	FILE *fp;
	string sHelp = "<-u=userid> <-p=password> <-g=group> <-searchid=id of object> <-revid=revid> <-tempname=Template name> <-filepath=path> <-filename=filename>";
	
	//const char *cHelpFormat = "<-u=userid> <-p=password> <-g=group> <-searchid=id of object> <-revid=revid> <-tempname=Template name> <-filepath=path> <-filename=filename>";
	if (ITK_ask_cli_argument("-h"))
	{
		//printf("%s\n", cHelpFormat);
		cout << sHelp;
	}
	if (cUser != NULL)
	{
		if (cPass != NULL)
		{
			if (cGroup != NULL)
			{
				ITK_init_module(cUser, cPass, cGroup);
				int len = sizeof(cFilePath) + sizeof(cSearchId);
				cFullpath = (char*)malloc(len * sizeof(char));//allocating the memory dynamically
				tc_strcpy(cFullpath, cFilePath);

				tc_strcat(cFullpath, cSearchId);
				tc_strcat(cFullpath, cSyslog);
				printf("%d\n", len);
				printf("%s\n", cFullpath);
				fp=fopen(cFullpath, "w+");
				fprintf(fp, "the path is %s\n", cFullpath);
				
				fprintf(fp,"Login successfull\n");
				//TC_write_syslog

				check(ITEM_find_rev(cSearchId, cRevId, &tRevTag));//finding item revision
				if (tRevTag != NULLTAG)
				{
					fprintf(fp,"Revision tag found\n");
					Func(cTempName, cFullpath, tRevTag);

					check(GRM_list_secondary_objects_only(tRevTag, NULLTAG, &iCount, &tSecObjects));//Listing secondary objects
					if (tSecObjects != NULL)
					{
						fprintf(fp,"Secondary objects listed\n");
						char *c = &tSecObjects;
						Func(cTempName, cFullpath, c);
						
					}
				}
			}
			else
			{
				printf("Invalid group\n %s\n", cHelpFormat);
			}
			MEM_free(tSecObjects);
			
		}
		else
		{
			printf("Invalid password\n %s\n", cHelpFormat);
		}

	}
	else
	{
		printf("Invalid user\n %s\n", cHelpFormat);
	}
	//fclose(fp);
	return 0;
	
}

int Func(char* cTempName,char *cFullpath, tag_t tRevTag)

{
	FILE *fp = fopen(cFullpath, "a+");
	int i;
	tag_t tTempTag = NULLTAG;
	int iCount = 0;
	int	iTargetAttaType = EPM_target_attachment;
	tag_t ProcessTag = NULLTAG;
	tag_t*	tSecObjects = NULL;
	check(EPM_find_process_template(cTempName, &tTempTag));//finding process template tag
	if (tTempTag != NULLTAG)
	{
		fprintf(cFullpath, "Template name found\n");
		check(EPM_create_process(cTempName, "TCM Release Process", tTempTag, 1, &tRevTag, &iTargetAttaType, &ProcessTag));//Initiating process for item revision
		fprintf(fp, "Item rvision released\n");
		for (i = 0; i < iCount; i++)
		{
			check(EPM_create_process(cTempName, "TCM Release Process", tTempTag, 1, &tSecObjects[i], &iTargetAttaType, &ProcessTag));//Initiating process for secondary objects
			if (ProcessTag != NULLTAG)
			{
				fprintf(fp, "Process initiaed\n");
			}
		}

	}
	return 0;
}