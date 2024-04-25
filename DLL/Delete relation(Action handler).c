/* Template to regester a DLL */
/* To create Action handler for custom Release status with arguments */

#include<Header.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<user_exits/user_exits.h>
#include<tc/tc_errors.h>
#include<tc/tc_arguments.h>
#include<tc/tc_startup.h>
#include<tccore/method.h>
#include<tccore/item.h>
#include<tccore/aom.h>
#include<tccore/custom.h>
#include<pom/pom/pom.h>
#include<tccore/aom_prop.h>
#include<epm/epm.h>
#include<fclasses/tc_string.h>
#include<tccore/grm.h>



#define DLLAPI _declspec(dllexport)
#define PLM_error (EMH_USER_error_base +9)

METHOD_id_t tmethod_id;
int iFail = ITK_ok;


//Declaration of function
extern DLLAPI int DLL_register_callbacks();
extern DLLAPI int Handler_execute_callbacks1(int *decision, va_list argv);
extern DLLAPI int Handler_execute_callbacks2(int *decision, va_list argv);
extern DLLAPI int add_action_handler(EPM_action_message_t msg);

//Definition of function
extern DLLAPI int DLL_register_callbacks()
{
	iFail = CUSTOM_register_exit("DLL", "USER_init_module", (CUSTOM_EXIT_ftn_t)Handler_execute_callbacks1);
	iFail = CUSTOM_register_exit("DLL", "USER_exit_module", (CUSTOM_EXIT_ftn_t)Handler_execute_callbacks2);
	return iFail;
}

extern DLLAPI int Handler_execute_callbacks1(int *decision, va_list argv)
{
	*decision = ALL_CUSTOMIZATIONS;
	printf("\n\n\n*****Welcome to Action Handler DLL Registration*****\n\n\n");
	printf("Login success\n");
	EPM_register_action_handler("GRM-delete", "Custom Action Handler with arguments", (EPM_action_handler_t)add_action_handler);
	return iFail;
}
//Action Handler with arguments
extern DLLAPI int add_action_handler(EPM_action_message_t msg)
{
	int count = 0;
	int count1 = 0;
	int no_of_arguments = 0;
	tag_t root_task = NULLTAG;
	tag_t *attachments = NULL;
	char *argument_name = NULL;
	char *argument_value = NULL;
	char *primary = NULL;
	char *secondary = NULL;
	tag_t *secondary_list = NULL;
	tag_t relation = NULLTAG;
	char *name = NULL;
	char *object_type = NULL;
	tag_t relation_type= NULLTAG;
	char *arguments[] = { "primary","secondary","relation" };
	char *values[3];

	CHECK(EPM_ask_root_task(msg.task, &root_task));
	CHECK(EPM_ask_attachments(root_task, EPM_target_attachment, &count, &attachments));
	no_of_arguments = TC_number_of_arguments(msg.arguments);
	/* To store argument values */
	for (int i = 0; i < no_of_arguments; i++)
	{
		ITK_ask_argument_named_value(TC_next_argument(msg.arguments), &argument_name, &argument_value);
		if (tc_strcmp(argument_name, arguments[i]) == 0)
		{
			values[i] = (char *)MEM_alloc(100);
			tc_strcpy(values[i], argument_value);
		}
	}
	/* to delete GRM Relation */
	for (int i = 0; i < count; i++)
	{
		WSOM_ask_object_type2(attachments[i], &object_type);
		if (tc_strcmp(object_type, values[0]) == 0)
		{
			GRM_find_relation_type(values[2], &relation_type);
			GRM_list_secondary_objects_only(attachments[i], relation_type, &count1, &secondary_list);
			for (int j = 0; j < count1; j++)
			{
				WSOM_ask_object_type2(secondary_list[j], &name);
				if (tc_strcmp(name, values[1]) == 0)
				{
					GRM_find_relation(attachments[i], secondary_list[j], relation_type, &relation);
					GRM_delete_relation(relation);
				}
			}
		}
		else
		{
			EMH_store_error_s1(EMH_severity_error, PLM_error, object_type);
			return PLM_error; //primary object error
		}
		
	}
	return ITK_ok;
}

extern DLLAPI int Handler_execute_callbacks2(int *decision, va_list argv)
{
	*decision = ALL_CUSTOMIZATIONS;
	printf("\n\n\n*****DLL logout success*****\n\n\n");
	return iFail;
}


