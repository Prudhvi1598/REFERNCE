// ---PREACTION----Before deleting revision the datsets attched to it shold be transfered to its previous revision
/*
Register callbacks
Add precondition callback
	First (ITEM_ask_item_of_rev) use this api-It will give the Item tag which the revision belongs to.
	After finding items tag list all revisions using items tag(ITEM_list_all_revs)
	List all secondary objects of item revision(GRM_list_secondary_objects_only)
	find relation type(GRM_find_relation_type)
	create relation with the previous item revision(GRM_create_relation)
	save the relation
*/



#include<stdio.h>
#include<conio.h>
#include<tccore/custom.h>
#include<tccore/method.h>
#include<tccore/aom_prop.h>
#include<fclasses/tc_string.h>
#include<tccore/grm.h>
#include<tccore/aom.h>
#include<tccore/item.h>
#include<pom/pom/pom.h>
#define DLLAPI _declspec(dllexport)

//#define PLM_error (EMH_USER_error_base +4)

//#define PLM_error1 (EMH_USER_error_base +4)

METHOD_id_t tMethodId;
int iStatus = 0;
extern DLLAPI int DLL_PROJECT_register_callbacks();
extern DLLAPI int PLM_execute_callbacks1(int *decision, va_list argv);
extern DLLAPI int PLM_add_pre_action(METHOD_message_t *msg, va_list argv);
extern DLLAPI int PLM_execute_callbacks2(int *decision, va_list argv);


extern DLLAPI int DLL_PROJECT_register_callbacks()
{
	iStatus = CUSTOM_register_exit("DLL_PROJECT", "USER_init_module", (CUSTOM_EXIT_ftn_t)PLM_execute_callbacks1);

	iStatus = CUSTOM_register_exit("DLL_PROJECT", "USER_exit_module", (CUSTOM_EXIT_ftn_t)PLM_execute_callbacks2);
	return iStatus;
}


extern DLLAPI int PLM_execute_callbacks1(int *decision, va_list argv)
{
	*decision = ALL_CUSTOMIZATIONS;
	printf("\n\n\n----welcome dll registration process---- \n\n\n");
	printf("\n\n\n----login sucess---- \n\n\n");

	iStatus = METHOD_find_method("ItemRevision", "IMAN_delete", &tMethodId);
	//ItemRevision- Type name
	//IMAN_delete- (tc_msg.h)
	//tMethodId- Returns an identifier for the method found
	iStatus = METHOD_add_action(tMethodId, METHOD_pre_action_type, (METHOD_function_t)PLM_add_pre_action, NULL);


	return iStatus;
}

extern DLLAPI int PLM_add_pre_action(METHOD_message_t *msg, va_list argv)
{

	int i, j;
	int k = 0;
	int iSecCount = 0, iCount = 0;
	char *cSourceItem = NULL;
	char *cSecObjectType = NULL;

	tag_t tSourceRev = NULLTAG;
	tSourceRev = va_arg(argv, tag_t);
	tag_t tSourceItem = NULLTAG;
	tag_t *tRevList = NULL;
	tag_t *tSecObjects = NULL;
	tag_t tRel2 = NULLTAG;
	tag_t tNewRelation = NULLTAG;
	tag_t tClassId = NULLTAG;
	char *cClassName = NULL;
	ITEM_ask_item_of_rev(tSourceRev, &tSourceItem);//This func will returm the tag of the item which revision belongs to.
	if (tSourceItem != NULLTAG)
	{
		if (ITEM_list_all_revs(tSourceItem, &iCount, &tRevList) == iStatus)//Retrieves all revisions based on item tag.
		{
			if (tRevList != NULL)
			{
				for (i = 0; i < iCount; i++)
				{
					if (tSourceRev == tRevList[i])
					{
						if (GRM_list_secondary_objects_only(tRevList[i], NULLTAG, &iSecCount, &tSecObjects) == iStatus)//Retrieves alll secondary objects
						{
							if (tSecObjects != NULL)
							{
								for (j = 0; j < iSecCount; j++)
								{
									POM_class_of_instance(tSecObjects[j], &tClassId);
									if (tClassId != NULLTAG)
									{
										POM_name_of_class(tClassId, &cClassName);
										if (cClassName != NULL)
										{
											printf("Sec object type found\n");
											k = tc_strcmp(cClassName, "Dataset");//compares two strings
											if (k == 0)
											{
												GRM_find_relation_type("IMAN_reference", &tRel2);//Retrieves the relation type for the given relation type name.
												if (tRel2 != NULLTAG)
												{
													printf("Relation type tag found\n");
													GRM_create_relation(tRevList[i - 1], tSecObjects[j], tRel2, NULLTAG, &tNewRelation);//Creates a relation of the specified type.
													if (tNewRelation != NULLTAG)
													{
														printf("Relation created\n");
														GRM_save_relation(tNewRelation);//Saves a relation.
													}

												}
											}
										}
									}
									
								}
							}
						}
					}
				}
			}
		}
		MEM_free(tRevList);
		MEM_free(tSecObjects);
		MEM_free(cSecObjectType);
	}
	return iStatus;
}
extern DLLAPI int PLM_execute_callbacks2(int *decision, va_list argv)
{
	*decision = ALL_CUSTOMIZATIONS;
	printf("\n\n\n----logout sucess---- \n\n\n");
	return iStatus;
}