/*
----TASK---

Creating the datset with specified dataset name and type and relation
	EPM_ask_root_task
	ITK_ask_argument_named_value
		compare all arguments with original values
	ask all attachments
	find class of instance
	find name of class
	find dataset type
	create datset
	save datset
	find relation type
	grm create relation
	save relation


*/


#include<stdio.h>
#include<conio.h>
#include<tccore/custom.h>
#include<tccore/method.h>
#include<fclasses/tc_string.h>
#include<pom/pom/pom.h>
#include<epm/epm.h>
#include<tc/emh.h>
#include<ae/dataset.h>
#include<ae/datasettype.h>
#include<tccore/aom_prop.h>
#include<tccore/aom.h>
#include<tccore/grm.h>
#define DLLAPI _declspec(dllexport)
#define plm_error (EMH_USER_error_base +4)
METHOD_id_t tMethodId;
int iStatus = ITK_ok;
extern DLLAPI int DLL_PROJECT_register_callbacks();
extern DLLAPI int handler_execute_callbacks1(int *decision, va_list argv);
extern DLLAPI int add_action_with_multiple_arguments(EPM_action_message_t msg);
extern DLLAPI int handler_execute_callbacks2(int *decision, va_list argv);


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
	printf("\n\n\n----Handler dll  registered---- \n\n\n");

	EPM_register_action_handler("Custom_Acction_Handler_With_Mul_Arguments", "Custom Handler With Multiple Arguments", (EPM_action_handler_t)add_action_with_multiple_arguments);
	//Registers an action handler with the given handlerName.
	//1.Handler name
	//2.handler description
	//3.function pointer
	return iStatus;
}

extern DLLAPI int add_action_with_multiple_arguments(EPM_action_message_t msg)
{
	tag_t tRootTask = NULLTAG;
	int iNoOfArguments = 0;
	int i, j;
	int iCount;
	char *cArgumentName = NULL;
	char *cArgumentValue = NULL;
	char *cDatasetName = NULL;
	char *cDatasetType = NULL;
	char *cRelation = NULL;
	tag_t *tAttachments = NULL;
	iStatus = EPM_ask_root_task(msg.task, &tRootTask);
	if (iStatus == ITK_ok&& tRootTask!=NULLTAG)
	{
		iNoOfArguments = TC_number_of_arguments(msg.arguments);
		if (iNoOfArguments > 0)
		{
			for (j = 0; j < iNoOfArguments; j++)
			{
				
				iStatus = ITK_ask_argument_named_value(TC_next_argument(msg.arguments), &cArgumentName, &cArgumentValue);//It will get the each argument in the handler
				if (cArgumentName != NULL && cArgumentValue != NULL)
				{
					if (tc_strcmp(cArgumentName, "dataset_name") == 0)
					{
						cDatasetName = (char*)MEM_alloc(50);
						tc_strcpy(cDatasetName, cArgumentValue);
					}
					if (tc_strcmp(cArgumentName, "dataset_type") == 0)
					{
						cDatasetType = (char*)MEM_alloc(50);
						
						tc_strcpy(cDatasetType, cArgumentValue);
					}
					if (tc_strcmp(cArgumentName, "relation") == 0)
					{
						cRelation = (char*)MEM_alloc(50);
						
						tc_strcpy(cRelation, cArgumentValue);
					}
				}
			}
			tag_t tClassId = NULLTAG;
			char *cClassName = NULL;
			char *cStatus = NULL;
			tag_t tReleaseStatus = NULLTAG;
			tag_t tNewDatset = NULLTAG;
			tag_t tDatasetType = NULLTAG;
			char *cItemId = NULL;
			tag_t tRelType = NULLTAG;
			tag_t tNewRelation = NULLTAG;
			iStatus=EPM_ask_attachments(tRootTask, EPM_target_attachment, &iCount, &tAttachments);//Retrieves root task of the given task or job.
			if (iStatus == ITK_ok && tAttachments != NULLTAG)
			{
				for (i = 0; i < iCount; i++)
				{
					POM_class_of_instance(tAttachments[i], &tClassId);//Finds out the class which "tAttachments[i]" belongs to
					if (tClassId != NULLTAG)
					{
						POM_name_of_class(tClassId, &cClassName);//finds the name of the class

						if (cClassName != NULL)
						{
							printf("class name=%s\n", cClassName);
							if (tc_strcmp(cClassName, "A4Object2Revision") == ITK_ok)
							{
								iStatus=AE_find_datasettype2(cDatasetType, &tDatasetType);//Finding dataset type
								if (iStatus == ITK_ok && tDatasetType != NULLTAG)
								{
									printf("dataset type found\n");
									iStatus=AOM_ask_value_string(tAttachments[i], "item_id", &cItemId);//Fetching the item id
									if (iStatus == ITK_ok && cItemId != NULL)
									{
										printf("item id  found\n");
										iStatus= AE_create_dataset_with_id(tDatasetType, cDatasetName, "datset desc", cItemId,"A", &tNewDatset);//Creates a Dataset by specified type, name, description, id, and rev.
										if (iStatus == ITK_ok && tNewDatset != NULLTAG)
										{
											printf("dataset created\n");
											AOM_save_without_extensions(tNewDatset);//Saving newly created dataset
											printf("datset saved\n");

											iStatus=GRM_find_relation_type(cRelation, &tRelType);//Finding the relation type
											if (iStatus == ITK_ok && tRelType != NULLTAG)
											{
												printf("relation type found\n");
												iStatus = GRM_create_relation(tAttachments[i], tNewDatset, tRelType, NULLTAG, &tNewRelation);//Creating relation with the found relation type
												if (iStatus == ITK_ok && tNewRelation != NULLTAG)
												{
													printf("new relation created\n");
													GRM_save_relation(tNewRelation);//Saving relation
													printf("relation saved\n");
												}
											}
											
										}
									}
									
									

								}
							}
							
						}
						
					}
				}
				
			}
			if (cClassName)
				MEM_free(cClassName);
			if (cItemId)
				MEM_free(cItemId);
			
		}
		
		
	}

	if (tAttachments)
		MEM_free(tAttachments);
	if (cArgumentName)
		MEM_free(cArgumentName);
	if (cArgumentValue)
		MEM_free(cArgumentValue);

	return iStatus;
}
extern DLLAPI int handler_execute_callbacks2(int *decision, va_list argv)
{
	*decision = ALL_CUSTOMIZATIONS;
	printf("\n\n\n----logout sucess---- \n\n\n");
	return iStatus;
}
