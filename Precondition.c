#include<stdio.h>
#include<conio.h>
#include<tccore/custom.h>
#include<tccore/method.h>
#include<tccore/aom_prop.h>
#include<fclasses/tc_string.h>
#include<tc/emh.h>
#define DLLAPI _declspec(dllexport)
#define PLM_error (EMH_USER_error_base +2)

METHOD_id_t tMethodId;
int iStatus = 0;
extern DLLAPI int DLL_PROJECT_register_callbacks();
extern DLLAPI int PLM_execute_callbacks1(int *decision, va_list argv);
extern DLLAPI int PLM_add_precondition(METHOD_message_t *msg, va_list argv);
extern DLLAPI int PLM_execute_callbacks2(int *decision, va_list argv);

extern DLLAPI int DLL_PROJECT_register_callbacks()
{
	iStatus = CUSTOM_register_exit("DLL_PROJECT", "USER_init_module", (CUSTOM_EXIT_ftn_t)PLM_execute_callbacks1);
	// This function registers a custom exit (a custom function pointer) for a given USER_ and USERSERVICE_ exit function
	iStatus = CUSTOM_register_exit("DLL_PROJECT", "USER_exit_module", (CUSTOM_EXIT_ftn_t)PLM_execute_callbacks2);
	return iStatus;
}


extern DLLAPI int PLM_execute_callbacks1(int *decision, va_list argv)
{
	*decision = ALL_CUSTOMIZATIONS;
	printf("\n\n\n----welcome dll registration process---- \n\n\n");
	printf("\n\n\n----login sucess---- \n\n\n");

	iStatus = METHOD_find_method("ItemRevision", "ITEM_copy_rev", &tMethodId);
	//This api finds the method used to implement the specified message combination.
	//This is useful for registering pre - or post - actions or a pre - condition for a method created elsewhere in the system.
	//ITEM_copy_rev=Identifies the message for which the method is registered
	iStatus = METHOD_add_pre_condition(tMethodId, (METHOD_function_t)PLM_add_precondition, NULL);
	//Registers the given function as a pre - condition handler for this method.
	//Per -condition means it will check before performing the task.
	
	return iStatus;
}
extern DLLAPI int PLM_add_precondition(METHOD_message_t *msg, va_list argv)
{
	tag_t tSourceRev = NULLTAG;
	char *cValue = NULL;
	char *cName = NULL;
	//int iNum = 0;
	tSourceRev = va_arg(argv, tag_t);
	AOM_UIF_ask_value(tSourceRev, "release_status_list", &cValue);//if the output value type is not known then we use this API.
	AOM_UIF_ask_value(tSourceRev, "object_name", &cName);
	
		if (tc_strcmp(cValue, "TCM Released") == 0)
		{
			printf("\n\n TCM Released \n\n");
		}
		else
		{
			EMH_store_error_s1(EMH_severity_error, PLM_error, cName);
			//EMH_severity_error in tc/tc_startup
			return PLM_error;
		}
		MEM_free(cName);
		MEM_free(cValue);
	return iStatus;
}

extern DLLAPI int PLM_execute_callbacks2(int *decision, va_list argv)
{
	*decision = ALL_CUSTOMIZATIONS;
	printf("\n\n\n----logout sucess---- \n\n\n");
	return iStatus;
}