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
#include<tccore/custom.h>
#include<fclasses/tc_string.h>
#include<tccore/workspaceobject.h>
#include<tccore/grm.h>


#define DLLAPI _declspec(dllexport)
#define PLM_error2 (EMH_USER_error_base +4)
#define PLM_error1 (EMH_USER_error_base +5)

METHOD_id_t tmethod_id;
int iFail = ITK_ok;


//Declaration of function
extern DLLAPI int DLL_register_callbacks();
extern DLLAPI int Handler_execute_callbacks1(int *decision, va_list argv);
extern DLLAPI int Handler_execute_callbacks2(int *decision, va_list argv);
extern DLLAPI int add_rule_handler(EPM_rule_message_t msg);

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
	printf("\n\n\n*****Welcome to Rule Handler DLL Registration*****\n\n\n");
	printf("Login success\n");
	EPM_register_rule_handler("Dataset-rule-handler", "IR should contain any dataset", (EPM_rule_handler_t)add_rule_handler);
	return iFail;
}
//Rule Handler without arguments
extern DLLAPI int add_rule_handler(EPM_rule_message_t msg)
{
	EPM_decision_t decision = EPM_go;
	int count = 0;
	int dataset_count = 0;
	int sec_obj_count = 0;
	int no_of_arguments = 0;
	tag_t root_task = NULLTAG;
	tag_t release_status = NULLTAG;
	tag_t *attachments = NULL;
	tag_t class_id = NULLTAG;
	tag_t class_id1 = NULLTAG;
	tag_t *sec_obj_list = NULL;
	char *class_name = NULL;
	char *class_name1 = NULL;
	char *argument_name = NULL;
	char *argument_value = NULL;
	char *dataset = NULL;
	char *name = NULL;

	/* To store argument value(dataset) given by user for argument -attached_dataset */
	ITK_ask_argument_named_value(TC_next_argument(msg.arguments), &argument_name, &argument_value);
	if (tc_strcmp(argument_name, "attached_dataset") == 0)
	{
		dataset = (char *)MEM_alloc(100);
		tc_strcpy(dataset, argument_value);
	}
	/* to check Target object and its secondary objects */
	CHECK(EPM_ask_root_task(msg.task, &root_task));
	CHECK(EPM_ask_attachments(root_task, EPM_target_attachment, &count, &attachments));
	for (int i = 0; i < count; i++)
	{
		POM_class_of_instance(attachments[i], &class_id);
		POM_name_of_class(class_id, &class_name);
		if (tc_strcmp(class_name, "ItemRevision") == 0)  // checks the target object is IR or not?
		{
			GRM_list_secondary_objects_only(attachments[i], NULLTAG, &sec_obj_count, &sec_obj_list);
			for (int j = 0; j < sec_obj_count; j++)
			{
				WSOM_ask_object_type2(sec_obj_list[j], &name);
				if (tc_strcmp(name, dataset) == 0) // checks count of datasets for IR
				{
					dataset_count++;
				}
			}
			if (dataset_count != 0) // Initiate WF only if it has any type of Dataset
			{
				return decision;//EPM_go
			}
			else
			{
				EMH_store_error_s1(EMH_severity_error, PLM_error1,NULL);
				return PLM_error1;  //Dataset Error
			}
		}
		else
		{
			EMH_store_error_s1(EMH_severity_error, PLM_error2, class_name);
			return PLM_error2; //Target object error
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


