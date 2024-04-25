
/* After object assigned to project, object desc and custom status should get updated */

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

#define DLLAPI _declspec(dllexport)
#define PLM_error (EMH_USER_error_base +2)

METHOD_id_t tmethod_id;
int iFail = ITK_ok;


//Declaration of function
extern DLLAPI int DLL_register_callbacks();
extern DLLAPI int PLM_execute_callbacks1(int *decision, va_list argv);
extern DLLAPI int PLM_execute_callbacks2(int *decision, va_list argv);
extern DLLAPI int PLM_add_post_action(METHOD_message_t *msg, va_list args);

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
	iFail = METHOD_find_method("ItemRevision", "TC_assign_primary_obj_to_project", &tmethod_id);
	iFail = METHOD_add_action(tmethod_id, METHOD_post_action_type, (METHOD_function_t)PLM_add_post_action, NULL);
	return iFail;
}
//post action
extern DLLAPI int PLM_add_post_action(METHOD_message_t *msg, va_list args)
{

	tag_t item_tag= va_arg(args, tag_t);
	tag_t proj_tag= va_arg(args, tag_t);

	char *name = NULL;
	tag_t rule = NULLTAG;
	tag_t release_status = NULLTAG;
	tag_t ws_objects[] = { item_tag };
	WSOM_ask_name2(proj_tag, &name);
	AOM_refresh(item_tag, true);
	AOM_set_value_string(item_tag, "object_desc", name);
	AOM_save_without_extensions(item_tag);
	AOM_refresh(item_tag, false);
	RELSTAT_create_release_status("Project assigned", &release_status);
	RELSTAT_add_release_status(release_status, 1, ws_objects, true);
	return iFail;
}

extern DLLAPI int PLM_execute_callbacks2(int *decision, va_list argv)
{
	*decision = ALL_CUSTOMIZATIONS;
	printf("\n\n\n*****DLL logout success*****\n\n\n");
	return iFail;
}

