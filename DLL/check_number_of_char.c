#include<stdio.h>
#include<tccore/custom.h>
#include<tccore/method.h>
#include<user_exits/user_exits.h>
#include<tccore/releasestatus.h>
#include<epm/epm.h>
#include<pom/pom/pom.h>
#include<tc/emh.h>
#include<tccore/aom.h>
#include<fclasses/tc_string.h>
#include<tccore/aom_prop.h>
#include<tc\emh.h>

#define DLLAPI _declspec(dllexport)
#define PLM_error (EMH_USER_error_base +9)

int iFail = ITK_ok;

// definition of Functions

extern DLLAPI int DLL_register_callbacks();
extern DLLAPI int PLM_execute_callbacks1(int *decision, va_list args);
extern DLLAPI int PLM_execute_callbacks2(int *decision, va_list args);
extern DLLAPI int check_number_of_char(EPM_rule_message_t msg);


extern DLLAPI int DLL_register_callbacks()
{
	iFail = CUSTOM_register_exit("DLL", " USER_init_module", (CUSTOM_EXIT_ftn_t)PLM_execute_callbacks1);
	iFail = CUSTOM_register_exit("DLL", " USER_exit_module", (CUSTOM_EXIT_ftn_t)PLM_execute_callbacks2);

	return iFail;
}


extern DLLAPI int PLM_execute_callbacks1(int *decision, va_list args)
{
	*decision = ALL_CUSTOMIZATIONS;

	printf("\n\n********** WELCOME **********");
	printf("\n\n********** LOGIN SUCCESSFUL**********");
	iFail = EPM_register_rule_handler("check_number_of_char", "check_number_of_char", (EPM_rule_handler_t)check_number_of_char);

	return iFail;
}

extern DLLAPI int check_number_of_char(EPM_rule_message_t msg)
{
	tag_t root_task = NULLTAG;
	int count = 0;
	tag_t* attachments = NULL;
	char* argument_name = NULL;
	char* argument_value = NULL;
	char* names = NULL;
	char* values = NULL;
	char* arg_name[] = {"object_name","property"};
	char* arg_values[2];

	iFail = EPM_ask_root_task(msg.task, &root_task);

	iFail = EPM_ask_attachments(root_task, EPM_target_attachment, &count, &attachments);


	int arg_num = TC_number_of_arguments(msg.arguments);

	iFail = ITK_ask_argument_named_value(TC_next_argument(msg.arguments), &argument_name, &argument_value);

	for (int j = 0; j < arg_num; j++)
	{
		if(tc_strcmp(argument_name, arg_name[j])==0)
		{
			arg_values[j] = (char *)MEM_alloc(100);
			tc_strcpy(arg_values[j], argument_value);
	
		}
	}

	for (int i = 0; i < count; i++)
	{
		AOM_ask_value_string(attachments[i], "object_name", &names);
		if (tc_strcmp(names, arg_values[0]) == 0)
		{
			AOM_ask_value_string(attachments[i], arg_values[1], &values);

			if (tc_strlen(values) > 5)
			{
				printf("success\n");

			}
			else
			{
				EMH_store_error(EMH_severity_error, PLM_error);

					return PLM_error;
			}




		}
	}
	return iFail;
}

extern DLLAPI int  PLM_execute_callbacks2(int *decision, va_list args)
{
	*decision = ALL_CUSTOMIZATIONS;

	printf("\n\n****** LOG OUT ******");

	return iFail;

}