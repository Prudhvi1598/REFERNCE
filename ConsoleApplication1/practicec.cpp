#include<stdio.h>
#include<tccore/grm.h>
#include<tc/tc_startup.h>
#include<tcinit\tcinit.h>
#include<tccore/item.h>
#include<tc/tc_macros.h>
#include<tccore/aom_prop.h>
#include<string>
#include<vector>
#include<iostream>
using namespace std;
vector <int>  FindingLastDigit(string str);
int ITK_user_main(int argc, char* argv[])
{

	/*tag_t tRevTag = NULLTAG;
	tag_t tType = NULLTAG;
	char *cValue = NULL;
	int iCount = 0;
	tag_t *list = NULL;
	char* cName= NULL;
	ITKCALL(ITK_init_module("hello", "hello", "dba"));
	ITKCALL(ITEM_find_rev("000346","A", &tRevTag));
	GRM_find_relation_type("IMAN_reference", &tType);
	GRM_list_secondary_objects_only(tRevTag, tType, &iCount,&list);
	for (int i = 0; i < iCount; i++)
	{
		AOM_ask_value_string(list[i], "object_name", &cName);
		printf("%s\n", cName);
	}*/


	cout << FindingLastDigit("000124-S-9")[0] <<"p";
	return 0;
}
vector <int>  FindingLastDigit(string str)
{
	vector<int>n;
	size_t nameSize = str.size();
	size_t FirstPos = str.find("-");
	size_t SecondPos = str.find("-", FirstPos + 1);
	string str1 = str.substr(SecondPos + 1, nameSize - SecondPos + 1);
	int num = stoi(str1);
	n.push_back(num);
	return n;
}