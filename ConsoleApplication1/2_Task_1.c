/* Write a ITK code to follow these steps: 

1.ITK_auto_login
2.find custom Item revision.
3.find list of secondary objects to IR.
4.In sec list if it's having PDF update object_name property value  to "ITK_Task"
5.ITK_exit_module.

*/

#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include<tc\emh.h>
#include<tccore\item.h>
#include<tc/tc_startup.h>
#include<tcinit\tcinit.h>
#include<tccore\grm.h>
#include<tccore\aom_prop.h>
#include<tccore\aom.h>
#include<ae\dataset.h>
#include<fclasses/tc_string.h>

int ITK_user_main(int argc, char* argv[])
{
	int iFail = 0;
	iFail = ITK_init_module("infodba", "infodba", "dba");
	if (iFail == ITK_ok)
	{
		printf("gani");
	}
	
	return iFail;
}