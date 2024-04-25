/*
-----TASK-----
----1.please explore TCTYPE APIs and create an item using it. After creation of an item, a property in the Item master form should be updated. -----

give the object type
Construct an item by giving the Tag of an item which we got in the above step
set the some basic properties to the item like name ,id
Use the API to create item(TCTYPE_create_object) based on the above input
After creating the business object save that business object
using item tag find the secondary objects of an item using GRM_list_secondary_objects_only
Find all the object type of secondary objects
compare if type is master form then update the value to a prperty.
*/

#include<conio.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<tc\emh.h>
#include<tc/tc_startup.h>
#include<tcinit\tcinit.h>
#include<tccore\aom.h>
#include<tccore\aom_prop.h>
#include<tccore\grm.h>
#include<tccore\tctype.h>
#include<tccore\item.h>

#define ITEM_ID "item_id"
#define OBJECT_NAME	"object_name"

int ITK_user_main(int argc, char* argv[])
{
	int iFail = 0;
	char *cError = NULL;
	tag_t tItem = NULLTAG;
	tag_t tCreateInput = NULLTAG;
	tag_t tBusinessObject = NULLTAG;

	int iCount = 0;
	int i;
	char *cObjectType = "Item Master";
	tag_t *tSecList = NULL;
	char *cValues = NULL;
	
	//ITK_ask_cli_argument();

	iFail = ITK_init_module("hello", "hello", "dba");
	if (iFail == ITK_ok)
	{
		printf("Login sucess\n");
		iFail = TCTYPE_ask_type("Item",&tItem);//Retrieves the type tag from the provided type name.
		if (iFail == ITK_ok&& tItem!=NULLTAG)
		{
			printf("Tag found\n");
			iFail = TCTYPE_construct_create_input(tItem, &tCreateInput);//Here we construct item
			if (iFail == ITK_ok&& tCreateInput!=NULLTAG)
			{
				printf("item construction done\n");
				iFail = AOM_set_value_string(tCreateInput, ITEM_ID, "001115"); //Here we are updating the item_id value
				if (iFail == ITK_ok)
				{
					printf("Item id set Sucess\n");
					iFail = AOM_set_value_string(tCreateInput, OBJECT_NAME, "i");//Here we are updating the OBJECT_NAME value
					if (iFail == ITK_ok)
					{
						iFail = TCTYPE_create_object(tCreateInput, &tBusinessObject);//Here we are creating the object based on the above input
						if (iFail == ITK_ok&& tBusinessObject!=NULLTAG)
						{
							printf("input created\n");
							AOM_save_without_extensions(tBusinessObject);//Saving the BUsiness object
							iFail = GRM_list_secondary_objects_only(tBusinessObject, NULLTAG, &iCount, &tSecList);//Finding the list of all secondary objects to the item we created i.e., ITEM MASTER FORM
							printf("%d", iCount);//Verifying the count 
							if (iFail == ITK_ok&& tSecList!=NULL)
							{
								for (i = 0; i < iCount; i++)
								{
									iFail = AOM_ask_value_string(tSecList[i], "object_type", &cValues);//Here we are finding the type of all the secondary objects to the item
									if (iFail == ITK_ok&& cValues!=NULL)
									{
										if (tc_strcmp(cValues, cObjectType)==0) //comparing the object type with Item Master
										{
											iFail = AOM_set_value_string(tSecList[i], "object_name", "MasterForm"); //here I am updating the master form name as "MasterForm"
											AOM_save_without_extensions(tSecList[i]);//After updating the name we have to save
											if (iFail == ITK_ok)
											{
												printf("A property value in MAster form is updated\n");
											}
											else                                                 //If the if condition fails else block will execute
											{

												EMH_ask_error_text(iFail, &cError);
												printf("The error is :%s\n", cError);
											}
										}
										else
										{

											EMH_ask_error_text(iFail, &cError);
											printf("The error is :%s\n", cError);
										}
									}
									else
									{

										EMH_ask_error_text(iFail, &cError);
										printf("The error is :%s\n", cError);
									}
								}
								MEM_free(cValues);
							}
							else
							{

								EMH_ask_error_text(iFail, &cError);
								printf("The error is :%s\n", cError);
							}
						}
						else
						{

							EMH_ask_error_text(iFail, &cError);
							printf("The error is :%s\n", cError);
						}
						MEM_free(tSecList);
					}
					else
					{

						EMH_ask_error_text(iFail, &cError);
						printf("The error is :%s\n", cError);
					}
				}
				else
				{

					EMH_ask_error_text(iFail, &cError);
					printf("The error is :%s\n", cError);
				}
			}
			else
			{

				EMH_ask_error_text(iFail, &cError);
				printf("The error is :%s\n", cError);
			}
		}
		else
		{

			EMH_ask_error_text(iFail, &cError);
			printf("The error is :%s\n", cError);
		}
	}
	else
		{

		EMH_ask_error_text(iFail, &cError);
		printf("The error is :%s\n", cError);
		}
	return iFail;
}