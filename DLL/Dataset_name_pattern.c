/* Template to regester a DLL */
/* While adding dataset to the IR, the count of the latest one should be attached with object name 
      pattern:- item_id+ item_revision_id+ latest count of created dataset */

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
#include<Header.h>

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
	iFail = METHOD_find_method("IMAN_specification", "GRM_create", &tmethod_id);
	iFail = METHOD_add_action(tmethod_id, METHOD_post_action_type, (METHOD_function_t)PLM_add_post_action, NULL);
	return iFail;
}
//post action
extern DLLAPI int PLM_add_post_action(METHOD_message_t *msg, va_list args)
{
	tag_t source_rev = NULLTAG;
	tag_t secondary_object = NULLTAG;
	tag_t relation_type = NULLTAG;
	tag_t *secondary_obj = NULL;//OF
	char *item_id = NULL;
	char *item_revision_id = NULL;
	int n_count = 0;
	int n_datasets = 0;
	char *object_type = NULL;//OF
	char *char_value = NULL;//OF
	char str_con[10];

	source_rev = va_arg(args, tag_t);
	secondary_object = va_arg(args, tag_t);
	relation_type = va_arg(args, tag_t);

	GRM_list_secondary_objects_only(source_rev, NULLTAG, &n_count, &secondary_obj);
	for (int j = 0; j < n_count; j++)
	{
		WSOM_ask_object_type2(secondary_obj[j], &object_type);
		if (strcmp(object_type, "PDF") == 0 || strcmp(object_type, "Text") == 0 || strcmp(object_type, "Excel") == 0)
		{
			n_datasets++;
		}
	}
	sprintf(str_con, "%d",n_datasets); // To convert int to string
	AOM_ask_value_string(source_rev, "item_id", &item_id);
	AOM_ask_value_string(source_rev, "item_revision_id", &item_revision_id);
	char *new_name = (char *)malloc(50 * sizeof(char));
	tc_strcpy(new_name, item_id);
	tc_strcat(new_name, "-");
	tc_strcat(new_name, item_revision_id);
	tc_strcat(new_name, "-");
	tc_strcat(new_name, str_con);
	AOM_set_value_string(secondary_object, "object_name", new_name);
	GRM_save_relation(relation_type);


	MEM_free(secondary_obj);
	MEM_free(object_type);
	MEM_free(item_id);
	MEM_free(item_revision_id);
	free(new_name);
	return iFail;
		
}

extern DLLAPI int PLM_execute_callbacks2(int *decision, va_list argv)
{
	*decision = ALL_CUSTOMIZATIONS;
	printf("\n\n\n*****DLL logout success*****\n\n\n");
	return iFail;
}

