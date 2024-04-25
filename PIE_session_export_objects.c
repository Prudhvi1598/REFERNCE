#include<stdio.h>
#include<tcinit/tcinit.h>
#include<tc/tc_startup.h>
#include<pie/pie.h>
#include<bom/bom.h>
#include "Header.h"

#define LOGIN ITK_init_module("izn","izn","dba")

int ITK_user_main(int argc, char** argv)
{
	int iFail = 0;
	int n_transfer_mode = 0;
	tag_t tag_item = NULLTAG;
	tag_t tag_session = NULLTAG;
	tag_t *tag_transfer_mode = NULLTAG;


	iFail = LOGIN;
	if (iFail == 0)
	{
		printf("TC Login successful!\n---------------------\n\n");
		CHECK(ITEM_find_item("000321", &tag_item));
		tag_t n_objects[] = { tag_item };
		CHECK(PIE_create_session(&tag_session));
		CHECK(PIE_session_set_file(tag_session, "D:\\TC13setup\\TC Exported files\\000321.xml"));
		CHECK(PIE_session_set_log_file(tag_session, "D:\\TC13setup\\TC Exported files\\000321.log"));
		CHECK(PIE_find_transfer_mode2("ConfiguredDataExportDefault", NULL, &n_transfer_mode, &tag_transfer_mode));
		CHECK(PIE_session_set_transfer_mode(tag_session, tag_transfer_mode[0]));
		CHECK(PIE_session_export_objects(tag_session, 1, n_objects));
		PIE_delete_session(tag_session);
		printf("File exported succesfully\n");
	}
	else
	{
		printf("TC Login Failed!\n-------------------------\n\n");
	}
	return ITK_ok;
}
















