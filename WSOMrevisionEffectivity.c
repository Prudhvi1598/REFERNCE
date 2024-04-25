#include<stdio.h>
#include<string.h>
#include<tcinit/tcinit.h>
#include<tc/tc_startup.h>
#include<tccore/aom.h>
#include<bom/bom.h>
#include<tccore/workspaceobject.h>
#include<tccore/releasestatus.h>

#define LOGIN ITK_init_module("izn","izn","dba")

int ITK_user_main(int argc, char** argv)
{
	int iFail = 0;
	int istatuses = 0;
	int icheck = 0;
	char *crange = "11-15";
	char *cid = NULL;
	tag_t trev = NULLTAG;
	tag_t tendrev = NULLTAG;
	tag_t teffectivity = NULLTAG;
	tag_t *tstatuses =NULLTAG;
	iFail = LOGIN;
	if (iFail == 0)
	{
		printf("TC Login successful!\n---------------------\n\n");
		ITEM_find_rev("000354", "C", &trev);
		if (trev != NULLTAG)
		{
			printf("ItemRevision found!\n\n");
			WSOM_ask_release_status_list(trev, &istatuses, &tstatuses);
			printf("Number of statuses:%d\n\n", istatuses);
			for (int i = 0; i < istatuses; i++)
			{
				WSOM_ask_id_string(tstatuses[i], &cid);
				printf("%s\n\n", cid);
				if (strcmp(cid, "TCM Released")==0)//Error here
				{
					icheck = 1;
					printf("%s found!\n\n", cid);
					ITEM_find_item("000354", &tendrev);
					iFail = WSOM_eff_create_with_unit_text(tstatuses[i], tendrev, crange, &teffectivity);
					AOM_save_without_extensions(teffectivity);
					if (iFail == 0 && teffectivity != NULLTAG)
					{
						printf("revision effectivity set successful!\n\n");
					}
					else
						printf("revision effectivity set Unsuccesful!\n\n");
				}
			}
			if(icheck==0)
				printf("%s found!\n\n", cid);
		}
		else
		{
			printf("ItemRevision not found!\n\n");
		}
	}
	else
	{
		printf("TC Login Failed!\n-------------------------\n\n");
	}
	return ITK_ok;
}
















