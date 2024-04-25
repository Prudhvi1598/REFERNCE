//---Action handler with aruguments to attach custom status
/*
This program will set a custom release status that will be given in the arguments

	Ask root task
	Ask attachments
		Find no of arguments in the 
		Based on the argument count ask name and value
		Compare the argument name & original value
		Ask class of instance
		Ask name of class
		Create release status 
		Add release status
*/

#include<stdio.h>
#include<conio.h>
#include<tccore/custom.h>
#include<tccore/method.h>
#include<fclasses/tc_string.h>
#include<pom/pom/pom.h>
#include<epm/epm.h>
#include<tc/emh.h>
#define DLLAPI _declspec(dllexport)
#define plm_error (EMH_USER_error_base +4)
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

	EPM_register_action_handler("Custom_Acction_Handler_With_Arguments", "Custom Handler With Arguments", (EPM_action_handler_t)add_custom_status);
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
	int i,j;
	int Count1 = 0;
	int iNoOfArguments = 0;
	char *cArgumentName = NULL;
	char *cArgumentValue = NULL;
	char *cStatus = NULL;
	logical RetainReleasedDate = true;//it is the indiacator to keep the released date or not.

	iStatus = EPM_ask_root_task(msg.task, &tRootTask);//Retrieves root task of the given task or job.
	if (iStatus == ITK_ok && tRootTask != NULLTAG)
	{
		iStatus = EPM_ask_attachments(tRootTask, EPM_target_attachment, &iCount, &tAttachments);//Retrieves all the objects attached to given task for given attachment type
		if (iStatus == ITK_ok && tAttachments != NULL)
		{
			iNoOfArguments = TC_number_of_arguments(msg.arguments);//returns total no of arguments attached to a handler
			for (i = 0; i < iNoOfArguments; i++)
			{
				ITK_ask_argument_named_value(TC_next_argument(msg.arguments), &cArgumentName, &cArgumentValue);
				// Returns the current argument from the list of Teamcenter arguments
				if (tc_strcmp(cArgumentName, "Apply_status") == 0)
				{
					cStatus = (char*)MEM_alloc(100);
					tc_strcpy(cStatus, "");
					tc_strcpy(cStatus, cArgumentValue);
				}
				for (j = 0; j < iCount; j++)
				{
					POM_class_of_instance(tAttachments[i], &tClassId);//Finds out the class which "tAttachments[i]" belongs to
					if (tClassId != NULLTAG)
					{
						POM_name_of_class(tClassId, &cClassName);//finds the name of the class

						if (cClassName != NULL)
						{
							//printf("class name=%s\n", cClassName);
							if (tc_strcmp(cClassName, "A4Object2Revision") == ITK_ok)
							{
								if (RELSTAT_create_release_status(cStatus, &tReleaseStatus) == ITK_ok)//Creates release status object for given input status type.
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

											EMH_ask_error_text(iStatus, &cError);
											printf("error is %s\n", cError);
										}
									}
								}
							}
							else
							{
								return plm_error;
							}
						}
					}

				}
				if(cClassName)
					MEM_free(cClassName);
			}
			if (cArgumentName)
				MEM_free(cArgumentName);
			if (cArgumentValue)
				MEM_free(cArgumentValue);
		}

		if (tAttachments)
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