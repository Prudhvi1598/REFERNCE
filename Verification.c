//--Task--It should check only the specified type item revisions should be added to topline in structure manager
/*
register call backs init and exit

find method
add pre condition method

ask object type of parent tag
check if item 
	ask object type
if rev
	find item tag and find object type

compare parent type and child object type
	if same return EPM_go
	else return EPM_nogo


*/
#include<stdio.h>
#include<conio.h>
#include<tccore/custom.h>
#include<tccore/method.h>
#include<tccore/aom_prop.h>
#include<fclasses/tc_string.h>
#include<tc/emh.h>
#include<bom/bom_msg.h>
#include<bom/bom.h>
#include<tccore/workspaceobject.h>
#include<tccore/item.h>
#define OBJECT_TYPE "Item"
#define DLLAPI _declspec(dllexport)
#define PLM_error (EMH_USER_error_base +10)

METHOD_id_t tMethodId;
int iStatus = 0;
extern DLLAPI int DLL_PROJECT_register_callbacks();
extern DLLAPI int PLM_execute_callbacks1(int *decision, va_list argv);
extern DLLAPI int PLM_add_precondition(METHOD_message_t *msg, va_list argv);
extern DLLAPI int PLM_execute_callbacks2(int *decision, va_list argv);
int check(int exp);
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

	iStatus = METHOD_find_method("BOMLine", "BOMLine_add", &tMethodId);//bom_msg.h
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
	tag_t tParent = NULLTAG;
	tag_t tItem = NULLTAG;
	tag_t tItemRev = NULLTAG;

	tag_t tItemTag = NULLTAG;
	tag_t tParentTag = NULLTAG;

	tParent= va_arg(argv, tag_t);
	tItem = va_arg(argv, tag_t);
	tItemRev = va_arg(argv, tag_t);

	char *cPvalue = NULL;
	char *cCvalue = NULL;
	
	if (tParent != NULLTAG)
	{
		check(AOM_UIF_ask_value(tParent, "bl_item_object_type", &cPvalue));//Rettrieving the object type
		if (cPvalue != NULL)
		{
			if (tItem != NULLTAG && tItemRev == NULLTAG)//If we click on Item while sending to SM then this condition works
			{
				check(AOM_ask_value_string(tItem, "object_type", &cCvalue));// Rettrieving the object type
			}
			else if (tItem == NULLTAG && tItemRev != NULLTAG)//If we click on Item revision while sending to structure manager
			{
				check(ITEM_ask_item_of_rev(tItemRev, &tItemTag));//Finding Item tag using Item revision
				if (tItemTag != NULLTAG)
				{
					check(AOM_ask_value_string(tItemTag, "object_type", &cCvalue));//Rettrieving the object type
				}

			}
		}
		
	}
	if (tc_strcmp(cPvalue, cCvalue) == 0)//Comparng parent object type and child object type
	{
		printf("value is of type Item\n");
	}
	else
	{
		EMH_store_error_s1(EMH_severity_information, PLM_error, cCvalue);
		//EMH_severity_error in tc/tc_startup
		return PLM_error;
	}
	if(cPvalue) 
		MEM_free(cPvalue);
	if (cCvalue)
	MEM_free(cCvalue);
	return iStatus;
}

extern DLLAPI int PLM_execute_callbacks2(int *decision, va_list argv)
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
	}
	return 0;
}