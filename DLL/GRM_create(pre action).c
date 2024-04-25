
/* Only Revisions should get attached to IR */

#include<user_exits/user_exits.h>
#include<tc/tc_errors.h>
#include<tccore/method.h>
#include<tccore/item.h>
#include<tccore/custom.h>
#include<tccore/aom.h>
#include<ae/dataset.h>
#include<ae/datasettype.h>
#include<tccore/aom_prop.h>
#include<fclasses/tc_string.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<tccore/grm.h>
#include<pom/pom/pom.h>
#include<tccore/releasestatus.h>
#include<Header.h>

#define DLLAPI _declspec(dllexport)
#define EMH_error (EMH_USER_error_base +13)

METHOD_id_t tmethod_id;
int iFail = ITK_ok;


//Declaration of function
extern DLLAPI int DLL_register_callbacks();
extern DLLAPI int PLM_execute_callbacks1(int *decision, va_list argv);
extern DLLAPI int PLM_execute_callbacks2(int *decision, va_list argv);
extern DLLAPI int PLM_add_pre_action(METHOD_message_t *msg, va_list args);

//Definition of function
extern DLLAPI int DLL_register_callbacks()
{
	iFail = CUSTOM_register_exit("DLL", "USER_init_module", (CUSTOM_EXIT_ftn_t)PLM_execute_callbacks1);
	iFail = CUSTOM_register_exit("DLL", "USER_exit_module", (CUSTOM_EXIT_ftn_t)PLM_execute_callbacks2);
	return iFail;
}

extern DLLAPI int PLM_execute_callbacks1(int *decision, va_list argv)
{
	*decision = ALL_CUSTOMIZATIONS;
	printf("\n\n\n*****Welcome to DLL Registration*****\n\n\n");
	printf("Login success\n");
	iFail = METHOD_find_method("IMAN_specification", "GRM_create", &tmethod_id);
	iFail = METHOD_add_action(tmethod_id, METHOD_pre_action_type, (METHOD_function_t)PLM_add_pre_action, NULL);
	return iFail;
}
//post action
extern DLLAPI int PLM_add_pre_action(METHOD_message_t *msg, va_list args)
{

	tag_t primary_object=va_arg(args, tag_t);
	tag_t secondary_object=va_arg(args, tag_t);
	tag_t relation_type= va_arg(args, tag_t);
	tag_t	user_data= va_arg(args, tag_t);
	tag_t*	new_relation= va_arg(args, tag_t*);
	
	tag_t class_id1 = NULLTAG;
	char *class_name1 = NULL;
	tag_t class_id2 = NULLTAG;
	char *class_name2 = NULL;
	char *name = NULL;
	POM_class_of_instance(primary_object, &class_id1);
	POM_name_of_class(class_id1, &class_name1);

	POM_class_of_instance(primary_object, &class_id2);
	POM_name_of_class(class_id2, &class_name2);

	if (tc_strcmp(class_name1, class_name2) == 0)
	{
		WSOM_ask_name2(relation_type, &name);
		AOM_refresh(secondary_object, true);
		AOM_set_value_string(secondary_object, "object_desc", name);
		AOM_save_without_extensions(secondary_object);
		AOM_refresh(secondary_object, false);
	}
	else
	{
		EMH_store_error_s1(EMH_severity_error, EMH_error, NULL);
		return EMH_error;
	}
	return iFail;
}

extern DLLAPI int PLM_execute_callbacks2(int *decision, va_list argv)
{
	*decision = ALL_CUSTOMIZATIONS;
	printf("\n\n\n*****DLL logout success*****\n\n\n");
	return iFail;
}

