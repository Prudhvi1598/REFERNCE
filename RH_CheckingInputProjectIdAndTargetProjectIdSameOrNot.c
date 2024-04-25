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
#define DLLAPI _declspec(dllexport)
#define PLM_Error (EMH_USER_error_base +12)
#define PROJECT_LIST "project_list"
#define OBJECT_STING "object_name"
#include<tccore/item.h>
#include<tccore/project.h>
int iStatus = ITK_ok;
extern DLLAPI int DLL_PROJECT_register_callbacks();
extern DLLAPI int handler_execute_callbacks1(int *decision, va_list argv);
extern DLLAPI EPM_decision_t check_rule(EPM_rule_message_t msg);
extern DLLAPI int handler_execute_callbacks2(int *decision, va_list argv);
int check(int exp);
int Func(tag_t tPBomLineTag,char *cInProjId);
int ProjectId(tag_t tRevision, char *cInProjId);

EPM_decision_t decision = EPM_go;

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

	EPM_register_rule_handler("Rule_Handler_Compare_project_Id", "Rule_Handler_Compare_project_Id", (EPM_rule_handler_t)check_rule);
	//Registers an action handler with the given handlerName.
	//1.Handler name
	//2.handler description
	//3.function pointer
	return iStatus;
}

extern DLLAPI EPM_decision_t check_rule(EPM_rule_message_t msg)
{

	
	tag_t tRootTask=NULLTAG;
	int iCount = 0;
	tag_t *tAttachments = NULL;
	int *iAttachmentType = 0;
	tag_t tClassId = NULLTAG;
	char *cClassName = NULL;
	char *cInProjId = "111";
	char *cValue = NULL;
	char *cObjectName = NULL;
	tag_t tPBomLineTag = NULLTAG;
	tag_t *tPBomViewTags = NULL;
	tag_t tItem = NULLTAG;
	int iBomViewCount = 0;
	tag_t tWindow = NULLTAG;
	int j;
	check(EPM_ask_root_task(msg.task, &tRootTask));//Retrieves root task of the given task or job.
	if (tRootTask != NULLTAG)
	{
		printf("Root task found\n");
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
							//tag_t rev = tAttachments[i];
							ProjectId(tAttachments[i], cInProjId);
							check(ITEM_ask_item_of_rev(tAttachments[i], &tItem));
							if (tItem != NULLTAG)
							{
								printf("Item tag found\n");
								check(ITEM_list_bom_views(tItem, &iBomViewCount, &tPBomViewTags));//Finding list of BOM views for an item
								if (tPBomViewTags != NULL && iBomViewCount != 0)
								{
									printf("Bom view there\n");
									for (j = 0; j < iBomViewCount; j++)
									{

										BOM_create_window(&tWindow);//creating BOM window
										BOM_save_window(tWindow);
										printf("Bom window created\n");
										if (tWindow != NULLTAG)
										{
											check(BOM_set_window_top_line(tWindow, tItem, tAttachments[i], tPBomViewTags[j], &tPBomLineTag));//Setting top line
											if (tPBomLineTag != NULLTAG)
											{
												printf("Bom line tag set\n");
												Func(tPBomLineTag, cInProjId);
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
							}

						}
					}
				}
			}
		}
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


int Func(tag_t tPBomLineTag, char *cInProjId)
{
	int count1 = 0;
	int i;
	tag_t tChildTag1 = NULLTAG;
	tag_t *tValue1 = NULL;
	tag_t *tChildren = NULL;
	tag_t tPBomLine = tPBomLineTag;
	char *cObjectName1 = NULL;
	char *cValue2 = NULL;
	char *cPid = NULL;

	check(BOM_line_ask_child_lines(tPBomLine, &count1, &tChildren));//Asking child lines
	if (tChildren != NULL)
	{
		for (i = 0; i < count1; i++)
		{
			check(AOM_ask_value_tag(tChildren[i], "bl_revision", &tChildTag1));//Asking title of BOM Line
			if (tChildTag1 != NULLTAG)
			{
				AOM_ask_value_string(tChildTag1, "object_string", &cValue2);
				printf("%s\n", cValue2);
				ProjectId(tChildTag1,cInProjId);
			}
			tPBomLine = tChildren[i];//assigning tChildren[i] as topline
			Func(tPBomLine,cInProjId);// calling function recursively to check childs
		}
	}
	return decision;
}

int ProjectId(tag_t tRevision, char *cInProjId)
{
	int iList = 0;
	int k;
	char *cPid = NULL;
	tag_t *tValue1 = NULL;
	tag_t tPBomLine = NULLTAG;
	char *cObjectName1 = NULL;
	AOM_ask_value_tags(tRevision, PROJECT_LIST, &iList, &tValue1);

	if (tValue1 != NULL && iList != 0)
	{
		for (k = 0; k < iList; k++)
		{
			check(PROJ_ask_id2(tValue1[k], &cPid));
			if (cPid != NULL)
			{
				if (tc_strcmp(cInProjId, cPid) == ITK_ok)
				{
					printf("child value is matched\n");
					//decision = EPM_go;
					return 0;
					break;
				}
				else
				{
					check(AOM_ask_value_string(tPBomLine, "bl_indented_title", &cObjectName1));
					if (cObjectName1 != NULL)
					{
						check(EMH_store_error_s1(EMH_severity_error, PLM_Error, cObjectName1));
						return 1;
					}
				}
			}
		}
	}
	return 0;
	
}

