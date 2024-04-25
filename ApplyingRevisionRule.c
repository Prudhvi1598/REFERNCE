/*
Write a Custom Action Handler to get Item Revision from Target which is an Assembly and get the related BOMView Revision. 
Get Child lines from BOMView Revision by expanding with “Latest Working” Revision Rule and write below information into a .csv file. 
The location of .csv file to be created should be stored in a preference.


	Registering custom action handler
	After initiating the workflow that action handler is kept we will gat a root task
	with that we can ask attachments
	After that pom class of instance
	After that we can find pom name of class




*/
#include<stdio.h>
#include<conio.h>
#include<tccore/custom.h>
#include<tccore/method.h>
#include<fclasses/tc_string.h>
#include<pom/pom/pom.h>
#include<epm/epm.h>
#include<tc/emh.h>
#include<cfm/cfm.h>
#include<bom/bom.h>
#include<tc\preferences.h>
#include<tccore/aom_prop.h>
#include<tccore/aom.h>
#define DLLAPI _declspec(dllexport)
#define plm_error (EMH_USER_error_base +4)
METHOD_id_t tMethodId;
int iStatus = ITK_ok;
extern DLLAPI int DLL_PROJECT_register_callbacks();
extern DLLAPI int handler_execute_callbacks1(int *decision, va_list argv);
extern DLLAPI int applying_revision_rule(EPM_action_message_t msg);
extern DLLAPI int handler_execute_callbacks2(int *decision, va_list argv);
extern DLLAPI int check(int exp);


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

	EPM_register_action_handler("Applying_Revision_Rule", "Applying Revision Rule", (EPM_action_handler_t)applying_revision_rule);
	//Registers an action handler with the given handlerName.
	//1.Handler name
	//2.handler description
	//3.function pointer
	return iStatus;
}
extern DLLAPI int applying_revision_rule(EPM_action_message_t msg)
{
	FILE *fp;
	tag_t tRootTask = NULLTAG;
	tag_t *tAttachments = NULL;
	int iCount = 0;
	tag_t tClassId = NULLTAG;
	char *cClassName = NULL;
	tag_t tItem = NULLTAG;
	int iBomViewCount = 0;
	tag_t *tBomViews = NULL;
	tag_t tWindow = NULLTAG;
	tag_t tTopBomLine = NULLTAG;
	tag_t tRule = NULLTAG;
	int iBomLineCount = 0;
	tag_t *tBomChilds = NULL;
	int iValue1 = 0;
	char *cValue2 = NULL;
	char *cValue3 = NULL;
	char *cValue4 = NULL;
	char *cValue5 = NULL;
	char *cPreference = "Custom_Applying_RevRule";
	char *cPrefValue = NULL;
	check(EPM_ask_root_task(msg.task, &tRootTask));
	if (tRootTask != NULLTAG)
	{
		check(EPM_ask_attachments(tRootTask, EPM_target_attachment, &iCount, &tAttachments));
		if (iCount > 0 && tAttachments != NULL)
		{
			printf("Attachments found\n");
			for (int i = 0; i < iCount; i++)
			{
				check(POM_class_of_instance(tAttachments[i], &tClassId));//Finds out the class which "tAttachments[i]" belongs to
				if (tClassId != NULLTAG)
				{
					printf("class id tag found\n");
					check(POM_name_of_class(tClassId, &cClassName));//finds the name of the class

					if (cClassName != NULL)
					{
						printf("class name found\n");
						//printf("class name=%s\n", cClassName);
						if (tc_strcmp(cClassName, "ItemRevision") == ITK_ok)
						{
							check(ITEM_ask_item_of_rev(tAttachments[i], &tItem));
							if (tItem != NULLTAG)
							{
								printf("item tag found found\n");
								check(ITEM_list_bom_views(tItem, &iBomViewCount, &tBomViews));
								if (iBomViewCount != 0 && tBomViews != NULL)
								{
									printf("bvrs listed\n");
									for (int j = 0; j < iBomViewCount; j++)
									{
										check(BOM_create_window(&tWindow));
										if (tWindow != NULLTAG)
										{
											printf("Bom window created\n");

											check(CFM_find("Latest Working", &tRule));
											if (tRule != NULLTAG)
											{
												check(BOM_set_window_top_line(tWindow, tItem, tAttachments[i], tBomViews[j], &tTopBomLine));
												if (tTopBomLine != NULLTAG)
												{
													check(BOM_save_window(tWindow));
													check(BOM_set_window_config_rule(tWindow, tRule));
													check(BOM_save_window(tWindow));
													printf("top line set\n");
													check(BOM_line_ask_all_child_lines(tTopBomLine, &iBomLineCount, &tBomChilds));
													if (iBomLineCount != 0 && tBomChilds != NULL)
													{
														printf("child lines found\n");
														check(PREF_ask_char_value(cPreference, 0, &cPrefValue));
														if (cPrefValue != NULL)
														{
															printf("preference value found\n");
															fp = fopen(cPrefValue, "w+");
															for (int k = 0; k < iBomLineCount; k++)
															{
																//Level| Child Item ID| Child Revision ID| Quantity| UOM
																//AOM_UIF_ask_value
																check(AOM_ask_value_int(tBomChilds[k], "bl_level_starting_0", &iValue1));
																printf("%d \n", iValue1);


																check(AOM_ask_value_string(tBomChilds[k], "bl_item_item_id", &cValue2));
																printf("%s \n", cValue2);


																check(AOM_ask_value_string(tBomChilds[k], "bl_rev_item_revision_id", &cValue3));
																printf("%s \n", cValue3);


																check(AOM_UIF_ask_value(tBomChilds[k], "bl_quantity", &cValue4));
																printf("%s \n", cValue4);


																check(AOM_UIF_ask_value(tBomChilds[k], "bl_uom", &cValue5));
																printf("%s \n", cValue5);
																fprintf(fp, "%d|%s|%s|%s|%s\n", iValue1, cValue2, cValue3, cValue4, cValue5);

															}
															if (cValue2) { MEM_free(cValue2); }
															if (cValue3) { MEM_free(cValue3); }
															if (cValue4) { MEM_free(cValue4); }
															if (cValue5) { MEM_free(cValue5); }
															fclose(fp);
														}
													}
												}
											}
										}
									}
									if(cPrefValue) { MEM_free(cPrefValue); }
									if(tBomChilds) { MEM_free(tBomChilds); }
								}
							}
							if(tBomViews) { MEM_free(tBomViews); }
						}
					}
				}
				if(cClassName) { MEM_free(cClassName); }
			}
		}
	}
	if (tAttachments) { MEM_free(tAttachments); }
	return 0;
}


extern DLLAPI int handler_execute_callbacks2(int *decision, va_list argv)
{
	*decision = ALL_CUSTOMIZATIONS;
	printf("\n\n\n----logout sucess---- \n\n\n");
	return iStatus;
}

extern DLLAPI int check(int exp) {
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