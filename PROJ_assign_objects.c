#include<stdio.h>
#include<tcinit/tcinit.h>
#include<tc/tc_startup.h>
#include<tccore/project.h>
#include<bom/bom.h>
#include "Header.h"

#define LOGIN ITK_init_module("izn","izn","dba")

int ITK_user_main(int argc, char** argv)
{
	int iFail = 0;
	tag_t tag_item = NULLTAG;
	tag_t tag_project = NULLTAG;


	iFail = LOGIN;
	if (iFail == 0)
	{
		printf("TC Login successful!\n---------------------\n\n");
		CHECK(ITEM_find_item("000321", &tag_item));
		CHECK(PROJ_find("project2", &tag_project));
		tag_t projects[] = { tag_project };
		tag_t objects[] = { tag_item };
		CHECK(PROJ_assign_objects(1, projects, 1, objects));
		printf("Project assigned\n\n");
	}
	else
	{
		printf("TC Login Failed!\n-------------------------\n\n");
	}
	return ITK_ok;
}
