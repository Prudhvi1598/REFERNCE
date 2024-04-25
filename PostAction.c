//After Save As Operation on item revision,adding some data into Item Revision Master form

#include<stdio.h>
#include<conio.h>
#include<tccore/custom.h>
#include<tccore/method.h>
#include<tccore/aom_prop.h>
#include<fclasses/tc_string.h>
#include<tccore/grm.h>
#include<tccore/aom.h>

#define DLLAPI _declspec(dllexport)

#define PROPERTY "user_data_1"

METHOD_id_t tMethodId;
int iStatus = 0;
extern DLLAPI int DLL_PROJECT_register_callbacks();
extern DLLAPI int PLM_execute_callbacks1(int *decision, va_list argv);
extern DLLAPI int PLM_add_post_action(METHOD_message_t *msg, va_list argv);
extern DLLAPI int PLM_execute_callbacks2(int *decision, va_list argv);

extern DLLAPI int DLL_PROJECT_register_callbacks()
{
	iStatus = CUSTOM_register_exit("DLL_PROJECT", "USER_init_module", (CUSTOM_EXIT_ftn_t)PLM_execute_callbacks1);
	
	iStatus = CUSTOM_register_exit("DLL_PROJECT", "USER_exit_module", (CUSTOM_EXIT_ftn_t)PLM_execute_callbacks2);
	return iStatus;
}


extern DLLAPI int PLM_execute_callbacks1(int *decision, va_list argv)
{
	*decision = ALL_CUSTOMIZATIONS;
	printf("\n\n\n----welcome dll registration process---- \n\n\n");
	printf("\n\n\n----login sucess---- \n\n\n");

	iStatus = METHOD_find_method("ItemRevision", "ITEM_create_from_rev", &tMethodId);//Item_msg.h
	//ItemRevision- Type name
	//ITEM_create_from_rev- Identifies the message for which the method is registered(tccore/item_msg.h)
	//tMethodId- Returns an identifier for the method found
	iStatus = METHOD_add_action(tMethodId, METHOD_post_action_type,(METHOD_function_t)PLM_add_post_action, NULL);
	

	return iStatus;
}
//Post action
extern DLLAPI int PLM_add_post_action(METHOD_message_t *msg, va_list argv)
{
	tag_t tSource = NULLTAG;
	tSource = va_arg(argv, tag_t);
	tag_t tRelationType = NULLTAG;
	tag_t *tSecObject = NULL;
	int iCount;
	if (GRM_find_relation_type("IMAN_master_form_rev", &tRelationType) == iStatus)//Retrieves the relation type of the given relation type name
	{
		if (GRM_list_secondary_objects_only(tSource, tRelationType, &iCount, &tSecObject) == iStatus)
		{
			for (int i = 0; i < iCount; i++)
			{
				AOM_UIF_set_value(tSecObject[i], PROPERTY, "save As operation performed");
				
				AOM_save_without_extensions(tSecObject[i]);
			}
		}
	}
	
	MEM_free(tSecObject);
	return iStatus;
}

extern DLLAPI int PLM_execute_callbacks2(int *decision, va_list argv)
{
	*decision = ALL_CUSTOMIZATIONS;
	printf("\n\n\n----logout sucess---- \n\n\n");
	return iStatus;
}