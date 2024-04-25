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
#define DLLAPI _declspec(dllexport)
#define PLM_Error (EMH_USER_error_base +7)


int iStatus = ITK_ok;
extern DLLAPI int DLL_PROJECT_register_callbacks();
extern DLLAPI int handler_execute_callbacks1(int *decision, va_list argv);
extern DLLAPI EPM_decision_t check_rule(EPM_rule_message_t msg);
extern DLLAPI int handler_execute_callbacks2(int *decision, va_list argv);
int check(exp);

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

	EPM_register_rule_handler("Rule_Handler_with_Multiple_Argument", "Rule_Handler_with_Multiple_Argument", (EPM_rule_handler_t)check_rule);
	//Registers an action handler with the given handlerName.
	//1.Handler name
	//2.handler description
	//3.function pointer
	return iStatus;
}

extern DLLAPI EPM_decision_t check_rule(EPM_rule_message_t msg)
{
	EPM_decision_t decision = EPM_nogo;
	tag_t tRootTask = NULLTAG;
	tag_t *tAttachments = NULL;
	int iCount = 0;
	int *iAttachmentType = 0;
	int iNoOfArguments = 0;
	char *cArgumentName = NULL;
	char *cArgumentValue = NULL;
	char *cDatsetType = NULL;
	char *cRelation = NULL;
	tag_t tClassId = NULLTAG;
	char *cClassName = NULL;
	char *cTypeName = NULL;
	int iSecCount = 0;
	GRM_relation_t *tSecondaryList = NULL;
	char *cObjectName = NULL;
	char *cObjectType = NULL;
	check(EPM_ask_root_task(msg.task, &tRootTask));//Retrieves root task of the given task or job.
	if (tRootTask != NULLTAG)
	{
		check(EPM_ask_all_attachments(tRootTask, &iCount, &tAttachments, &iAttachmentType));//Retrieves all the objects attached to given task for given attachment type
		if (iCount != 0 && tAttachments != NULL && iAttachmentType != 0)
		{
			iNoOfArguments = TC_number_of_arguments(msg.arguments);
			if (iNoOfArguments != 0)
			{
				for (int i = 0; i < iNoOfArguments; i++)
				{
					check(ITK_ask_argument_named_value(TC_next_argument(msg.arguments), &cArgumentName, &cArgumentValue));
					// Returns the current argument from the list of Teamcenter arguments
					if (cArgumentName != NULL && cArgumentValue != NULL)
					{
						if (tc_strcmp(cArgumentName, "dataset_type") == 0)
						{
							cDatsetType = (char*)MEM_alloc(100);
							tc_strcpy(cDatsetType, "");
							tc_strcpy(cDatsetType, cArgumentValue);
						}
						if (tc_strcmp(cArgumentName, "relation") == 0)
						{
							cRelation = (char*)MEM_alloc(50);
							tc_strcpy(cRelation, "");
							tc_strcpy(cRelation, cArgumentValue);
						}
					}
				}
				for (int j = 0; j < iCount; j++)
				{
					check(POM_class_of_instance(tAttachments[j], &tClassId));//Finds out the class which "tAttachments[i]" belongs to
					if (tClassId != NULLTAG)
					{
						check(POM_name_of_class(tClassId, &cClassName));//finds the name of the class

						if (cClassName != NULL)
						{
							//printf("class name=%s\n", cClassName);
							if (tc_strcmp(cClassName, "ItemRevision") == ITK_ok)
							{
								printf("Item revision found\n");
								check(GRM_list_secondary_objects(tAttachments[j], NULLTAG, &iSecCount, &tSecondaryList));//lists all secondary objects
								if (iSecCount != 0 && tSecondaryList != NULL)
								{
									for (int k = 0; k < iSecCount; k++)
									{
										check(WSOM_ask_object_type2(tSecondaryList[k].secondary, &cObjectType));// Retrieves the object type of the specified WorkspaceObject.

										if (cObjectType != NULL)
										{
											check(AOM_ask_value_string(tSecondaryList[k].relation_type, "type_name", &cTypeName));
											if (cTypeName != NULL) 
											{
												if ((tc_strcmp(cObjectType, cDatsetType) == 0) && (tc_strcmp(cTypeName, cRelation) == 0))
												{
													return EPM_go;
												}
												else
												{
													check(WSOM_ask_object_id_string(tSecondaryList[k].primary, &cObjectName));//Retrieves the object ID of the specified WorkspaceObject.
													if (cObjectName != NULL)
													{
														check(EMH_store_error_s1(EMH_severity_error, PLM_Error, cObjectName));
														return EPM_nogo;
													}
												}
											}
										}
										
									}
								
								}
								MEM_free(cObjectName);
								MEM_free(cTypeName);
								MEM_free(cObjectType);
							}
							MEM_free(tSecondaryList);
						}
					}
					MEM_free(cClassName);
				}
			}
			MEM_free(cArgumentName);
			MEM_free(cArgumentValue);
		}
	}
	MEM_free(tAttachments);
	MEM_free(iAttachmentType);
	return decision;
}

extern DLLAPI int handler_execute_callbacks2(int *decision, va_list argv)
{
	*decision = ALL_CUSTOMIZATIONS;
	printf("\n\n\n----logout sucess---- \n\n\n");
	return iStatus;
}

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