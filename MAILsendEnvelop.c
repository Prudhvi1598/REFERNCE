#include<stdio.h>
#include<tcinit/tcinit.h>
#include<tc/tc_startup.h>
#include<tc/envelope.h>
#include<tc/folder.h>
#include<bom/bom.h>
#include<sa/user.h>

#define LOGIN ITK_init_module("izn","izn","dba")

int ITK_user_main(int argc, char** argv)
{
	int iFail = 0;
	tag_t tenvelop = NULLTAG;
	tag_t tuser = NULLTAG;
	tag_t tattachment = NULLTAG;


	iFail = LOGIN;
	if (iFail == 0)
	{
		printf("TC Login successful!\n---------------------\n\n");
		MAIL_create_envelope("000357-View", "000357-View", &tenvelop);
		MAIL_initialize_envelope2(tenvelop, "ITK subject", "ITK comments");
		if (tenvelop != NULLTAG)
		{
			printf("Envelop created!\n\n");
			SA_find_user2("izn", &tuser);
			MAIL_add_envelope_receiver(tenvelop, tuser);
			ITEM_find_item("000351", &tattachment);
			if (tattachment != NULLTAG)
			{
				printf("Attachment object found!\n\n");
				FL_insert(tenvelop, tattachment, 1);
					iFail = MAIL_send_envelope(tenvelop);
				if (iFail == 0)
					printf("Mail sent!\n\n");
				else
					printf("Mail not sent!\n\n");
			}
			else
			{
				printf("Attachment object not found!\n\n");
			}
		}
		else
			printf("Envelop not created!\n\n");
	}
	else
	{
		printf("TC Login Failed!\n-------------------------\n\n");
	}
	return ITK_ok;
}