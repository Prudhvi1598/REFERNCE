#include<stdio.h>
#include<tcinit/tcinit.h>
#include<tc/tc_startup.h>
#include<res/res_itk.h>
#include<sa/user.h>
#include<tccore/aom_prop.h>
#include "Header.h"

#define LOGIN ITK_init_module("izn","izn","dba")

#define POM_enquiry_equal   15000

int ITK_user_main(int argc, char** argv)
{
	int iFail = 0;
	int nval = 0;
	char *char_id = NULL;
	char *char_id1 = NULL;
	tag_t *object_tag = NULL;
	tag_t tag_user = NULLTAG;
	
	


	iFail = LOGIN;
	if (iFail == 0)
	{
		printf("TC Login successful!\n---------------------\n\n");
		CHECK(SA_find_user2("izn", &tag_user));
		CHECK(RES_list_checkout(tag_user, &nval, &object_tag));
		printf("object_string\tobject_type\n________________________________\n\n");
		for (int i = 0; i < nval; i++)
		{
			CHECK(AOM_ask_value_string(object_tag[i], "object_string", &char_id));
			CHECK(AOM_ask_value_string(object_tag[i], "object_type", &char_id1));
			printf("%s\t%s\n", char_id,char_id1);
		}
	}
	else
	{
		printf("TC Login Failed!\n-------------------------\n\n");
	}
	return ITK_ok;
}