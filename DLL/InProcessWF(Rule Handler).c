
/* To check if previous revision has pending process before intiating WF on Target attachment. */

#include<user_exits/user_exits.h>
#include<epm/epm.h>
#include<tccore/method.h>
#include<tccore/item.h>
#include<tccore/custom.h>
#include<tccore/aom.h>
#include<tccore/aom_prop.h>
#include<fclasses/tc_string.h>
#include<tccore/grm.h>
#include<Header.h>

#define DLLAPI _declspec(dllexport)
#define EMH_error (EMH_USER_error_base +15)

METHOD_id_t tmethod_id;
int iFail = ITK_ok;


//Declaration of function
extern DLLAPI int DLL_register_callbacks();
extern DLLAPI int PLM_execute_callbacks1(int *decision, va_list argv);
extern DLLAPI int PLM_execute_callbacks2(int *decision, va_list argv);
extern DLLAPI int Rule_Handler(EPM_rule_message_t);

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
	EPM_register_rule_handler("Check_inprocess_WF", NULL, (EPM_rule_handler_t)Rule_Handler);
	return iFail;
}
//Rule handler
extern DLLAPI int Rule_Handler(EPM_rule_message_t msg)
{
	int count = 0;
	int num = 0;
	int item_count = 0;
	tag_t root_task = NULLTAG;
	tag_t item = NULLTAG;
	tag_t class_id = NULLTAG;
	tag_t *attachments = NULLTAG;
	tag_t *rev_list = NULLTAG;
	char *class_name = NULL;
	char *value = NULL;
	char **values = NULL;
	char *value1 = NULL;
	char *error = NULL;


	EPM_ask_root_task(msg.task, &root_task);
	EPM_ask_attachments(root_task, EPM_target_attachment, &count, &attachments);
	for (int i = 0; i < count; i++)
	{
		POM_class_of_instance(attachments[i], &class_id);//To check Target type
		POM_name_of_class(class_id, &class_name);
		if (tc_strcmp(class_name, "ItemRevision") == 0)
		{
			AOM_ask_value_string(attachments[i], "item_revision_id", &value);
			ITEM_ask_item_of_rev(attachments[i], &item);
			ITEM_list_all_revs(item, &item_count, &rev_list);
			for (int j = 0; j < item_count; j++)  //To find Target object previous revision
			{
				AOM_ask_value_string(rev_list[j],"item_revision_id", &value1);
				if (tc_strcmp(value, value1) == 0)
				{
					AOM_ask_displayable_values(rev_list[j - 1], "process_stage_list", &num, &values);
					if (num != 0)
					{
						AOM_ask_value_string(rev_list[j], "object_string", &error);
						EMH_store_error_s1(EMH_severity_error, EMH_error, error);
						return EMH_error;
					}
				}
			}
		}
	}
	return iFail;
}

extern DLLAPI int PLM_execute_callbacks2(int *decision, va_list argv)
{
	*decision = ALL_CUSTOMIZATIONS;
	printf("\n\n\n*****DLL logout success*****\n\n\n");
	return iFail;
}

