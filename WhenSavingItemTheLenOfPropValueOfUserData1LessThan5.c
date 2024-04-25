//When Saving Item The Length Of Property Value Of User_data_1 should be less than 5
	/*declaring functions
	registering dlls
	finding method
	adding precondition
		source tag
		GRM_list_secondary_objects
			each sec object check user_data_1 value
			find len of value
			check length<5
			if correct save as works
	*/

#include<stdio.h>
#include<conio.h>
#include<tccore/custom.h>
#include<tccore/method.h>
#include<tccore/aom_prop.h>
#include<fclasses/tc_string.h>
#include<tc/emh.h>
#include<tccore/grm.h>
#include<tccore/aom_prop.h>
#include<tccore/workspaceobject.h>
#define DLLAPI _declspec(dllexport)
#define USER_DATA "user_data_1"
#define OBJECT_NAME "object_name"
#define PLM_error (EMH_USER_error_base +8)

METHOD_id_t tMethodId;
int iStatus = 0;
extern DLLAPI int DLL_PROJECT_register_callbacks();
extern DLLAPI int PLM_execute_callbacks1(int *decision, va_list argv);
extern DLLAPI int PLM_add_precondition(METHOD_message_t *msg, va_list argv);
extern DLLAPI int PLM_execute_callbacks2(int *decision, va_list argv);
int check(exp);

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

	iStatus = METHOD_find_method("Item", "ITEM_create_from_rev", &tMethodId);//<tccore/Item_msg.h>
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
	GRM_relation_t *tSecObjects = NULL;
	int iCount = 0;
	size_t len = 0;
	tag_t tRelType = NULLTAG;
	tSourceRev = va_arg(argv, tag_t);
	if (tSourceRev != NULLTAG)
	{
		GRM_find_relation_type("IMAN_master_form", &tRelType);
		if (tRelType != NULLTAG)
		{
			check(GRM_list_secondary_objects(tSourceRev, tRelType, &iCount, &tSecObjects));//Retrieving all list of[ primary,secondary,rela_type,user_data]
			if (iCount != 0 && tSecObjects != NULL)
			{

				for (int i = 0; i < iCount; i++)
				{
					check(AOM_ask_value_string(tSecObjects[i].secondary, USER_DATA, &cValue));//retrieves the property of a value
					if (cValue != NULL)
					{
						len = tc_strlen(cValue);//finding the length of user_dat_1
						if (len < 5)
						{
							printf("length is less than 5 and item is saved\n");
						}
						else
						{
							check(AOM_ask_value_string(tSourceRev, OBJECT_NAME, &cName));//retrieves the property of a value
							if (cName != NULL)
							{
								check(EMH_store_error_s1(EMH_severity_error, PLM_error, cName));
								return PLM_error;
							}
						}
						MEM_free(cName);
					}
				}
				MEM_free(cValue);
			}
		}
		MEM_free(tSecObjects);
	}
	return iStatus;
}

extern DLLAPI int PLM_execute_callbacks2(int *decision, va_list argv)
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
