/*
When a dataset is attached to a item revision with some specific relation then dataset name pattern should be in the form of  “ itemid -revid-n 


*/

#include<stdio.h>
#include<conio.h>
#include<tccore/custom.h>
#include<tccore/method.h>
#include<tccore/aom_prop.h>
#include<fclasses/tc_string.h>
#include<tccore/grm.h>
#include<tccore/aom.h>
#include<tccore/grm.h>
#include<tccore/grm_msg.h>
#include<stdlib.h>
#define DLLAPI _declspec(dllexport)

#define PROPERTY "user_data_1"

METHOD_id_t tMethodId;
int iStatus = 0;
extern DLLAPI int DLL_PROJECT_register_callbacks();
extern DLLAPI int PLM_execute_callbacks1(int *decision, va_list argv);
extern DLLAPI int PLM_add_post_action(METHOD_message_t *msg, va_list argv);
extern DLLAPI int PLM_execute_callbacks2(int *decision, va_list argv);
int check(int exp);

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

	iStatus = METHOD_find_method("IMAN_reference", "GRM_create", &tMethodId);//Item_msg.h
	//ItemRevision- Type name
	//ITEM_create_from_rev- Identifies the message for which the method is registered(tccore/item_msg.h)
	//tMethodId- Returns an identifier for the method found
	iStatus = METHOD_add_action(tMethodId, METHOD_post_action_type, (METHOD_function_t)PLM_add_post_action, NULL);


	return iStatus;
}
//Post action
extern DLLAPI int PLM_add_post_action(METHOD_message_t *msg, va_list args)
{
	tag_t tInputRelationType = NULLTAG;
	tag_t tPrimaryObject = NULLTAG;
	tag_t tSecondaryObject = NULLTAG;
	tag_t tRelationType = NULLTAG;
	tag_t tUserData = NULLTAG;
	tag_t tNewRelation = NULLTAG;

	tPrimaryObject= va_arg(args, tag_t);
	tSecondaryObject= va_arg(args, tag_t);
	tRelationType= va_arg(args, tag_t);
	tUserData= va_arg(args, tag_t);
	tNewRelation= va_arg(args, tag_t);

	char *cItemId = NULL;
	char *cRevId = NULL;
	char *cDatasetName = NULL;
	size_t iSize = 0;
	int iCount = 0;
	tag_t *tSecList = NULL;
	char num[10];
	check(AOM_ask_value_string(tPrimaryObject, "item_id", &cItemId));
	if (cItemId != NULL)
	{
		check(AOM_ask_value_string(tPrimaryObject, "item_revision_id", &cRevId));
		if (cRevId != NULL)
		{
			check(GRM_list_secondary_objects_only(tPrimaryObject, tRelationType, &iCount, &tSecList));
			sprintf(num, "%d", iCount);

			iSize = tc_strlen(cItemId) + tc_strlen(cRevId)+tc_strlen(num)+2;
			cDatasetName = (char*)malloc(iSize*sizeof(char));
			tc_strcpy(cDatasetName, "");
			tc_strcat(cDatasetName, cItemId);
			tc_strcat(cDatasetName, "-");
			tc_strcat(cDatasetName, cRevId);
			tc_strcat(cDatasetName, "-");
			tc_strcat(cDatasetName, num);
			//AOM_refresh(tSecondaryObject, true);
			check(AOM_set_value_string(tSecondaryObject,"object_name", cDatasetName));
			//AOM_refresh(tSecondaryObject, false);
			AOM_save_without_extensions(tSecondaryObject);
		}
	}
	if (cItemId) MEM_free(cItemId);
	if (cRevId) MEM_free(cRevId);
	if (tSecList) MEM_free(tSecList);
	return 0;
}



extern DLLAPI int PLM_execute_callbacks2(int *decision, va_list argv)
{
	*decision = ALL_CUSTOMIZATIONS;
	printf("\n\n\n----logout sucess---- \n\n\n");
	return iStatus;
}
int check(int exp) {
	int iFail = 0;
	char *cError = NULL;
	if (exp != iFail)
	{
		EMH_ask_error_text(iFail, &cError);
		printf("The error is :%s\n", cError);
		return 0;
	}
	return 0;
}