
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
#include <epm/epm.h>
#include <bom/bom.h>
#include <ps/ps.h>

#define DLLAPI _declspec(dllexport)
#define PLM_error (EMH_USER_error_base +15)

METHOD_id_t tmethod_id;
int iFail = ITK_ok;


//Declaration of function
extern DLLAPI int DLL_register_callbacks();
extern DLLAPI int PLM_execute_callbacks1(int *decision, va_list argv);
extern DLLAPI int PLM_execute_callbacks2(int *decision, va_list argv);
extern DLLAPI int BOMline_attribute(EPM_rule_message_t);

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
	EPM_register_rule_handler("BOM_check", "hello", (EPM_rule_handler_t)BOMline_attribute);
	return iFail;
}
//Rule handler
extern DLLAPI int BOMline_attribute(EPM_rule_message_t msg)
{
	int count = 0;
	int count1 = 0;
	int count2 = 0;
	int n_parents = 0;
	int* levels = 0;
	tag_t root_task=NULLTAG;
	tag_t item = NULLTAG;
	tag_t class_id = NULLTAG;
	tag_t window = NULLTAG;
	tag_t top_bom_line = NULLTAG;
	tag_t* attachments = NULL;
	tag_t* bv_list = NULL;
	tag_t* children = NULL;
	tag_t* parents = NULL;
	char* class_name = NULL;
	char* project = NULL;
	char* objstring = NULL;
	char* bl_string = NULL;

	EPM_ask_root_task(msg.task, &root_task);
	EPM_ask_attachments(root_task, EPM_target_attachment, &count, &attachments);
	for (int i = 0; i < count; i++)
	{
		POM_class_of_instance(attachments[i], &class_id);
		POM_name_of_class(class_id, &class_name);
		/* If the Item revision is parent then fetch its any bom line property directly */
		if (tc_strcmp(class_name, "A2FruitRevision") == 0)
		{
			ITEM_ask_item_of_rev(attachments[i], &item);
			ITEM_list_bom_views(item, &count1, &bv_list);
			if (count1 != 0)
			{
				BOM_create_window(&window);
				BOM_set_window_top_line(window, item, NULLTAG, bv_list[0], &top_bom_line);
				AOM_ask_value_string(top_bom_line, "bl_rev_project_ids", &project);
				printf("%s\n", project);
				if (tc_strcmp(project, "") == 0)
					return EPM_nogo;
			}
			else
			{
				/* If the Item revision is not parent then find its parent later the child line property */
				AOM_ask_value_string(attachments[i], "item_id", &objstring);
				PS_where_used_all(attachments[i], PS_where_used_all_levels, &n_parents, &levels, &parents);
				for (int j = 0; j < n_parents; j++)
				{
					POM_class_of_instance(parents[j], &class_id);
					POM_name_of_class(class_id, &class_name);
					if (tc_strcmp(class_name, "A2FruitRevision") == 0)
					{
						ITEM_ask_item_of_rev(parents[j], &item);
						ITEM_list_bom_views(item, &count1, &bv_list);
						if (count1 != 0)
						{
							BOM_create_window(&window);
							BOM_set_window_top_line(window, item, NULLTAG, bv_list[0], &top_bom_line);
							BOM_line_ask_all_child_lines(top_bom_line, &count2, &children);
							for (int j = 0; j < count2; j++)
							{
								AOM_ask_value_string(children[j], "bl_item_item_id", &bl_string);
								if (tc_strcmp(objstring, bl_string) == 0)
								{
									AOM_ask_value_string(children[j], "bl_rev_project_ids", &project);
									printf("%s\n", project);
									if (tc_strcmp(project, "") == 0)
										return EPM_nogo;
								}
							}
						}
					}
				}
			}
		}
	}
	return EPM_go;
}

extern DLLAPI int PLM_execute_callbacks2(int *decision, va_list argv)
{
	*decision = ALL_CUSTOMIZATIONS;
	printf("\n\n\n*****DLL logout success*****\n\n\n");
	return iFail;
}

