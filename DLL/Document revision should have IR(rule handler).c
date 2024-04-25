/* Template to regester a DLL */
/* check referenced documentrevision attached to itemrevision then datasets are attached with document revision , Check each document revisison having dataset is attached or not if not print custom */

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<user_exits/user_exits.h>
#include<tc/tc_errors.h>
#include<tc/tc_arguments.h>
#include<tc/tc_startup.h>
#include<tccore/method.h>
#include<bom/bom.h>
#include<tccore/item.h>
#include<tccore/aom.h>
#include<tccore/custom.h>
#include<pom/pom/pom.h>
#include<tccore/aom_prop.h>
#include<epm/epm.h>
#include<fclasses/tc_string.h>
#include<tccore/grm.h>



#define DLLAPI _declspec(dllexport)
#define PLM_error2 (EMH_USER_error_base +3)

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
	printf("\n\n\n*****Welcome to Action Handler DLL Registration*****\n\n\n");
	printf("Login success\n");
	EPM_register_rule_handler("check_datasets", "Custom rule Handler", (EPM_rule_handler_t)add_rule_handler);
	return iFail;
}
//rule Handler without arguments
extern DLLAPI int add_rule_handler(EPM_rule_message_t msg)
{
	int count = 0;
	int count1 = 0;
	int count2 = 0;
	int dataset = 0;
	int showerror = 0;
	int descision = EPM_go;
	tag_t root_task = NULLTAG;
	tag_t *attachments = NULL;
	tag_t *secondary_objects = NULL;
	tag_t *datasets = NULL;
	tag_t class_id = NULLTAG;
	char *name = NULL;
	char *value = NULL;
	char *value1 = NULL;
	char *object_string = NULL;
	char *class_name = NULL;

	EPM_ask_root_task(msg.task, &root_task);
	EPM_ask_attachments(root_task, EPM_target_attachment, &count, &attachments);
	
	/* to check datasets attached to DocRev */
	for (int i = 0; i < count; i++)
	{
		AOM_ask_value_string(attachments[i], "object_type", &value);
		if (tc_strcmp(value, "ItemRevision") == 0)
		{
			GRM_list_secondary_objects_only(attachments[i], NULLTAG, &count1, &secondary_objects);
			for (int j = 0; j < count1; j++)
			{
				AOM_ask_value_string(secondary_objects[j], "object_type", &value1);
				dataset = 0;
				if (tc_strcmp(value1, "DocumentRevision") == 0)
				{
					GRM_list_secondary_objects_only(secondary_objects[j], NULLTAG, &count2, &datasets);
					for (int k = 0; k < count2; k++)
					{
						POM_class_of_instance(datasets[k], &class_id);
						POM_name_of_class(class_id, &class_name);
						if (tc_strcmp(class_name, "Dataset") == 0)
							dataset++;
					}
					if (dataset == 0)
					{
						EMH_store_error_s1(EMH_severity_error, PLM_error2, value1);
						descision = EPM_nogo;
					}
				}
			}
		}
	}
	return descision;
}

extern DLLAPI int Handler_execute_callbacks2(int *decision, va_list argv)
{
	*decision = ALL_CUSTOMIZATIONS;
	printf("\n\n\n*****DLL logout success*****\n\n\n");
	return iFail;
}


