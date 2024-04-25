//Rule handler task:
//Create rule handler to check any property on bom line, if property filled for all child's then u should initiate the WF otherwise it should show custom error.
/*
register rule handler

ask root task
ask all attachments
find pom class of instance
find class name
compare class name
if ok
	by using item revision that we got in attachments find item
	find item bom views
	create window
	save window
	set top line
	list all childs
	check bom property of all childs
	if all filled 
		workflow should initiate
	else
		print custom error
*/

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
#include<tccore/item.h>
#include<bom/bom.h>
#include<tccore/workspaceobject.h>
#define OBJECT_TYPE "ItemRevision"
#define DLLAPI _declspec(dllexport)
#define PLM_Error (EMH_USER_error_base +9)


int iStatus = ITK_ok;
extern DLLAPI int DLL_PROJECT_register_callbacks();
extern DLLAPI int handler_execute_callbacks1(int *decision, va_list argv);
extern DLLAPI EPM_decision_t check_rule(EPM_rule_message_t msg);
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
	printf("\n\n\n----Rule Handler with single argument registered---- \n\n\n");

	EPM_register_rule_handler("Check_If_Prop_Of_BomChild_Is_Filled", "Check_If_Prop_Of_BomChild_Is_Filled", (EPM_rule_handler_t)check_rule);
	//Registers an action handler with the given handlerName.
	//1.Handler name
	//2.handler description
	//3.function pointer
	return iStatus;
}
extern DLLAPI EPM_decision_t check_rule(EPM_rule_message_t msg)
{
	EPM_decision_t decision = EPM_nogo;
	tag_t tItem = NULLTAG;
	tag_t tItemRevTag = NULLTAG;
	tag_t *tBomViews = NULL;
	int iBomViewCount = 0;
	tag_t tWindow = NULLTAG;
	tag_t tRootTask = NULLTAG;
	int iCount = 0;
	tag_t *tAttachments = NULL;
	int *iAttachmentType = 0;
	int iNoOfArguments = 0;
	tag_t tTopBomLine = NULLTAG;
	int iBomLineCount = 0;
	tag_t *tBomChilds = NULL;
	char *cValue = NULL;
	tag_t tClassId = NULLTAG;
	char *cClassName = NULL;
	int iFlag = 0;
	char *cObjectName = NULL;
	tag_t tErrorChild = NULLTAG;
	int iFail = 0;
	check(EPM_ask_root_task(msg.task, &tRootTask));//Retrieves root task of the given task or job.
	if (tRootTask != NULLTAG)
	{
		check(EPM_ask_all_attachments(tRootTask, &iCount, &tAttachments, &iAttachmentType));//Retrieves all the objects attached to given task for given attachment type
		if (iCount != 0 && tAttachments != NULL && iAttachmentType != 0)
		{
			for (int i = 0; i < iCount; i++)
			{
				check(POM_class_of_instance(tAttachments[i], &tClassId));//Finds out the class Id which "tAttachments[i]" belongs to
				if (tClassId != NULLTAG)
				{
					check(POM_name_of_class(tClassId, &cClassName));//finds the name of the class

					if (cClassName != NULL)
					{
						//printf("class name=%s\n", cClassName);
						if (tc_strcmp(cClassName, OBJECT_TYPE) == ITK_ok)
						{
							printf("Item revision found\n");
							check(ITEM_ask_item_of_rev(tAttachments[i], &tItem));
							if (tItem != NULLTAG)
							{
								check(ITEM_list_bom_views(tItem, &iBomViewCount, &tBomViews));
								if (iBomViewCount != 0 && tBomViews != NULL)
								{
									for (int j = 0; j < iBomViewCount; j++)
									{
										check(BOM_create_window(&tWindow));
										if (tWindow != NULLTAG)
										{
											check(BOM_save_window(tWindow));
											check(BOM_set_window_top_line(tWindow, tItem, tAttachments[i], tBomViews[j], &tTopBomLine));
											if (tTopBomLine != NULLTAG)
											{
												check(BOM_line_ask_all_child_lines(tTopBomLine, &iBomLineCount, &tBomChilds));
												if (iBomLineCount != 0 && tBomChilds != NULL)
												{
													for (int k = 0; k < iBomLineCount; k++)
													{
														check(AOM_ask_value_string(tBomChilds[k], "bl_all_notes", &cValue));

														if (tc_strlen(cValue)!=0)
														{
															iFlag=1;
														}
														else
														{
															tErrorChild = tBomChilds[k];
															iFlag =0;
															break;
														}
													}
													MEM_free(cValue);
													if (iFlag == 1)
													{
														decision=EPM_go;
													}
													else
													{
														//check(WSOM_ask_name2(tErrorChild, &cObjectName));//Retrieves the object ID of the specified WorkspaceObject.
														//if (cObjectName != NULL)
														//{
															//check(EMH_store_error_s1(EMH_severity_error, PLM_Error, cObjectName));
														iFail=AOM_ask_value_string(tErrorChild, "bl_indented_title", &cObjectName);
														if (iFail==0&&cObjectName != NULL)
														{
															EMH_store_error_s1(EMH_severity_error, PLM_Error, cObjectName);
															decision = EPM_nogo;
														}
													
														//}
													}
													
													MEM_free(cObjectName);
												}
												MEM_free(tBomChilds);
											}
											
										}
									}
								}
								MEM_free(tBomViews);
							}
							
						}
					}
					MEM_free(cClassName);
				}
			}
			
		}
		MEM_free(tAttachments);
		MEM_free(iAttachmentType);
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