/*
Create  a Custom Action Handler which firstly sees if in a BOM, all the child components of an Item Revision are released, if not then it should release all
*/

#include<stdio.h>
#include<conio.h>
#include<tccore/custom.h>
#include<tccore/method.h>
#include<fclasses/tc_string.h>
#include<tccore/aom_prop.h>
#include<tc/emh.h>
#include<tccore/item.h>
#include<epm/epm.h>
#include<bom/bom.h>
#include<tccore/releasestatus.h>
#define OBJECT_TYPE "ItemRevision"

#define DLLAPI _declspec(dllexport)

METHOD_id_t tMethodId;
int iStatus = ITK_ok;
extern DLLAPI int DLL_PROJECT_register_callbacks();
extern DLLAPI int handler_execute_callbacks1(int *decision, va_list argv);
extern DLLAPI int releasing_Childs(EPM_action_message_t msg);
extern DLLAPI int handler_execute_callbacks2(int *decision, va_list argv);
int check(int exp);
int check_If_Released(tag_t Revision);

extern DLLAPI int DLL_PROJECT_register_callbacks()
{
	iStatus = CUSTOM_register_exit("DLL_PROJECT", "USER_init_module", (CUSTOM_EXIT_ftn_t)handler_execute_callbacks1);

	iStatus = CUSTOM_register_exit("DLL_PROJECT", "USER_exit_module", (CUSTOM_EXIT_ftn_t)handler_execute_callbacks2);
	return iStatus;
}


extern DLLAPI int handler_execute_callbacks1(int *decision, va_list argv)
{
	*decision = ALL_CUSTOMIZATIONS;
	printf("\n\n\n----welcome dll registration process---- \n\n\n");
	printf("\n\n\n----login sucess---- \n\n\n");
	printf("\n\n\n----Handler dll  registered---- \n\n\n");

	EPM_register_action_handler("Custom_Releasing_All_Childs_If_Not", "Custom_Releasing_All_Childs_If_Not", (EPM_action_handler_t)releasing_Childs);
	//Registers an action handler with the given handlerName.
	//1.Handler name
	//2.handler description
	//3.function pointer
	return iStatus;
}
extern DLLAPI int releasing_Childs(EPM_action_message_t msg)
{
	
	tag_t tRootTask = NULLTAG;
	int iCount = 0;
	tag_t *tAttachment = NULL;
	int *iAttachmentType = 0;
	int i,j;
	tag_t tClassId = NULLTAG;
	char *cClassName = NULL;
	tag_t tItem = NULLTAG;
	int iViewCount = 0;
	tag_t *tPBomViewTags = NULL;
	tag_t tWindow = NULLTAG;
	tag_t tPBomLineTag = NULLTAG;
	check(EPM_ask_root_task(msg.task, &tRootTask));
	{
		if (tRootTask != NULLTAG)
		{
			check(EPM_ask_all_attachments(tRootTask, &iCount, &tAttachment, &iAttachmentType));
			if (iCount != 0 && tAttachment != NULL && iAttachmentType != 0)
			{
				for (i = 0; i < iCount; i++)
				{

					check(POM_class_of_instance(tAttachment[i], &tClassId));
					if (tClassId != NULLTAG)
					{
						check(POM_name_of_class(tClassId, &cClassName));
						if (cClassName != NULL)
						{
							if (tc_strcmp(cClassName, OBJECT_TYPE) == 0)
							{
								check(ITEM_ask_item_of_rev(tAttachment[i], &tItem));
								if (tItem != NULLTAG)
								{
									check(ITEM_list_bom_views(tItem, &iViewCount, &tPBomViewTags));//Finding list of BOM views for an item
									if (tPBomViewTags != NULL)
									{
										for (j = 0; j < iViewCount; j++)
										{

											check(BOM_create_window(&tWindow));//creating BOM window

											check(BOM_set_window_top_line(tWindow, tItem, tAttachment[i], tPBomViewTags[j], &tPBomLineTag));//Setting top line
											if (tPBomLineTag != NULLTAG)
											{
												check_If_Released(tPBomLineTag);
											}
											else
											{
												printf("BOMline tag not found\n");
											}
										}
									}
									if (tPBomViewTags) MEM_free(tPBomViewTags);
								}
							}
						}
						if (cClassName) MEM_free(cClassName);

					}
				}
			}
			if (tAttachment) MEM_free(tAttachment);
			if (iAttachmentType) MEM_free(iAttachmentType);
		}
	}
	return 0;
}
extern DLLAPI int handler_execute_callbacks2(int *decision, va_list argv)
{
	*decision = ALL_CUSTOMIZATIONS;
	printf("\n\n\n----logout sucess---- \n\n\n");
	return iStatus;
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
int Func(tag_t tPBomLineTag)
{
	int count1 = 0;

	int i;

	tag_t *tChildren = NULL;
	tag_t tPBomLine = tPBomLineTag;

	check(BOM_line_ask_child_lines(tPBomLine, &count1, &tChildren));//Asking child lines
	if (tChildren != NULL)
	{
		for (i = 0; i < count1; i++)
		{
			tPBomLine = tChildren[i];//assigning tChildren[i] as topline
			//Func(tPBomLine);// calling function recursively to check childs
			check_If_Released(tPBomLine);
		}
	}
	if (tChildren) MEM_free(tChildren);
	return 0;
}
int check_If_Released(tag_t tPBomLineTag)
{
	int iIs_Released = 0;
	logical RetainReleasedDate = true;
	char *cStatus = "TCM Released";
	tag_t tReleaseStatus = NULLTAG;
	tag_t tRevision = NULLTAG;
	check(AOM_ask_value_tag(tPBomLineTag, "bl_revision", &tRevision));
	if (tRevision != NULLTAG)
	{
		check(EPM_ask_if_released(tRevision, &iIs_Released));
		if (iIs_Released == 1)
		{
			printf("child already released\n");
			Func(tPBomLineTag);
		}
		else
		{
			check(RELSTAT_create_release_status(cStatus, &tReleaseStatus));
			if (tReleaseStatus != NULLTAG)
			{
				check(RELSTAT_add_release_status(tReleaseStatus, 1, &tRevision, RetainReleasedDate));
				Func(tPBomLineTag);
			}
		}
	}
	
	return 0;
}