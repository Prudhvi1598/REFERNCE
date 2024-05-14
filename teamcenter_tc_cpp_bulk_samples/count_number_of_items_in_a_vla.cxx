/*
This C++ example demonstrates how to count the number of items/objects with a VLA property.

Folder object is used with property, 'contents' to count the number of objects within a folder.
The Folder.object_name with Folder.owning_user properties are used to limit which Folder object is used in the counting.

Macro ITK_ERR is personal error checking mechanism to test for ITK errors. Most users have their own error checking impementation. 

Author: Patrick Hoonhout
Date: 07/26/2023
*/

void countFolderContent()
{
	int rows = 0;
	int cols = 0;
	char* user_name{ "gtac" };
	tag_t user_tag = NULLTAG;
	void*** ppReport = nullptr;
	const char* pQry1 = "Query";
	std::array select_attr { "countDistExpr" };

	try
	{
		ITK_ERR(POM_get_user(&user_name, &user_tag));

		ITK_ERR(POM_enquiry_create(pQry1));
		ITK_ERR(POM_enquiry_set_attr_expr(pQry1, select_attr[0], "Folder", "contents", POM_enquiry_countdist, ""));
		ITK_ERR(POM_enquiry_add_select_exprs(pQry1, 1, &select_attr.front()));

		ITK_ERR(POM_enquiry_set_string_expr(pQry1, "str_expr", "Folder", "object_name", POM_enquiry_equal, "Home"));
		ITK_ERR(POM_enquiry_set_tag_expr(pQry1, "tag_expr", "Folder", "owning_user", POM_enquiry_equal, user_tag));
		ITK_ERR(POM_enquiry_set_expr(pQry1, "where_expr", "str_expr", POM_enquiry_and, "tag_expr"));
		ITK_ERR(POM_enquiry_set_where_expr(pQry1, "where_expr"));

		ITK_ERR(POM_enquiry_execute(pQry1, &rows, &cols, &ppReport));
		ITK_ERR(POM_enquiry_delete(pQry1));

		if (ppReport != nullptr && rows == 1 && cols == 1)
		{
			void** count = ppReport[0];

			if(count != nullptr)
				printf("Total Count: %d\n", *(int*)count[0]);
		}
		else
			printf("Proplem performing count!\n");

		MEM_free(ppReport);
	}
	catch (const std::exception& err)
	{
		std::cout << "Error: " << err.what() << std::endl;
	}
}