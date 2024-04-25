/* Template to regester a DLL */
/* While deleting dataset from the IR, the dataset count should be re assigned
	  pattern:- item_id+ item_revision_id+ missing count of Dataset */

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
	iFail = METHOD_find_method("IMAN_specification", "IMAN_delete", &tmethod_id);
	iFail = METHOD_add_action(tmethod_id, METHOD_post_action_type, (METHOD_function_t)PLM_add_post_action, NULL);
	return iFail;
}
//post action
extern DLLAPI int PLM_add_post_action(METHOD_message_t *msg, va_list args)
{
	tag_t source_rev = va_arg(args, tag_t);

	GRM_relation_t*  secondary_list = NULL;
	//tag_t secondary_object = NULLTAG;
	//tag_t relation_type = NULLTAG;
	tag_t class_id = NULLTAG;
	tag_t *secondary_obj = NULL;//OF
	char *item_id = NULL;
	char *item_revision_id = NULL;
	tag_t primary_object = NULLTAG;
	char* object_type1 = NULL;
	int n_count = 0;
	int n_datasets = 0;
	int append = 1;
	char *class_name = NULL;//OF
	char *str_con=NULL;

	
	WSOM_ask_name2(source_rev, &object_type1);
	printf("%s\n", object_type1);
	GRM_ask_primary(source_rev, &primary_object);
	GRM_list_secondary_objects(primary_object,NULLTAG, &n_count, &secondary_list);
	for (int j = 0; j < n_count; j++)
	{
		POM_class_of_instance(secondary_list[j].secondary, &class_id);
		POM_name_of_class(class_id, &class_name);
		if (strcmp(class_name, "Dataset") == 0)
		{
			AOM_ask_value_string(secondary_list[j].secondary, "item_id", &item_id);
			AOM_ask_value_string(secondary_list[j].secondary, "item_revision_id", &item_revision_id);
			sprintf(str_con, "%d",append++); // To convert int to string
			char *new_name = (char *)malloc(100 * sizeof(char));
			tc_strcpy(new_name, item_id);
			tc_strcat(new_name, "-");
			tc_strcat(new_name, item_revision_id);
			tc_strcat(new_name, "-");
			tc_strcat(new_name, str_con);
			AOM_set_value_string(secondary_list[j].secondary, "object_name", new_name);
			MEM_free(new_name);
			MEM_free(str_con);
		}	
	}
	MEM_free(secondary_obj);
	MEM_free(item_id);
	MEM_free(item_revision_id);
	MEM_free(secondary_list);
	return iFail;
}

extern DLLAPI int PLM_execute_callbacks2(int *decision, va_list argv)
{
	*decision = ALL_CUSTOMIZATIONS;
	printf("\n\n\n*****DLL logout success*****\n\n\n");
	return iFail;
}

