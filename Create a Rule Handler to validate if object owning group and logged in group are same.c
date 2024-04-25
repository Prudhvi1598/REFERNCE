#include<stdio.h>
#include<conio.h>
#include<tccore/custom.h>
#include<tccore/method.h>
#include<fclasses/tc_string.h>
#include<tccore/aom_prop.h>
#include<tc/emh.h>
#include<tccore/item.h>
#include<epm/epm.h>
#include<bom/bom.h>
#include<tccore/releasestatus.h>
#include<pom/pom/pom.h>
#include<fclasses/tc_string.h>
#define OBJECT_TYPE "ItemRevision"

#define DLLAPI _declspec(dllexport)
#define PLM_Error (EMH_USER_error_base +6)
METHOD_id_t tMethodId;
int iStatus = ITK_ok;
extern DLLAPI int DLL_PROJECT_register_callbacks();
extern DLLAPI int handler_execute_callbacks1(int *decision, va_list argv);
extern DLLAPI int comparingLoginGroupAndObjectOwningGroup(EPM_rule_message_t msg);
extern DLLAPI int handler_execute_callbacks2(int *decision, va_list argv);
int check(int exp);


extern DLLAPI int DLL_PROJECT_register_callbacks()
{
	iStatus = CUSTOM_register_exit("DLL_PROJECT", "USER_init_module", (CUSTOM_EXIT_ftn_t)handler_execute_callbacks1);

	iStatus = CUSTOM_register_exit("DLL_PROJECT", "USER_exit_module", (CUSTOM_EXIT_ftn_t)handler_execute_callbacks2);
	return iStatus;
}
extern DLLAPI int handler_execute_callbacks1(int *decision, va_list argv)
{
	*decision = ALL_CUSTOMIZATIONS;
	printf("\n\n\n----welcome dll registration process---- \n\n\n");
	printf("\n\n\n----login sucess---- \n\n\n");
	printf("\n\n\n----Rule Handler for comparing Login Group And Object OwningGroup---- \n\n\n");

	EPM_register_rule_handler("Comparing_object_group_with_login_Group", "Comparing_object_group_with_login_Group", (EPM_rule_handler_t)comparingLoginGroupAndObjectOwningGroup);
	//Registers an action handler with the given handlerName.
	//1.Handler name
	//2.handler description
	//3.function pointer
	return iStatus;
}

extern DLLAPI int comparingLoginGroupAndObjectOwningGroup(EPM_rule_message_t msg)
{
	EPM_decision_t decision = EPM_nogo;
	tag_t tRootTask = NULLTAG;
	tag_t *tAttachments = NULL;
	int iCount = 0;
	int *iAttachmentType = 0;
	tag_t tGroupTag = NULLTAG;
	char *cObjectGroup = NULL;
	char *cUserName = NULL;
	tag_t tUserTag = NULLTAG;
	tag_t tLoginGroupTag = NULLTAG;
	char *cLoginGroup = NULL;
	check(EPM_ask_root_task(msg.task, &tRootTask));//Retrieves root task of the given task or job.
	if (tRootTask != NULLTAG)
	{
		check(EPM_ask_all_attachments(tRootTask, &iCount, &tAttachments, &iAttachmentType));//Retrieves all the objects attached to given task for given attachment type
		if (iCount != 0 && tAttachments != NULL && iAttachmentType != 0)
		{
			for (int i = 0; i < iCount; i++)
			{
				check(AOM_ask_value_tag(tAttachments[i], "owning_group", &tGroupTag));
				if (tGroupTag != NULLTAG)
				{
					
					printf("Group tag found\n");
					check(AOM_ask_value_string(tGroupTag, "display_name", &cObjectGroup));
					if (cObjectGroup != NULL)
					{
						POM_ask_group(&cLoginGroup, &tLoginGroupTag);

						if (cLoginGroup != NULL&& tLoginGroupTag!=NULLTAG)
						{
							if (tc_strcmp(cObjectGroup, cLoginGroup) == 0)
							{
								decision = EPM_go;
							}
							else
							{
								decision = EPM_nogo;
								check(EMH_store_error(EMH_severity_error, PLM_Error));
							}
								
							
						}
						if (cLoginGroup) MEM_free(cLoginGroup);
						
					}
					if (cObjectGroup) MEM_free(cObjectGroup);
				}
				
			}
		}
		if (iAttachmentType) MEM_free(iAttachmentType);
		if (iAttachmentType) MEM_free(iAttachmentType);

	}
	return decision;
}


extern DLLAPI int handler_execute_callbacks2(int *decision, va_list argv)
{
	*decision = ALL_CUSTOMIZATIONS;
	printf("\n\n\n----logout sucess---- \n\n\n");
	return iStatus;
}
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