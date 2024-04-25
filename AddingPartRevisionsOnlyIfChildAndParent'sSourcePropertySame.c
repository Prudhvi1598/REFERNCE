/*
Should allow adding a child item to parent item in structure manager when a property “source” value  is same on both.  
-------------PRECONDITION--------------  
*/
#include<stdio.h>
#include<conio.h>
#include<tccore/custom.h>
#include<tccore/method.h>
#include<tccore/aom_prop.h>
#include<fclasses/tc_string.h>
#include<tc/emh.h>
#include<bom/bom_msg.h>
#include<bom/bom.h>
#include<tccore/workspaceobject.h>
#include<tccore/item.h>
#define OBJECT_TYPE "Item"
#define DLLAPI _declspec(dllexport)
#define PLM_error (EMH_USER_error_base +14)

METHOD_id_t tMethodId;
int iStatus = 0;
extern DLLAPI int DLL_PROJECT_register_callbacks();
extern DLLAPI int PLM_execute_callbacks1(int *decision, va_list argv);
extern DLLAPI int PLM_add_precondition(METHOD_message_t *msg, va_list argv);
extern DLLAPI int PLM_execute_callbacks2(int *decision, va_list argv);
int check(int exp);
int compare_source_property(int iValue, tag_t tRevision);
extern DLLAPI int DLL_PROJECT_register_callbacks()
{
	iStatus = CUSTOM_register_exit("DLL_PROJECT", "USER_init_module", (CUSTOM_EXIT_ftn_t)PLM_execute_callbacks1);
	// This function registers a custom exit (a custom function pointer) for a given USER_ and USERSERVICE_ exit function
	iStatus = CUSTOM_register_exit("DLL_PROJECT", "USER_exit_module", (CUSTOM_EXIT_ftn_t)PLM_execute_callbacks2);
	return iStatus;
}


extern DLLAPI int PLM_execute_callbacks1(int *decision, va_list argv)
{
	*decision = ALL_CUSTOMIZATIONS;
	printf("\n\n\n----welcome dll registration process---- \n\n\n");
	printf("\n\n\n----login sucess---- \n\n\n");

	iStatus = METHOD_find_method("BOMLine", "BOMLine_add", &tMethodId);//bom_msg.h
	
	iStatus = METHOD_add_pre_condition(tMethodId, (METHOD_function_t)PLM_add_precondition, NULL);

	return iStatus;
}

extern DLLAPI int PLM_add_precondition(METHOD_message_t *msg, va_list argv)
{
	int decision=0;
	printf("Started\n");
	tag_t tParent = NULLTAG;
	tag_t tPart = NULLTAG;
	tag_t tPartRevision = NULLTAG;

	tParent= va_arg(argv, tag_t);
	tPart= va_arg(argv, tag_t);
	tPartRevision =va_arg(argv, tag_t);

	char *cPvalue = NULL;
	char *cCvalue = NULL;
	tag_t tRevTag = NULLTAG;
	tag_t tChildRev = NULLTAG;
	int iPvalue = 0;
	int icvalue = 0;
	if (tParent != NULLTAG)
	{
		check(AOM_ask_value_tag(tParent, "bl_revision", &tRevTag));
		if (tRevTag != NULLTAG)
		{
			printf("Parent Revision tag found\n");
			check(AOM_ask_value_int(tRevTag, "source", &iPvalue));
			printf("Parent Source property fetched\n");
			if (tPart != NULLTAG && tPartRevision == NULLTAG)
			{
				printf("Copied Part\n");
				ITEM_ask_latest_rev(tPart, &tChildRev);
				if (tChildRev != NULLTAG)
				{
					printf("Part revision tag fetched for part\n");
					compare_source_property(iPvalue, tChildRev);
				}
				
			}
			else if (tPart == NULLTAG && tPartRevision != NULLTAG)
			{
				printf("Copied Part revision\n");
				decision=compare_source_property(iPvalue, tPartRevision);
			}
		}
	}
	//





	if (decision == 0) return PLM_error;
	
	return 0;
}
int compare_source_property(int iValue,tag_t tRevision)
{
	int iFlag = 0;
	int iCvalue=-1;
	char *cObjectName1 = NULL;
	check(AOM_ask_value_int(tRevision, "source", &iCvalue));
	if (iValue == iCvalue)
	{
		printf("Values same\n");
		iFlag = 1;
	}
	else
	{
		check(AOM_ask_value_string(tRevision, "object_string", &cObjectName1));
		if (cObjectName1 != NULL)
		{
			check(EMH_store_error_s1(EMH_severity_error, PLM_error, cObjectName1));
		}

	}
	if (cObjectName1) MEM_free(cObjectName1);
	return iFlag;
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
	}
	return 0;
}