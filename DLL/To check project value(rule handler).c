/* Template to regester a DLL */
/* Rule handler to check if target object has bvr and its components are assigned to project */

#include<Header.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<tccore/item.h>
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


#define DLLAPI _declspec(dllexport)
#define PLM_error2 (EMH_USER_error_base +4)
#define PLM_error1 (EMH_USER_error_base +8)
#define PLM_error3 (EMH_USER_error_base +7)

METHOD_id_t tmethod_id;
int iFail = ITK_ok;
tag_t revision = NULLTAG;
tag_t child_revision = NULLTAG;
EPM_decision_t decision = EPM_go;
int count = 0;
int bvr_count = 0;
int project_count = 0;
int bom_line_count = 0;
int sec_obj_count = 0;
int no_of_arguments = 0;
tag_t root_task = NULLTAG;
tag_t *attachments = NULL;
tag_t class_id = NULLTAG;
tag_t bvr_tag = NULLTAG;
tag_t window = NULLTAG;
tag_t top_bom_line = NULLTAG;
tag_t *children = NULLTAG;
tag_t *bvrs = NULLTAG;
char *class_name = NULL;
char *class_name1 = NULL;
char *dataset = NULL;
char *project = NULL;
char *search_id = NULL;
char *rev_id = NULL;


//Declaration of function
extern DLLAPI int DLL_register_callbacks();
extern DLLAPI int Handler_execute_callbacks1(int *decision, va_list argv);
extern DLLAPI int Handler_execute_callbacks2(int *decision, va_list argv);
extern DLLAPI int add_rule_handler(EPM_rule_message_t msg);
int multilevel_bom(tag_t);


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
	EPM_register_rule_handler("Dataset-rule-handler", "IR in BOMline should be assigned to project", (EPM_rule_handler_t)add_rule_handler);
	return iFail;
}

//Rule Handler without arguments
extern DLLAPI int add_rule_handler(EPM_rule_message_t msg)
{
	/* to check Target object and its secondary objects */
	(EPM_ask_root_task(msg.task, &root_task));
	(EPM_ask_attachments(root_task, EPM_target_attachment, &count, &attachments));
	for (int i = 0; i < count; i++)
	{
		multilevel_bom(attachments[i]);
	}
	return decision;//EPM_go
}


/* Recursion Function to check multilevel */
int multilevel_bom(tag_t revision)
{
	POM_class_of_instance(revision, &class_id);
	POM_name_of_class(class_id, &class_name);
	if (tc_strcmp(class_name, "ItemRevision") == 0)  // checks the target object is IR or not?
	{
		ITEM_rev_list_all_bom_view_revs(revision, &bvr_count, &bvrs);
		if (bvr_count != 0)
		{
			BOM_create_window(&window);
			BOM_set_window_top_line_bvr(window, bvrs[0], &top_bom_line);
			BOM_line_ask_all_child_lines(top_bom_line, &bom_line_count, &children);
			for (int k = 0; k < bom_line_count; k++)
			{
				AOM_ask_value_string(children[k], "bl_rev_project_ids", &project);
				if (tc_strcmp(project, " ") != 0)
				{
					AOM_ask_value_string(children[k], "bl_item_item_id", &search_id);
					AOM_ask_value_string(children[k], "bl_rev_item_revision_id", &rev_id);
					ITEM_find_rev(search_id, rev_id, &child_revision);
					multilevel_bom(child_revision);
				}
				else
				{
					EMH_store_error_s1(EMH_severity_error, PLM_error3, NULL);
					return PLM_error3;  //All IR does not assigned to project
				}
			}	
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


