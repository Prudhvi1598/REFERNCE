/* Template to regester a DLL */
/* To attach doc revision on item while creation  */

#include<user_exits/user_exits.h>
#include<tc/tc_errors.h>
#include<tccore/method.h>
#include<tccore/item.h>
#include<tccore/aom.h>
#include<ae/dataset.h>
#include<tccore/custom.h>
#include<ae/datasettype.h>
#include<tccore/aom_prop.h>
#include<fclasses/tc_string.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<tccore/grm.h>
#include<Header.h>

#define DLLAPI _declspec(dllexport)
#define PLM_error (EMH_USER_error_base +3)

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
	iFail = METHOD_find_method("Item", "ITEM_create", &tmethod_id);
	iFail = METHOD_add_action(tmethod_id, METHOD_post_action_type, (METHOD_function_t)PLM_add_post_action, NULL);
	return iFail;
}
//post action
extern DLLAPI int PLM_add_post_action(METHOD_message_t *msg, va_list args)
{

	const char *item_id = NULL;
	item_id = va_arg(args, char *);

	const char *item_name = NULL;
	item_name = va_arg(args, char *);

	const char *type_name = NULL;
	type_name = va_arg(args, char *);

	const char *rev_id = NULL;
	rev_id = va_arg(args, char *);

	tag_t *new_item = NULL;
	new_item = va_arg(args, tag_t *);
	tag_t item_tag = *new_item;

	tag_t *new_rev = NULL;
	new_rev = va_arg(args, tag_t *);

	tag_t rel_type = NULLTAG;
	tag_t datasettype = NULLTAG;
	tag_t new_dataset= NULLTAG;
	tag_t relation= NULLTAG;

	
	GRM_find_relation_type("IMAN_reference", &rel_type);
	AE_find_datasettype2("PDF", &datasettype);
	AE_create_dataset_with_id(datasettype, "DLL", NULL, NULL, NULL, &new_dataset);
	AOM_save_without_extensions(new_dataset);
	GRM_create_relation(item_tag, new_dataset, rel_type, NULLTAG, &relation);
	GRM_save_relation(relation);

	return ITK_ok;
}

extern DLLAPI int PLM_execute_callbacks2(int *decision, va_list argv)
{
	*decision = ALL_CUSTOMIZATIONS;
	printf("\n\n\n*****DLL logout success*****\n\n\n");
	return iFail;
}

