/* Template to regester a DLL */
#include<user_exits/user_exits.h>
#include<tc/tc_errors.h>
#include<tc/preferences.h>
#include<tccore/method.h>
#include<tccore/custom.h>
#include<tccore/aom.h>
#include<tccore/aom_prop.h>
#include<stdio.h>
#include<string.h>

#define DLLAPI _declspec(dllexport)
int iFail = 0;
METHOD_id_t method_id;

//Declaration of function
extern DLLAPI int DLL_register_callbacks();
extern DLLAPI int PLM_execute_callbacks1(int *decision, va_list argv);
extern DLLAPI int PLM_execute_callbacks2(int *decision, va_list argv);
extern DLLAPI int PropertyUpdate(METHOD_message_t* msg, va_list argv);

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
	METHOD_find_method("A2Fruit", "ITEM_create", &method_id);
	if (method_id.id == 0)
	{
		printf("Method not found!");
		return 1;
	}
		
	METHOD_add_action(method_id, METHOD_post_action_type, (METHOD_function_t)PropertyUpdate, NULL);
	return iFail;
}

extern DLLAPI int PropertyUpdate(METHOD_message_t* msg, va_list argv)
{
	const char* item_id = va_arg(argv, const char*);
	const char* item_name = va_arg(argv, const char*);
	const char* type_name = va_arg(argv, const char*);
	const char* rev_id = va_arg(argv, const char*);
	tag_t* new_item = va_arg(argv, tag_t*);
	tag_t* new_rev = va_arg(argv, tag_t*);

	int count = 0;
	char* prop1 = NULL;
	char** values = NULL;

	AOM_ask_value_string(*new_item, "a2Property1", &prop1);
	printf("prop1:%s\n", prop1);

	PREF_ask_char_values("DLL_update_property", &count, &values);
	printf("Preference arguments found:%d\n", count);
	for (int i = 0; i < count; i++)
	{
		char* token = strtok(values[i], ":"); // Tokenize using ':'
		if (token != NULL)
		{
			char* number = token;
			token = strtok(NULL, ":");
			if (token != NULL)
			{
				char* string = token;
				printf("prop2:%s\n", string);
				if (tc_strcmp(number, prop1) == 0)
				{
					AOM_refresh(*new_item, true);
					AOM_set_value_string(*new_item, "a2Property2", string);
					AOM_save_without_extensions(*new_item);
					AOM_refresh(*new_item, false);
					printf("Value updated!\n");
				}
			}
		}
	}

	MEM_free(values);
	return 0;
}


extern DLLAPI int PLM_execute_callbacks2(int *decision, va_list argv)
{
	*decision = ALL_CUSTOMIZATIONS;
	printf("\n\n\n*****DLL logout success*****\n\n\n");
	return iFail;
}
