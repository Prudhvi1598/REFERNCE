/*
-----SENDING MAIL----


	-u=hello -p=hello -g=dba -userid=iz -subject=mailthroughitk -comment=mailthroughitk 

	LOGIN INTO TEAMCENTER MODULE
	FIND USER TAG(RECEIVER) TO WHOM YOU WANT TO SEND MAIL
	CREATE AN ENVELOPE
	ADD ENVELOPE RECEIVER
	INSERT ANY ATTACHMENTS YOU WANT(FL_INSERT-FOLDER.H)
	SEND ENVELOPE 

*/
#include<iostream>
#include<tc/tc_startup.h>
#include<tcinit\tcinit.h>
#include<tc/emh.h>
#include<tccore/item.h>
#include<tc/envelope.h>
#include<sa/user.h>
#include<tc/folder.h>
using namespace std;
int check(int exp);
int ITK_user_main(int argc, char* argv[])
{

	int iCount = 0;
	char *cValue = NULL;
	tag_t tUserIdTag = NULLTAG;
	tag_t tEnvelope = NULLTAG;
	tag_t *tReferencedObjects = NULL;
	tag_t ItemTag = NULLTAG;
	char *cUser = ITK_ask_cli_argument("-u=");
	char *cPass = ITK_ask_cli_argument("-p=");
	char *cGroup = ITK_ask_cli_argument("-g=");
	char *cUserId = ITK_ask_cli_argument("-userid=");
	
	char *cItemId = ITK_ask_cli_argument("-itemid=");
	const char *cSubject = ITK_ask_cli_argument("-subject=");
	const char *cMailComment = ITK_ask_cli_argument("-comment=");
	//char *cExtMail = ITK_ask_cli_argument("-extmail=");
	if (cUser != NULL && cPass != NULL && cGroup != NULL)
	{
		check(ITK_init_module(cUser, cPass, cGroup));
		cout << "Login sucessful\n" << endl;
		check(ITEM_find_item(cItemId, &ItemTag));
		check(SA_find_user2(cUserId, &tUserIdTag));
		if (tUserIdTag != NULLTAG)
		{
			cout << "User tag found" << endl;
			check(MAIL_create_envelope(cSubject, cMailComment, &tEnvelope));
			if (tEnvelope != NULLTAG)
			{
				cout << "mail created" << endl;
				check(MAIL_add_envelope_receiver(tEnvelope, tUserIdTag));
				cout << "Envelope reciever added" << endl;
				//check(MAIL_add_external_receiver(tEnvelope, MAIL_send_to, cExtMail));
				//cout << "External mail id added" << endl;
				check(FL_insert(tEnvelope, ItemTag,1));
				cout << "Item attached to mail" << endl;
				check(MAIL_send_envelope(tEnvelope));
				cout << "mail sent to the receiver" << endl;

			}
			else
			{
				cout << "Envelope not created" << endl;
			}
		}
		else
		{
			cout << "User tag not found" << endl;
		}


	}
	else
	{
		cout << "Enter all credentials" << endl;
	}
	return 0;
	ITK_exit_module(true);
}

int check(int exp) {
	int iFail = 0;
	char *cError = NULL;
	if (exp != iFail)
	{
		EMH_ask_error_text(iFail, &cError);
		printf("The error is :%s\n", cError);
		exit(0);
	}
	return 0;
}


