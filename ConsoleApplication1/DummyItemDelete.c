#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<tc\emh.h>
#include<tccore\item.h>
#include<tccore\aom_prop.h>
#include<tccore\aom.h>
#include<tcinit\tcinit.h>
#include<tc/tc_startup.h>
#include<tccore/grm.h>
#include<ps/ps.h>
#include<tccore/workspaceobject.h>
#include<fclasses/tc_string.h>
#include<tc/folder.h>
#include<bom/bom.h>
#define OBJECT_TYPE "Fnd0HomeFolder"
int deletingSecondaryObjectsForRevisions(tag_t ItemTag, tag_t Regerencer, char *cRelation);
int deletingFromBom(tag_t tBOMviewRevisionTag, tag_t ItemTag);
int whereReferenced(tag_t ItemTag);
int check(int exp);
int ITK_user_main(int argc, char* argv[])
{
	tag_t tItemTag = NULLTAG;
	int iRevCount = 0;
	tag_t *tRevList = NULL;
	char *cRevId = NULL;
	char *cUser = ITK_ask_cli_argument("-u=");
	char *cPass = ITK_ask_cli_argument("-p=");
	char *cGroup = ITK_ask_cli_argument("-g=");
	char *cItem = ITK_ask_cli_argument("-itemid=");
	if (cUser != NULL && cPass != NULL && cGroup != NULL)
	{
		check(ITK_init_module(cUser, cPass, cGroup));
		printf("Login Sucessfull\n");
		check(ITEM_find_item(cItem, &tItemTag));
		if (tItemTag != NULLTAG)
		{
			printf("Item tag found\n");
			check(ITEM_list_all_revs(tItemTag, &iRevCount, &tRevList));
			if (tRevList != NULL && iRevCount != 0)
			{
				printf("revisions tags found\n");
				for (int i = 0; i < iRevCount; i++)
				{
					check(AOM_ask_value_string(tRevList[i], "item_revision_id", &cRevId));
					printf("the revision Id is = %s\n", cRevId);
					whereReferenced(tRevList[i]);


				}
			}
			whereReferenced(tItemTag);
			check(ITK_set_bypass(true));
			check(ITEM_delete_item(tItemTag));
			check(ITK_set_bypass(false));
			printf("Item deleted\n");

		}
	}
	check(ITK_exit_module(true));
	return 0;

}

int whereReferenced(tag_t ItemTag)
{
	const int iDepth = 1;
	int iReferencersCount = 0;
	int *iLevels;
	tag_t *tReferencers = NULL;
	char **cRelations = NULL;
	tag_t tRelationType = NULLTAG;
	tag_t tRelation = NULLTAG;
	char *cObjectType = NULL;
	char *cReferencerName = NULL;
	check(WSOM_where_referenced2(ItemTag, iDepth, &iReferencersCount, &iLevels, &tReferencers, &cRelations));
	if (iReferencersCount != 0 && tReferencers != NULL && cRelations != NULL)
	{
		for (int i = 0; i < iReferencersCount; i++)
		{
			check(AOM_ask_value_string(tReferencers[i], "object_string", &cReferencerName));
			printf("\"%s\" is referenced with relation %s\n", cReferencerName, cRelations[i]);
			check(WSOM_ask_object_type2(tReferencers[i], &cObjectType));
			if (tc_strcmp(cObjectType, OBJECT_TYPE) == 0 || tc_strcmp(cObjectType, "Folder") == 0 || tc_strcmp(cObjectType, "Newstuff Folder") == 0 || tc_strcmp(cObjectType, "Mail Folder") == 0)
			{
				check(FL_remove(tReferencers[i], ItemTag));
				printf("Item Removed from folder\n");
				check(AOM_save_without_extensions(tReferencers[i]));
				printf("Folder saved\n");

			}
			else if (tc_strcmp(cObjectType, "BOMView Revision") == 0)
			{
				//printf("\n%d\n", __LINE__);
				printf("---------Loop entered to delete  from BOM---------\n");
				deletingFromBom(tReferencers[i], ItemTag);
			}
			else if (tc_strcmp(cRelations[i], "IMAN_based_on") == 0 || tc_strcmp(cRelations[i], "") == 0)
			{
				//printf("\n%d\n", __LINE__);
				printf("Excluded Iman based on and empty relation\n");
				continue;
			}
			else
			{
				printf("---------Loop entered to delete secondary objects---------\n");
				deletingSecondaryObjectsForRevisions(ItemTag, tReferencers[i], cRelations[i]);
				//printf("\n%d\n", __LINE__);
				printf("Secondary object deleted\n");


			}
		}
	}
	return 0;
}

int deletingFromBom(tag_t tBOMviewRevisionTag, tag_t ItemTag)
{
	tag_t tWindow = NULLTAG;
	tag_t tTopline = NULLTAG;
	int iCount = 0;
	tag_t *tChildren = NULL;
	char *cItemValue = NULL;
	char *cToplineValue = NULL;
	check(BOM_create_window(&tWindow));
	if (tWindow != NULLTAG)
	{
		printf("BOM window created\n");
		check(BOM_set_window_top_line_bvr(tWindow, tBOMviewRevisionTag, &tTopline));
		if (tTopline != NULLTAG)
		{
			printf("Top line set\n");
			check(AOM_ask_value_string(ItemTag, "item_id", &cItemValue));
			check(BOM_line_ask_child_lines(tTopline, &iCount, &tChildren));
			if (tChildren != NULL && iCount != 0)
			{
				printf("Child lines found\n");
				for (int i = 0; i < iCount; i++)
				{
					check(AOM_ask_value_string(tChildren[i], "bl_item_item_id", &cToplineValue));
					if (tc_strcmp(cItemValue, cToplineValue) == 0)
					{
						printf("Item id matched with BOM line item id\n");
						ITK_set_bypass(true);
						check(BOM_line_cut(tChildren[i]));
						ITK_set_bypass(false);
						BOM_save_window(tWindow);
						BOM_close_window(tWindow);
						printf("bomline removed\n");
					}
				}
			}
		}
	}
	printf("\n%d\n", __LINE__);

	return 0;
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


int deletingSecondaryObjectsForRevisions(tag_t ItemTag, tag_t Regerencer, char *cRelation)
{
	tag_t tRelationType = NULLTAG;
	tag_t tRelation = NULLTAG;

	check(GRM_find_relation_type(cRelation, &tRelationType));
	if (tRelationType != NULLTAG)
	{
		if (GRM_find_relation(ItemTag, Regerencer, tRelationType, &tRelation) != 0)
		{
			printf("relation found in if\n");
			if (tRelation != NULLTAG)
			{
				check(ITK_set_bypass(TRUE));
				check(GRM_delete_relation(tRelation));
				check(ITK_set_bypass(FALSE));
			}
		}
		else
		{
			printf("relation found in else\n");
			check(GRM_find_relation(Regerencer, ItemTag, tRelationType, &tRelation));
			if (tRelation != NULLTAG)
			{
				check(ITK_set_bypass(TRUE));
				check(GRM_delete_relation(tRelation));
				check(ITK_set_bypass(FALSE));
			}
		}
		printf("\n%d\n", __LINE__);
	}


	return 0;
}
/*int deletingBomViews()
{

}*/
