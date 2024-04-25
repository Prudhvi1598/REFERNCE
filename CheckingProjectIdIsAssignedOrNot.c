#include<stdio.h>
#include<conio.h>
#include<tccore/custom.h>
#include<tccore/method.h>
#include<fclasses/tc_string.h>
#include<pom/pom/pom.h>
#include<epm/epm.h>
#include<tc/emh.h>
#include<tccore/grm.h>
#include<tccore/aom_prop.h>
#include<bom/bom.h>
#include<tccore/item.h>
#include<tccore/project.h>
#define DLLAPI _declspec(dllexport)
#define PLM_Error (EMH_USER_error_base +12)
#define PROJECT_LIST "project_list"
#define OBJECT_STING "object_name"

int iStatus = ITK_ok;
int iFlag = 0;
extern DLLAPI int DLL_PROJECT_register_callbacks();
extern DLLAPI int handler_execute_callbacks1(int *decision, va_list argv);
extern DLLAPI EPM_decision_t check_rule(EPM_rule_message_t msg);
extern DLLAPI int handler_execute_callbacks2(int *decision, va_list argv);
int check(int exp);
int ProjectId(tag_t tRevision, char *cInProjId);
int childline(tag_t tPBomLineTag, char *cInProjId);


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
	printf("\n\n\n----Rule Handler to check target and given project id's are same---- \n\n\n");

	EPM_register_rule_handler("Rule_Handler_Check_project_Id", "Rule_Handler_Check_project_Id", (EPM_rule_handler_t)check_rule);
	//Registers an action handler with the given handlerName.
	//1.Handler name
	//2.handler description
	//3.function pointer
	return iStatus;
}
extern DLLAPI EPM_decision_t check_rule(EPM_rule_message_t msg)
{
	int decision = 0;
	int iCount = 0;
	int *iAttachmentType = 0;

	tag_t tRootTask = NULLTAG;
	tag_t *tAttachments = NULL;
	tag_t tClassId = NULLTAG;
	char *cClassName = NULL;
	tag_t tPBomLineTag = NULLTAG;
	tag_t *tPBomViewTags = NULL;
	tag_t tItem = NULLTAG;
	int iBomViewCount = 0;
	tag_t tWindow = NULLTAG;
	int iNoOfArguments = 0;
	char *cArgumentName = NULL;
	char *cArgumentValue = NULL;
	char *cPid = NULL;
	check(EPM_ask_root_task(msg.task, &tRootTask));//Retrieves root task of the given task or job.
	if (tRootTask != NULLTAG)
	{
		printf("Root task found\n");
		iNoOfArguments = TC_number_of_arguments(msg.arguments);
		if (iNoOfArguments != 0)
		{
			for (int j = 0; j < iNoOfArguments; j++)
			{
				check(ITK_ask_argument_named_value(TC_next_argument(msg.arguments), &cArgumentName, &cArgumentValue));
				// Returns the current argument from the list of Teamcenter arguments
				if (tc_strcmp(cArgumentName, "pid") == 0)
				{
					cPid = (char*)MEM_alloc(sizeof(cArgumentName));
					tc_strcpy(cPid, "");
					tc_strcpy(cPid, cArgumentValue);
				}
			}
			if (cArgumentName)  MEM_free(cArgumentName); 
			if (cArgumentValue) MEM_free(cArgumentValue); 
		}
		check(EPM_ask_all_attachments(tRootTask, &iCount, &tAttachments, &iAttachmentType));//Retrieves all the objects attached to given task for given attachment type
		if (iCount != 0 && tAttachments != NULL && iAttachmentType != 0)
		{
			printf("Attachments found\n");
			for (int i = 0; i < iCount; i++)
			{
				check(POM_class_of_instance(tAttachments[i], &tClassId));//Finds out the class which "tAttachments[i]" belongs to
				if (tClassId != NULLTAG)
				{
					printf("class id found\n");
					check(POM_name_of_class(tClassId, &cClassName));//finds the name of the class
					if (cClassName != NULL)
					{
						printf("class name retrieved\n");
						//printf("class name=%s\n", cClassName);
						if (tc_strcmp(cClassName, "ItemRevision") == ITK_ok)
						{
							printf("Item revision found\n");
							
							if (ProjectId(tAttachments[i], cPid)==1)
							{
								decision = 1;
								check(ITEM_ask_item_of_rev(tAttachments[i], &tItem));
								if (tItem != NULLTAG)
								{
									printf("Item tag found\n");
									check(ITEM_list_bom_views(tItem, &iBomViewCount, &tPBomViewTags));//Finding list of BOM views for an item
									if (tPBomViewTags != NULL && iBomViewCount != 0)
									{
										printf("Bom view there\n");
										for (int k = 0; k < iBomViewCount; k++)
										{
											BOM_create_window(&tWindow);//creating BOM window
											BOM_save_window(tWindow);
											printf("Bom window created\n");
											if (tWindow != NULLTAG)
											{
												check(BOM_set_window_top_line(tWindow, tItem, tAttachments[i], tPBomViewTags[k], &tPBomLineTag));//Setting top line
												if (tPBomLineTag != NULLTAG)
												{
													printf("Bom line tag set\n");
													decision = childline(tPBomLineTag, cPid);
												}
												else
												{
													printf("BOMline tag not found\n");
												}
											}

										}
									}
									else
									{
										printf("The particular business object don't have bom views\n");
									}
									if (tPBomViewTags)  MEM_free(tPBomViewTags);
								}
							}
							else decision = 0;
						}
					}
					if (cClassName)  MEM_free(cClassName);
				}
			}
		}
		if (tAttachments)  MEM_free(tAttachments);
		if (iAttachmentType)  MEM_free(iAttachmentType);
	}

	if (decision == 1)
	{
		return EPM_go;
	}
	else
	{
		return EPM_nogo;
	}
	
}
int ProjectId(tag_t tRevision, char *cInProjId)
{
	int iList = 0;
	int k;
	char *cProjId = NULL;
	tag_t *tValue1 = NULL;
	char *cObjectName1 = NULL;
	AOM_ask_value_tags(tRevision, PROJECT_LIST, &iList, &tValue1);
	if (tValue1 != NULL && iList!=0)
	{
		for (k = 0; k < iList; k++)
		{
			check(PROJ_ask_id2(tValue1[k], &cProjId));
			if (cProjId != NULL)
			{
				if (tc_strcmp(cInProjId, cProjId) == ITK_ok)
				{
					printf("child value is matched\n");
					return 1;
					
				}
			}
		}
	}
	else
	{
		check(AOM_ask_value_string(tRevision, "object_string", &cObjectName1));
		if (cObjectName1 != NULL)
		{
			check(EMH_store_error_s1(EMH_severity_error, PLM_Error, cObjectName1));

		}
	}
	if (cObjectName1)  MEM_free(cObjectName1);
	if (tValue1)  MEM_free(tValue1);
	return 0;
 }

int childline(tag_t tPBomLineTag, char *cInProjId)
{
	int decision = 0;
	int iCount1 = 0;
	tag_t tRevision = NULLTAG;
	tag_t *tChildren = NULL;

	check(AOM_ask_value_tag(tPBomLineTag, "bl_revision", &tRevision));
	if (ProjectId(tRevision, cInProjId) == 1)
	{
		decision = 1;
		if (BOM_line_ask_child_lines(tPBomLineTag, &iCount1, &tChildren) == 0)
		{
			
			for (int i = 0; i < iCount1; i++)
			{
				decision = childline(tChildren[i], cInProjId);
				if (decision == 0) return decision;
			}
		}
		if (tChildren)  MEM_free(tChildren);
	}
	else
	{
		decision = 0;
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
