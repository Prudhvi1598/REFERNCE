// create a rule Handler to check whether the selected business object is attached with pdf or not

#include<stdio.h>
#include<conio.h>
#include<tccore/custom.h>
#include<tccore/method.h>
#include<fclasses/tc_string.h>
#include<epm/epm.h>
#include<tccore/item.h>
#include<tc/emh.h>
#include<pom/pom/pom.h>
#include<tccore/grm.h>
#define OBJECT_TYPE "ItemRevision"
#define DLLAPI _declspec(dllexport)

#define PLM_Error (EMH_USER_error_base +5)
int iStatus = ITK_ok;

extern DLLAPI int DLL_PROJECT_register_callbacks();//Entry point of execution
extern DLLAPI int handler_execute_callbacks1(int *decision, va_list argv);
extern DLLAPI int check_rule(EPM_rule_message_t msg);
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
	printf("\n\n\n----Rule Handler  registered---- \n\n\n");
	
	check(EPM_register_rule_handler("Check_Pdf_Attached", "Checks whether the pdf is attached or not", (EPM_rule_handler_t)check_rule));
	//Registers an Rule handler with the given handlerName.
	//1.Handler name
	//2.handler description
	//3.function pointer
	return iStatus;
}
extern DLLAPI int check_rule(EPM_rule_message_t msg)
{
	EPM_decision_t tDecision = EPM_nogo;
	int iCount = 0;
	int count1 = 0;
	tag_t *tAttachment = NULL;
	int *iAttachmentType = 0;
	int i;
	tag_t tClassId = NULLTAG;
	char *cClassName = NULL;
	int iSecCount = 0;
	tag_t *tSecObjects = NULL;
	char *cObjectType = NULL;
	char *cObjectString = NULL;
	tag_t tRootTask = NULLTAG;
	//add a root task
	check(EPM_ask_root_task(msg.task, &tRootTask));
	{
		if (tRootTask != NULLTAG)
		{
			check(EPM_ask_all_attachments(tRootTask, &iCount, &tAttachment, &iAttachmentType));
			if (iCount != 0 && tAttachment != NULL && iAttachmentType != 0)
			{
				for (i = 0; i < iCount; i++)
				{

					check(POM_class_of_instance(tAttachment[i], &tClassId));
					if (tClassId != NULLTAG)
					{
						check(POM_name_of_class(tClassId, &cClassName));
						if (cClassName != NULL)
						{
							if (tc_strcmp(cClassName, OBJECT_TYPE) == 0)
							{
								check(GRM_list_secondary_objects_only(tAttachment[i], NULLTAG, &iSecCount, &tSecObjects));
								if (tSecObjects != NULL&& iSecCount!=0)
								{
									for (int j = 0; j < iSecCount; j++)
									{
										check(WSOM_ask_object_type2(tSecObjects[j], &cObjectType));
										{
											if (cObjectType != NULL)
											{
												if (tc_strcmp(cObjectType, "PDF") == 0)
												{
													return EPM_go;
												}

												else
												{
													check(WSOM_ask_object_id_string(tAttachment[i], &cObjectString));
													if (cObjectString) 
													{
														check(EMH_store_error_s1(EMH_severity_error, PLM_Error, cObjectString));
														return EPM_nogo;
													}
													
												}
												MEM_free(cObjectString);
											}

										}
									}
								}
								MEM_free(cObjectType);
							}
							MEM_free(tSecObjects);
						}
					}
					MEM_free(cClassName);
				}
			}
		}
	}
	MEM_free(tAttachment);
	MEM_free(iAttachmentType);
	return tDecision;
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