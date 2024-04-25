/*
find template
Find root task
attach to target
*/
#include<stdio.h>
#include<conio.h>
#include<tccore/custom.h>
#include<tccore/method.h>
#include<fclasses/tc_string.h>
#include<pom/pom/pom.h>
#include<epm/epm.h>
#define DLLAPI _declspec(dllexport)
#define plm_error (EMH_USER_error_base +3)

METHOD_id_t tMethodId;
int iStatus = ITK_ok;
extern DLLAPI int DLL_PROJECT_register_callbacks();
extern DLLAPI int handler_execute_callbacks1(int *decision, va_list argv);
extern DLLAPI int add_custom_status(EPM_action_message_t msg);
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

	EPM_register_action_handler("Custom_Handler_Create_Status", "Creating custom status", (EPM_action_handler_t)add_custom_status);
	//Registers an action handler with the given handlerName.
	//1.Handler name
	//2.handler description
	//3.function pointer
	return iStatus;
}
extern DLLAPI int add_custom_status(EPM_action_message_t msg)
{
	tag_t tRootTask = NULLTAG;
	tag_t *tAttachments = NULL;
	tag_t tClassId = NULLTAG;
	tag_t tReleaseStatus = NULLTAG;

	char *cClassName = NULL;
	char *cError = NULL;

	int iCount = 0;
	int i;
	int Count1 = 0;
	logical RetainReleasedDate = true;//it is the indiacator to keep the released date or not.

	iStatus = EPM_ask_root_task(msg.task, &tRootTask);//Retrieves root task of the given task or job.
	if (iStatus == ITK_ok && tRootTask != NULLTAG)
	{
		iStatus = EPM_ask_attachments(tRootTask, EPM_target_attachment, &iCount, &tAttachments);//Retrieves all the objects attached to given task for given attachment type
		if (iStatus == ITK_ok && tAttachments != NULL)
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
							if (RELSTAT_create_release_status("CustomStatus", &tReleaseStatus) == ITK_ok)//Creates release status object for given input status type.
							{
								if (tReleaseStatus != NULLTAG)
								{
									if (RELSTAT_add_release_status(tReleaseStatus, 1, &tAttachments[i], RetainReleasedDate) == ITK_ok)
									//Adds the given release status to each of the ws_objects, with an indicator to retain the released date or not.
									{
										printf("The custom status added\n");
									}
									else
									{
										EMH_store_error_s1(EMH_severity_error, plm_error, cClassName);
										EMH_ask_error_text(iStatus, &cError);
										printf("error is %s\n", cError);
										return plm_error;
									}
								}
							}
						}
						else
						{
							printf("object is not a type of 'A4Object2Revision'\n");
						}
					}
				}
			}
		}
		MEM_free(tAttachments);
	}
	return iStatus;
}


extern DLLAPI int handler_execute_callbacks2(int *decision, va_list argv)
{
	*decision = ALL_CUSTOMIZATIONS;
	printf("\n\n\n----logout sucess---- \n\n\n");
	return iStatus;
}