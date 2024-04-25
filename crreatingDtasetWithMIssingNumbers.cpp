
#include<stdio.h>
#include<conio.h>
#include<tccore/custom.h>
#include<tccore/method.h>
#include<tccore/aom_prop.h>
#include<fclasses/tc_string.h>
#include<tccore/grm.h>
#include<tccore/aom.h>
#include<tc/tc_macros.h>
#include<string>
#include<vector>
#include<algorithm>
#include<iostream>
#define DLLAPI _declspec(dllexport)
using namespace std;

METHOD_id_t tMethodId;

int iStatus = 0;
vector<int> v1;
extern DLLAPI int DLL_PROJECT_register_callbacks();
extern DLLAPI int PLM_execute_callbacks1(int *decision, va_list argv);
extern DLLAPI int PLM_add_pre_action(METHOD_message_t *msg, va_list argv);
extern DLLAPI int PLM_execute_callbacks2(int *decision, va_list argv);
void FindingLastDigit(string str);
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

	iStatus = METHOD_find_method("IMAN_reference", "IMAN_create", &tMethodId);
	//ItemRevision- Type name
	//IMAN_delete- (tc_msg.h)
	//tMethodId- Returns an identifier for the method found
	iStatus = METHOD_add_action(tMethodId, METHOD_pre_action_type, (METHOD_function_t)PLM_add_pre_action, NULL);


	return iStatus;
}
//Post action
extern DLLAPI int PLM_add_pre_action(METHOD_message_t *msg, va_list args)
{
	tag_t tPrimaryObject = NULLTAG;
	tag_t tSecondaryObject = NULLTAG;
	tag_t tRelationType = NULLTAG;

	tPrimaryObject = va_arg(args, tag_t);
	tSecondaryObject = va_arg(args, tag_t);
	tRelationType = va_arg(args, tag_t);

	if (tPrimaryObject != NULLTAG && tSecondaryObject != NULLTAG && tRelationType != NULLTAG)
	{

	}
	return 0;
}

extern DLLAPI int PLM_execute_callbacks2(int *decision, va_list argv)
{
	*decision = ALL_CUSTOMIZATIONS;
	printf("\n\n\n----logout sucess---- \n\n\n");
	return iStatus;
}

int FindingLastDigit(string str)
{
	size_t nameSize = str.size();
	//cout<<"the name size is = "<<nameSize<<endl;
	size_t FirstPos = str.find("-");
	//cout<<FirstPos<<endl;
	size_t SecondPos = str.find("-", FirstPos + 1);
	//cout<<SecondPos<<endl;
	string str1 = str.substr(SecondPos + 1, nameSize - SecondPos + 1);
	//cout<<"the value is = "<<str1<<endl;
	int num = stoi(str1);
	v1.push_back(num);
}

int UpdatingName()
{
	
	return 0;
}

/*string str = j.to_string();

sprintf(num, "%d", iCount);
cDatasetName = (char*)malloc(iSize * sizeof(char));
tc_strcpy(cDatasetName, "");
tc_strcat(cDatasetName, cItemId);
tc_strcat(cDatasetName, "-");
tc_strcat(cDatasetName, cRevId);
tc_strcat(cDatasetName, "-");
tc_strcat(cDatasetName, num);
AOM_refresh(tSecondaryObject, true);
AOM_set_value_string(tSecondaryObject, "object_name", cDatasetName);
AOM_save_without_extensions(tSecondaryObject);
AOM_refresh(tSecondaryObject, false);*/