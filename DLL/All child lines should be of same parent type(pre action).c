/* pre action to check whether the child line and the parent are of same type or not */

#include<stdio.h>
#include<stdlib.h>
#include<user_exits/user_exits.h>
#include<tccore/custom.h>
#include<tccore/aom_prop.h>
#include<tccore/item.h>


#define DLLAPI _declspec(dllexport)
#define BOMLine_add_msg   "BOMLine_add"
#define PLM_error1 (EMH_USER_error_base +10)

int iFail = 0;
METHOD_id_t method_id;
tag_t child_item = NULLTAG;
char *object_type1 = NULLTAG;
char *object_type2 = NULLTAG;

//Declaration of function
extern DLLAPI int DLL_register_callbacks();
extern DLLAPI int PLM_execute_callbacks1(int *decision, va_list argv);
extern DLLAPI int PLM_execute_callbacks2(int *decision, va_list argv);
extern DLLAPI int add_pre_action(METHOD_message_t *msg, va_list argv);

//Definition of function
extern DLLAPI int DLL_register_callbacks()
{
	iFail=CUSTOM_register_exit("DLL", "USER_init_module", (CUSTOM_EXIT_ftn_t)PLM_execute_callbacks1);
	iFail=CUSTOM_register_exit("DLL", "USER_exit_module", (CUSTOM_EXIT_ftn_t)PLM_execute_callbacks2);
	return iFail;
}

extern DLLAPI int PLM_execute_callbacks1(int *decision, va_list argv)
{
	*decision = ALL_CUSTOMIZATIONS;
	printf("\n\n\n*****Welcome to DLL Registration*****\n\n\n");
	printf("Login success\n");
	iFail = METHOD_find_method("BOMLine", BOMLine_add_msg, &method_id);
	iFail = METHOD_add_action(method_id, METHOD_pre_action_type, (METHOD_function_t)add_pre_action, NULL);
	return iFail;
}

extern DLLAPI int add_pre_action(METHOD_message_t *msg, va_list argv)
{
	tag_t parent = va_arg(argv, tag_t);
	tag_t item = va_arg(argv, tag_t);
	tag_t item_revision = va_arg(argv, tag_t);

	AOM_ask_value_string(parent, "bl_item_object_type", &object_type1);
	if(item!=NULLTAG && item_revision==NULLTAG)
		AOM_ask_value_string(item, "object_type", &object_type1);
	else
	{
		ITEM_ask_item_of_rev(item_revision, &child_item);
		AOM_ask_value_string(child_item, "object_type", &object_type2);
	}
	if(tc_strcmp(object_type1,object_type2)==0){}
	else
	{
		EMH_store_error_s1(EMH_severity_error, PLM_error1, object_type1);
		return PLM_error1;  //child is not same type of parent
	}
	return iFail;
}


extern DLLAPI int PLM_execute_callbacks2(int *decision, va_list argv)
{
	*decision = ALL_CUSTOMIZATIONS;
	printf("\n\n\n*****DLL logout success*****\n\n\n");
	return iFail;
}

