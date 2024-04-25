
/* While creating project, its name<10 and desc should be filled  */

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
#define EMH_error (EMH_USER_error_base +14)

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
	iFail = METHOD_find_method("TC_Project", "PROJECT_create", &tmethod_id);
	iFail = METHOD_add_action(tmethod_id, METHOD_pre_action_type, (METHOD_function_t)PLM_add_pre_action, NULL);
	return iFail;
}
//post action
extern DLLAPI int PLM_add_pre_action(METHOD_message_t *msg, va_list args)
{
	char *project_id = va_arg(args, char *);
	char *name = va_arg(args, char *);
	char *desc = va_arg(args, char *);
	tag_t *new_project = va_arg(args, tag_t*);

	int length = tc_strlen(name);
	tag_t class_id1 = NULLTAG;
	char *class_name1 = NULL;
	tag_t class_id2 = NULLTAG;
	char *class_name2 = NULL;
	
	if (length < 10 && tc_strcmp(desc, " ") != 0);
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

