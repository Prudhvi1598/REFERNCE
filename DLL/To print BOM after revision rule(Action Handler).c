/* Template to regester a DLL */
/* Rule handler to check if target object has bvr and its components are assigned to project */

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
#include<bom/bom.h>
#include<tccore/aom_prop.h>
#include<cfm/cfm.h>


#define DLLAPI _declspec(dllexport)
#define PLM_error2 (EMH_USER_error_base +4)
#define PLM_error1 (EMH_USER_error_base +8)
#define PLM_error3 (EMH_USER_error_base +8)

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
	printf("\n\n\n*****Welcome to Rule Handler DLL Registration*****\n\n\n");
	printf("Login success\n");
	EPM_register_action_handler("BOM-print-rev-rule", NULL, (EPM_action_handler_t)add_action_handler);
	return iFail;
}
//Action Handler without arguments
extern DLLAPI int add_action_handler(EPM_action_message_t msg)
{
	EPM_decision_t decision = EPM_go;
	int count = 0;
	int bvr_count = 0;
	int bom_line_count = 0;
	int no_of_arguments = 0;
	tag_t root_task = NULLTAG;
	tag_t *attachments = NULL;
	tag_t class_id = NULLTAG;
	tag_t window = NULLTAG;
	tag_t rule = NULLTAG;
	tag_t top_bom_line = NULLTAG;
	tag_t *children = NULLTAG;
	tag_t *bvrs = NULLTAG;
	char *class_name = NULL;
	char *argument_name = NULL;
	char *argument_value = NULL;
	char *rev_rule = NULL;
	char *title = NULL;
	char *config = NULL;
	char *seq = NULL;


	EPM_ask_root_task(msg.task, &root_task);
	EPM_ask_attachments(root_task, EPM_target_attachment, &count, &attachments);
	no_of_arguments = TC_number_of_arguments(msg.arguments);
	/* To Fetch argument value*/
	for (int i = 0; i < no_of_arguments; i++)
	{
		ITK_ask_argument_named_value(TC_next_argument(msg.arguments), &argument_name, &argument_value);
		if (tc_strcmp(argument_name, "rev_rule") == 0)
		{
			CFM_find(argument_value, &rule);
		}
	}

    /* To Apply revision rule and print BOM */
	for (int i = 0; i < count; i++)
	{
		POM_class_of_instance(attachments[i], &class_id);
		POM_name_of_class(class_id, &class_name);
		if (tc_strcmp(class_name, "ItemRevision") == 0)  // checks the target object is IR or not?
		{
			ITEM_rev_list_all_bom_view_revs(attachments[i], &bvr_count, &bvrs);
			if (bvr_count != 0)
			{
				BOM_create_window(&window);
				BOM_set_window_top_line_bvr(window, bvrs[0], &top_bom_line);
				BOM_set_window_config_rule(window, rule); // To apply revision rule
				BOM_line_ask_all_child_lines(top_bom_line, &bom_line_count, &children);
				FILE *file = fopen("C:\\Users\\T50361\\Desktop\\Assignments\\BOMPrint.txt", "w");
				if (file!= NULL)
				{
					fprintf(file, "%s\t%s\t%s\n", "bl_indented_title", "bl_config_sring", "bl_sequence_no");
					for (int k = 0; k < bom_line_count; k++)
					{
						AOM_ask_value_string(children[k], "bl_indented_title", &title);
						AOM_ask_value_string(children[k], "bl_config_sring", &config);
						AOM_ask_value_string(children[k], "bl_sequence_no", &seq);
						fprintf(file, "%s\t", title);
						fprintf(file, "%s\t", config);
						fprintf(file, "%s\t\n", seq);
					}
				}
			}
			else
			{
				EMH_store_error_s1(EMH_severity_error, PLM_error1, NULL);
				return PLM_error1;  //BOM view Error
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
