/* Template to regester a DLL */
/* Rule handler to check if target object has bvr and its components are assigned to project */

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
#include<tccore/aom.h>
#include<fclasses/tc_string.h>
#include<tccore/workspaceobject.h>
#include<tccore/grm.h>
#include<bom/bom.h>
#include<bom/bom_msg.h>
#include<tccore/aom_prop.h>


#define DLLAPI _declspec(dllexport)
#define PLM_error1 (EMH_USER_error_base +1)


METHOD_id_t tmethod_id;


//Declaration of function
extern DLLAPI int DLL_register_callbacks();
extern DLLAPI int Handler_execute_callbacks1(int *decision, va_list argv);
extern DLLAPI int Handler_execute_callbacks2(int *decision, va_list argv);
extern DLLAPI int eventDuplicateStructurePost(METHOD_message_t*  m, va_list args);
int multilevel_bom(tag_t);


//Definition of function
extern DLLAPI int DLL_register_callbacks()
{
	CUSTOM_register_exit("DLL", "USER_init_module", (CUSTOM_EXIT_ftn_t)Handler_execute_callbacks1);
	CUSTOM_register_exit("DLL", "USER_exit_module", (CUSTOM_EXIT_ftn_t)Handler_execute_callbacks2);
	return 0;
}

extern DLLAPI int Handler_execute_callbacks1(int *decision, va_list argv)
{
	*decision = ALL_CUSTOMIZATIONS;
	printf("\n\n\n*****Welcome to Rule Handler DLL Registration*****\n\n\n");
	printf("Login success\n");
	METHOD_find_method("BOMLine", BOM_duplicate_structure_post_msg, &tmethod_id);
	METHOD_add_action(tmethod_id, METHOD_post_action_type, eventDuplicateStructurePost, NULL);
	return 0;
}
	
	int eventDuplicateStructurePost(METHOD_message_t*  m, va_list args)
	{
		va_list largs;
		va_copy(largs, args);
		tag_t toplineTag = va_arg(largs, tag_t);
		tag_t new_item_rev = va_arg(largs, tag_t);
		va_end(largs);
		int count = 0;
		tag_t* children = NULL;
		tag_t window = NULLTAG;
		tag_t new_line = NULLTAG;
		tag_t revision = NULLTAG;

		BOM_create_window(&window);
		BOM_line_ask_all_child_lines(toplineTag, &count, &children);
		for (int i = 0; i < count; i++)
		{
			if (i == count - 1)
			{
				ITEM_find_rev("000042", "B", &revision);
				BOM_line_add_substitute(children[i], NULLTAG, revision, NULLTAG, &new_line);
				AOM_save_without_extensions(new_line);
				BOM_save_window(window);
			}
		}
		return 0;
	}


extern DLLAPI int Handler_execute_callbacks2(int *decision, va_list argv)
{
	*decision = ALL_CUSTOMIZATIONS;
	printf("\n\n\n*****DLL logout success*****\n\n\n");
	return 0;
}


