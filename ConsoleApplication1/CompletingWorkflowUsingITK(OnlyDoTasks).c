/*
Login to Teamcenter with supplied username and password.
Usage will be something like below
ConsoleApplication2 [-u] [-p] [-g] [-item_id]
Get all assigned tasks.
Complete task which has target item having supplied item_id


*/
#include<tcinit\tcinit.h>
#include<tccore/item.h>
#include<tc/tc_macros.h>
#include<tccore/aom_prop.h>
#include<epm/epm.h>
#include<sa/sa.h>
#include<stdio.h>
int CompletingTask(char *cItemId, char *cUser);
int ITK_user_main(int argc, char* argv[])
{
	int flag = 0;
	tag_t tItemTag = NULLTAG;
	tag_t tRevTag = NULLTAG;
	tag_t tType = NULLTAG;
	
	char *cValue = NULL;
	
	char* cName= NULL;
	char *cUser = ITK_ask_cli_argument("-u=");
	char *cPass = ITK_ask_cli_argument("-p=");
	char *cGroup = ITK_ask_cli_argument("-g=");
	char *cItemId = ITK_ask_cli_argument("-itemid=");
	
	if (cUser != NULL && cPass != NULL && cGroup != NULL)
	{
		ITKCALL(ITK_init_module(cUser, cPass, cGroup));
		printf("logged in\n");
		while (true)
		{
			flag = CompletingTask(cItemId,cUser);
			if (flag == 0)
			{
				break;
			}
		}
	}
	return 0;
}

int CompletingTask(char *cItemId, char *cUser)
{
	int iAttCount = 0;
	tag_t tRootTask = NULLTAG;
	tag_t *tAttachments = NULL;
	char *cValue = NULL;
	int decision = 0;
	tag_t tUser = NULLTAG;
	int iCount = 0;
	tag_t *tAssignedTasks = NULL;

	ITKCALL(SA_find_user2(cUser, &tUser));
	if (tUser != NULLTAG)
	{
		printf("User tag found\n");
		ITKCALL(EPM_ask_assigned_tasks(tUser, 2, &iCount, &tAssignedTasks));
		if (tAssignedTasks != NULL)
		{
			printf("Assigned tasks found\n");
			for (int i = 1; i < iCount; i = i + 2)
			{
				ITKCALL(EPM_ask_root_task(tAssignedTasks[i], &tRootTask));
				if (tRootTask != NULLTAG)
				{
					ITKCALL(EPM_ask_attachments(tRootTask, EPM_target_attachment, &iAttCount, &tAttachments));
					if (iAttCount != 0 && tAttachments != NULL)
					{
						for (int j = 0; j < iAttCount; j++)
						{
							ITKCALL(AOM_ask_value_string(tAttachments[j], "item_id", &cValue));
							if (cValue != NULL)
							{
								if (tc_strcmp(cItemId, cValue) == 0)
								{
									decision = 1;
									ITKCALL(EPM_set_task_result(tAssignedTasks[i], EPM_RESULT_Completed));
									printf("Task completed set\n");
									ITKCALL(EPM_trigger_action(tAssignedTasks[i], EPM_complete_action, "c123355"));
									printf("Action triggered\n");

								}
							}
						}
						if (cValue)MEM_free(cValue);
					}
					if (tAttachments)MEM_free(tAttachments);
				}
			}
		}
		if (tAssignedTasks)MEM_free(tAssignedTasks);
	}

	return decision;
}