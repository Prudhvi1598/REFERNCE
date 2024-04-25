#include<stdio.h>
#include<tcinit/tcinit.h>
#include<tc/tc_startup.h>
#include<tccore/project.h>
#include<sa/group.h>
#include<sa/role.h>
#include<bom/bom.h>
#include "Header.h"

#define LOGIN ITK_init_module("izn","izn","dba")

int ITK_user_main(int argc, char** argv)
{
	int iFail = 0;
	tag_t tag_project = NULLTAG;
	tag_t tag_group1 = NULLTAG;
	tag_t tag_team_administrator = NULLTAG;
	tag_t tag_privilaged_user1 = NULLTAG;

	iFail = LOGIN;
	if (iFail == 0)
	{
		printf("TC Login successful!\n---------------------\n\n");
		CHECK(PROJ_create_project("project3", "project3", NULL, &tag_project));
		printf("project created!\n\n");
		CHECK(SA_find_group("dba", &tag_group1)); 
		tag_t group_members[] = { tag_group1 };      /* Group members to add in project */

		CHECK(SA_find_user2("kumar", &tag_team_administrator)); /* Administrator to add in project */
		CHECK(SA_find_user2("izn", &tag_privilaged_user1)); 
		tag_t privilaged_users[] = { tag_privilaged_user1 };/* privilaged users to add */

		CHECK(PROJ_assign_team(tag_project, 1, group_members,tag_team_administrator,1,privilaged_users));
		printf("Group members are added!\n\n");
	}
	else
	{
		printf("TC Login Failed!\n-------------------------\n\n");
	}
	return ITK_ok;
}














