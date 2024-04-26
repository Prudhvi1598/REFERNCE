

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
#include<tccore/aom_prop.h>

#define DLLAPI _declspec(dllexport)//Exporting the code to DLL

#define PLM_Error (EMH_USER_error_base +3)
int iStatus = ITK_ok;

extern DLLAPI int DLLs_register_callbacks();//Entry point of Library
extern DLLAPI int handler_execute_callbacks1(int *decision, va_list argv);
extern DLLAPI int add_action_handler(EPM_action_message_t msg);//EPM_rule_message_t typedef struct
extern DLLAPI int handler_execute_callbacks2(int *decision, va_list argv);

extern DLLAPI int DLLs_register_callbacks()
{
	iStatus = CUSTOM_register_exit("DLLs", "USER_init_module", (CUSTOM_EXIT_ftn_t)handler_execute_callbacks1);
	iStatus = CUSTOM_register_exit("DLLs", "USER_exit_module", (CUSTOM_EXIT_ftn_t)handler_execute_callbacks2);
	// This functions executes the custom callbacks registered for a particular USER_ or USERSERVICE_ exit
	return iStatus;
}


extern DLLAPI int handler_execute_callbacks1(int *decision, va_list argv)
{
	*decision = ALL_CUSTOMIZATIONS;
	printf("\n\n\n----welcome dll registration process---- \n\n\n");
	printf("\n\n\n----login sucess---- \n\n\n");
	printf("\n\n\n----Rule Handler  registered---- \n\n\n");

	EPM_register_action_handler("DLL-rule-handler", "", (EPM_action_handler_t)add_action_handler);
	//Registers an Rule handler with the given handlerName.
	return iStatus;
}
extern DLLAPI int add_action_handler(EPM_action_message_t msg)
{
	//EPM_decision_t decision = EPM_nogo;
	tag_t tRootTask = NULLTAG;
	EPM_ask_root_task(msg.task, &tRootTask);
	//EPM_decision_t decision = EPM_nogo;
	tag_t rootTask = msg.task;
	char* taskResult = NULL;
	char* Comments = NULL;
	char* taskName = NULL;
	TC_write_syslog("entered");

	AOM_ask_value_string(msg.task, "object_name", &taskName);
	printf("the task name is %s\n", taskName);
	AOM_ask_value_string(msg.task, "task_result", &taskResult);
	printf("task_result is %s\n", taskResult);
	AOM_ask_value_string(msg.task, "comments", &Comments);
	printf("comments is %s\n", Comments);
	if ((tc_strcmp(taskResult, "Approve")!=0) && (tc_strlen(Comments) == 0))
	{
		
			EMH_store_error_s1(EMH_severity_error, PLM_Error, taskResult);
			return PLM_Error;
			//decision = EPM_nogo;
	}
	/*else {
			decision = EPM_go;
		}*/
	
	MEM_free(taskName);
	MEM_free(Comments);


	return iStatus;
}
extern DLLAPI int handler_execute_callbacks2(int *decision, va_list argv)
{
	*decision = ALL_CUSTOMIZATIONS;
	printf("\n\n\n----logout sucess---- \n\n\n");
	return iStatus;
}