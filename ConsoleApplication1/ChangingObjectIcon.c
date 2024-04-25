#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<tc\emh.h>
#include<tccore\item.h>
#include<tccore\aom_prop.h>
#include<tccore\aom.h>
#include<tcinit\tcinit.h>
#include<tc/tc_startup.h>
#include<tccore/grm.h>
#include<pom/pom/pom.h>
#include<Cls0classification/cls_itk.h>
#define OBJECT_TYPE "object_type"
int check(int exp);
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
	char *cUser = ITK_ask_cli_argument("-u=");
	char *cPass = ITK_ask_cli_argument("-p=");
	char *cGroup = ITK_ask_cli_argument("-g=");
	if (cUser != NULL && cPass != NULL && cGroup != NULL)
	{
		check(ITK_init_module(cUser, cPass, cGroup));
		tag_t tClassId = NULLTAG;
		tag_t tIconTag = NULLTAG;
		POM_class_id_of_class("A4Object2Revision", &tClassId);
		if (tClassId != NULLTAG)
		{
			CLS_import_hierarchy_node_icon(tClassId, "C:\\Users\\T50315\\Desktop\\apple.jpg", "A4.newimage", false, &tIconTag);
			if (tIconTag != NULLTAG)
			{
				printf("icon imported\n");
			}
			else
			{
				printf("icon not imported\n");
			}
		}
	}
	return 0;
}