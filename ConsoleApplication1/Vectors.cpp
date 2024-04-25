#include<iostream>
#include<tc/tc_startup.h>
#include<tcinit\tcinit.h>
#include<tc/emh.h>
#include<tccore/item.h>
#include<tc/tc_macros.h>
#include<fstream>
#include<vector>
#include<string.h>
#include<string>
#include<sstream>
#include<algorithm>
using namespace std;

int ITK_user_main(int argc, char* argv[])
{
	fstream file;
	vector<string> v1;
	vector<string> v2;
	string token;
	tag_t tItemTag = NULLTAG;
	char *cUser = ITK_ask_cli_argument("-u=");
	char *cPass = ITK_ask_cli_argument("-p=");
	char *cGroup = ITK_ask_cli_argument("-g=");
	if (cUser != NULL && cPass != NULL && cGroup != NULL)
	{
		ITKCALL(ITK_init_module(cUser, cPass, cGroup));
		cout << "Login sucessful\n" << endl;
		file.open("C:\\Users\\T50315\\Downloads\\test.txt");
		if (file.is_open())
		{
			string str;
			while (getline(file, str))
			{
				
				str.erase(remove(str.begin(), str.end(), '"'), str.end());       //removing double quotes
				str.erase(remove(str.begin(), str.end(), ' '), str.end());       //removing extra spaces
				v1.push_back(str);                                               //string storing into the vector
			}
			file.close();
		}
		else
		{
			cout << "File not open" << endl;
		}

		cout << "-------Before cleaning the file-------" << endl;
		for (int i = 0; i < v1.size(); i++)
		{
			cout << v1[i] << endl;
			size_t underScorePos = v1[i].find('_');                                 //finding underscore position
			if (underScorePos != string::npos&&isdigit((v1[i])[underScorePos + 1])) //checking the character after underscore is digit
			{
				size_t commaPos = v1[i].find(",");                                  //finding comma position
				v1[i].erase(commaPos+1, underScorePos- commaPos);                   //Earasing the characters in between comma and underscore
				
			}
			else if(underScorePos!= string::npos&&isalpha((v1[i])[underScorePos + 1]))//checking the character after underscore is alphabet
			{
				size_t commaPos = v1[i].find(",", underScorePos);                     //finding comma position after underscore
				
				v1[i].erase(underScorePos, commaPos- underScorePos);                   //earasing characters includind underscore
				
			}
		}
		cout << "-------After cleaning the file-------" << endl;
		cout << v1[0] << endl;                                                         //printing only heading part 
		for (int j = 1; j < v1.size(); j++)
		{
			cout << v1[j] << endl;                                                     //After cleaning printing the lines line by line
			size_t commaPos1 = v1[j].find(",");                                        //finding first comma position
			size_t commaPos2 = v1[j].find(",", commaPos1+1);                           //finding next comma position
			string str1 = v1[j].substr(commaPos1+1, commaPos2- commaPos1-1);           //retrieving the sub string that is item id.
		
			ITKCALL(ITEM_find_item(str1.c_str(), &tItemTag));                           //checking in the database whether it is present in database or not
			if (tItemTag!=NULLTAG)
			{
				cout <<"Item "<< str1 <<"  is in Tc" << endl;
			}
			else
			{
				cout << "Item " << str1 << " not in Tc" << endl;
			}
		}
	}
	return 0;
	ITK_exit_module(true);
}
