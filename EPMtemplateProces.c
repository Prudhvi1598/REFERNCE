#include<stdio.h>
#include<tcinit/tcinit.h>
#include<tc/tc_startup.h>
#include<epm/epm_task_template_itk.h>
#include<epm/epm.h>
#include<bom/bom.h>
#include "Header.h"

#define LOGIN ITK_init_module("izn","izn","dba")

int ITK_user_main(int argc, char** argv)
{
	int iFail = 0;
	tag_t t_process_template = NULLTAG;
	tag_t t_rev = NULLTAG;
	tag_t t_new_process = NULLTAG;
	int attach_types[1] = { 1 };

	iFail = LOGIN;
	if (iFail == 0)
	{
		printf("TC Login successful!\n---------------------\n\n");
		CHECK(EPM_find_process_template("TCM Release Process", &t_process_template));
		printf("Process Template found!\n\n");
		CHECK(ITEM_find_rev("000388", "A", &t_rev));
		printf("Target object found!\n\n");
		CHECK(EPM_create_process("TCM Release Process", NULL, t_process_template, 1, &t_rev, attach_types, &t_new_process));
		AOM_save_without_extensions(t_new_process);
		printf("Process template initiated succesfully!\n\n");
	}
	else
	{
		printf("TC Login Failed!\n-------------------------\n\n");
	}
	return ITK_ok;
}