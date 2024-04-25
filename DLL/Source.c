

#include <stdio.h>

#include <tccore\item.h>

#include <user_exits\user_exits.h>

#include <tccore\custom.h>

#include <tccore\method.h>

#include <tccore\workspaceobject.h>

#include <tccore\aom_prop.h>

#include <sa\am.h>

#include <tc\folder.h>

#include <sa\user.h>

#include <ps\ps.h>





#define DLLAPI _declspec(dllexport)

#define PLM_error (EMH_USER_error_base +1)

iFail = ITK_ok;

METHOD_id_t method_id;





extern DLLAPI int DLL_register_callbacks();

extern DLLAPI int execute_callbacks1(int* decision, va_list args);

extern DLLAPI int execute_callbacks2(int* decision, va_list args);

extern DLLAPI int post_action(int* decision, va_list args);



extern DLLAPI int DLL_register_callbacks()

{

	iFail = CUSTOM_register_exit("DLL", "USER_init_module", (CUSTOM_EXIT_ftn_t)execute_callbacks1);

	iFail = CUSTOM_register_exit("DLL", "USER_exit_module", (CUSTOM_EXIT_ftn_t)execute_callbacks2);



	return iFail;

}





extern DLLAPI int execute_callbacks1(int* decision, va_list args)

{

	*decision = ALL_CUSTOMIZATIONS;



	printf("***************** WELCOME **************\n\n");

	printf("************** LOGIN SUCCESS **************\n\n");

	printf("******* WELCOME TO AKHIL'S TEAMCENTER ******\n\n");


	METHOD_find_prop_method("Folder", "contents", "PROP_set_value_tags", &method_id);
	METHOD_add_action(method_id, METHOD_post_action_type, (METHOD_function_t)post_action, NULL);
	return iFail;

}

extern DLLAPI int post_action(int* decision, va_list args)

{
	tag_t prop_tag = va_arg(args, tag_t);
	int prop_value_num = va_arg(args, int);
	tag_t*	prop_values = va_arg(args, tag_t*);
	tag_t class_id = NULLTAG;
	char* class_name = NULL;
	int n_references = 0;
	int* levels = NULL;
	tag_t* references = NULL;
	char* fl_name = NULL;
	char** relations = NULL;

	for (int i = 0; i < prop_value_num; i++)
	{
		POM_class_of_instance(prop_values[i], &class_id);
		POM_name_of_class(class_id, &class_name);
		WSOM_where_referenced2(prop_values[i], WSO_where_ref_any_depth, &n_references, &levels, &references, &relations);
		for (int j = 0; j < n_references; j++)
		{
				AOM_ask_value_string(references[j], "object_name", &fl_name);
				if (tc_strcmp(fl_name, "Dataset_folder") == 0)
				{
					if (tc_strcmp(class_name, "Dataset") == 0);
					else
					{
						EMH_store_error(EMH_severity_error, PLM_error);
						return PLM_error;
					}
				}
		}
	}

	MEM_free(class_name);

	return iFail;

}



extern DLLAPI execute_callbacks2(int *decision, va_list args)

{

	*decision = ALL_CUSTOMIZATIONS;



	printf("\n\n\n ******** Log Out********");



	return iFail;

}