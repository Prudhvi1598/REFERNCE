#include<stdio.h>
#include<tcinit/tcinit.h>
#include<tc/tc_startup.h>
#include<bom/bom.h>
#include<pom/enq/old_on_new.h>
#include<pom/pom/pom.h>
#include<tccore/aom_prop.h>
#include<sa/user.h>
#include "Header.h"

#define LOGIN ITK_init_module("izn","izn","dba")

#define POM_enquiry_equal   15000

int ITK_user_main(int argc, char** argv)
{
	int iFail = 0;
	int nval = 0;
	char *char_id = NULL;
	char *object_type = NULL;
	tag_t tag_user = NULLTAG;
	tag_t tag_class_id = NULLTAG;
	tag_t tag_attr_id = NULLTAG;
	tag_t tag_enqid= NULLTAG;
	tag_t *tag_values= NULLTAG;


	iFail = LOGIN;
	if (iFail == 0)
	{
		printf("TC Login successful!\n---------------------\n\n");
		CHECK(SA_find_user2("izn", &tag_user));
		CHECK(POM_class_id_of_class("Dataset", &tag_class_id));
		CHECK(POM_attr_id_of_attr("owning_user", "Dataset", &tag_attr_id));
		CHECK(POM_create_enquiry_on_tag(tag_class_id, tag_attr_id, POM_enquiry_equal, &tag_user, &tag_enqid));
		CHECK(POM_execute_enquiry(tag_enqid, &nval, &tag_values));
		for (int i = 0; i < nval;i++)
		{
			WSOM_ask_id_string(tag_values[i], &char_id);
			AOM_ask_value_string(tag_values[i], "object_type", &object_type);
			printf("%s\t: %s\n",object_type, char_id);
			char *char_id = NULL;
			char *object_type = NULL;
		}
	}
	else
	{
		printf("TC Login Failed!\n-------------------------\n\n");
	}
	return ITK_ok;
}