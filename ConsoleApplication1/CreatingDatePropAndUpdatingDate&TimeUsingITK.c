#include<conio.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<tc\emh.h>
#include<tc/tc_startup.h>
#include<tcinit\tcinit.h>
#include<tccore\aom_prop.h>
#include<tccore/aom.h>
#include<tccore\item.h>
#include<time.h>
#define Size 18
#define DATE_PROP "a4DateProp"
int check(exp)
{
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
int ITK_user_main(int argc, char* argv[])
{
	//time_t t;
	tag_t tItemTag = NULLTAG;
	/*char *cUser = ITK_ask_cli_argument("-u=");
	char *cPass = ITK_ask_cli_argument("-p=");
	char *cGroup = ITK_ask_cli_argument("-g=");
	const char *cItemId = ITK_ask_cli_argument("-itemid=");
	if (cUser != NULL && cPass != NULL && cGroup != NULL)
	{*/
		check(ITK_init_module("hello", "hello", "dba"));
		printf("Login successfull\n");
		char Date[Size];
		date_t tDate;
		time_t t;
		struct tm *tmp; //declares a pointer tmp to a structure tm.
		time(&t);// assigns the current calendar time to the variable t.
		//converts the calendar time represented by t to broken-down time representation stored in the structure tm pointed to by tmp.
		tmp = localtime(&t);
		//formats the broken-down time represented by tmp and stores the result in the character array Date.
		strftime(Date, sizeof(Date), "%d-%b-%Y %H:%M", tmp);
		printf("%s\n", Date);
		printf("size is :%d", sizeof(Date));
		/*check(ITEM_find_item(cItemId, &tItemTag));
		if (tItemTag != NULLTAG)
		{
			printf("Item Tag find\n");
			AOM_refresh(tItemTag, true);
			check(ITK_string_to_date(Date, &tDate));///tc/tc_startup.h>
			check(AOM_set_value_date(tItemTag, DATE_PROP, tDate));
			printf("Date value updated\n");
			check(AOM_save_without_extensions(tItemTag));
			printf("Saved\n");
			check(AOM_refresh(tItemTag, false));
		}*/
	//}
	return 0;
}