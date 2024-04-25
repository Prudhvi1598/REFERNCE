#include<stdio.h>
#include<conio.h>
#include<tccore/custom.h>
#include<tccore/method.h>
#include<tccore/aom_prop.h>
#include<fclasses/tc_string.h>
#include<tccore/grm.h>
#include<tccore/aom.h>
#include<tc\preferences.h>
#include<pom/pom/pom.h>
#define DLLAPI _declspec(dllexport)


METHOD_id_t tMethodId;
int iStatus = 0;
extern DLLAPI int DLL_PROJECT_register_callbacks();
extern DLLAPI int PLM_execute_callbacks1(int *decision, va_list argv);
extern DLLAPI int PLM_add_post_action(METHOD_message_t *msg, va_list argv);
extern DLLAPI int PLM_execute_callbacks2(int *decision, va_list argv);
int check(int exp);
int Func(tag_t tag, char *prop, char *value);
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

	iStatus = METHOD_find_method("A4Object2", "ITEM_create", &tMethodId);//Item_msg.h
	//ItemRevision- Type name
	//ITEM_create_from_rev- Identifies the message for which the method is registered(tccore/item_msg.h)
	//tMethodId- Returns an identifier for the method found
	iStatus = METHOD_add_action(tMethodId, METHOD_post_action_type, (METHOD_function_t)PLM_add_post_action, NULL);


	return iStatus;
}

extern DLLAPI int PLM_add_post_action(METHOD_message_t *msg, va_list argv)
{
	const char *cItemId = NULL;
	const char *cItemName = NULL;
	const char *cTypeName = NULL;
	const char *cRevId = NULL;
	tag_t *tSource = NULLTAG;
	//tag_t tRev = NULLTAG;
	cItemId = va_arg(argv, char*);
	cItemName=va_arg(argv, char*);
	cTypeName=va_arg(argv, char*);
	cRevId=va_arg(argv, char*);
	tSource = va_arg(argv, tag_t*);
	//tRev = va_arg(argv, tag_t);
	
	char *cPreference = "DLL_Preference";
	int iCount = 0;
	char **cValues = NULL;
	char *num=NULL;
	char *Alpha = NULL;
	char *cString = NULL;
	char *cName = NULL;
	char *cName2 = NULL;
	tag_t tItemtag = NULLTAG;
	tItemtag = *tSource;
	char *property = NULL;
	check(AOM_ask_value_string(tItemtag, "object_name", &cName));//Checking whether the correct Item tag is found or not
	printf("%s\n", cName);

	check(PREF_ask_char_values(cPreference, &iCount, &cValues));//Retrieving preference values
	if (cValues != NULL && iCount != 0)
	{
		for (int i = 0; i < iCount; i++)
		{
			num = tc_strtok(cValues[i], ":");
			Alpha = tc_strtok(NULL, ":");

			check(AOM_ask_value_string(tItemtag, "a4Number", &cString));//Getting the "a4Number" property value
			if (tc_strlen(cString)>0)
			{
				if (tc_strcmp(cString, num) == 0)
				{
					property = "a4Alphabet";
					Func(tItemtag, property, Alpha);//Calling the function to update property
					break;
				}
				
			}
			else
			{
				check(AOM_ask_value_string(tItemtag, "a4Alphabet", &cString));//Getting the "a4Alphabet" property value
				if (tc_strlen(cString) > 0)
				{
					property = "a4Number";
					Func(tItemtag, property, num);//Calling the function to update property
					break;
				}
			}

		}
	}
	if (cName) 
		MEM_free(cName);
	if (cName)
		MEM_free(cValues);
	if (cName)
		MEM_free(cString);
	return iStatus;
}


extern DLLAPI int PLM_execute_callbacks2(int *decision, va_list argv)
{
	*decision = ALL_CUSTOMIZATIONS;
	printf("\n\n\n----logout sucess---- \n\n\n");
	return iStatus;
}

int check(int exp) {										//Function to check iFail condition
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
int Func(tag_t tag, char *prop, char *value)				//Function to update property value
{
	check(AOM_refresh(tag, true));
	printf("Item checked out\n");
	check(AOM_set_value_string(tag, prop, value));
	printf("value updated\n");
	check(AOM_save_without_extensions(tag));
	printf("Item saved\n");
	check(AOM_refresh(tag, false));
	return 0;
}