#include "defaultHeaderFiles.h"
#include<iostream>
#include<vector>
#include<fstream>
#include<string>
#include<sstream>
#include<algorithm>
#include<iterator>
#include<tccore/item.h>
using namespace std;
int ITK_user_main(int argc, char* agrv[]) {
	vector<string> v1;
	char* cUser = NULL;
	char* cPassword = NULL;
	char* cGroup = NULL;
	const char* cFileName = NULL;
	string Line;
	tag_t tItem = NULLTAG;
	//string token;
	cUser = ITK_ask_cli_argument("-u=");//Provide Userid
	cPassword = ITK_ask_cli_argument("-p=");//provide password
	cGroup = ITK_ask_cli_argument("-g=");// provide group
	cFileName = ITK_ask_cli_argument("-file=");
	ITK(LOGIN);
	if (cUser != NULL && cPassword != NULL && cGroup != NULL) {
		cout << "Login successful" << endl;
		fstream fp;
		fp.open(cFileName);
		if (fp.is_open()) {
			while (getline(fp, Line))//get lines from the file
			{

				Line.erase(remove(Line.begin(), Line.end(), '"'), Line.end());//Removing all the Double quotes from the Line from beginning to ending 
				v1.push_back(Line);//Storing in the vector
			}
			cout << "-------------------------------------------" << endl;
			fp.close();
			for (int i = 0; i < v1.size(); i++) {
				cout << v1[i] << endl;
				size_t PosOfUnderscore = v1[i].find("_");
				//Get index of Underscores for each line in the file
				if (PosOfUnderscore != -1 && isdigit((v1[i])[PosOfUnderscore + 1])) {//Getting the string prefix underscore
					size_t PosOfComma = v1[i].find(",");
					//Finding comma position
					v1[i].erase(PosOfComma + 1, PosOfUnderscore - PosOfComma);
					//removing string and underscore
				}
				else if (PosOfUnderscore != -1 &&isalpha((v1[i])[PosOfUnderscore + 1])) {//Getting the string suffix underscore
					size_t PosOfComma = v1[i].find(",", PosOfUnderscore);
					//finding position of comma (after underscore)
					v1[i].erase(PosOfUnderscore, PosOfComma - PosOfUnderscore);
					//removing string and underscore
				}
			}
			cout << "\n--------------------CLeaned file-------------------" << endl;
			cout << v1[0] << endl;
			//Printing only first line
			for (int x = 1; x < v1.size(); x++) {
				cout << v1[x] << endl;
				size_t PosOfComma1 = v1[x].find(",");
				//Getting first comma position after cleaning
				size_t PosOfComma2 = v1[x].find(",", PosOfComma1 + 1);
				//Getting second comma position
				string Store = v1[x].substr(PosOfComma1 + 1, PosOfComma2 - PosOfComma1 - 1);// Copying of string starts from pos and is done till pos+len means [pos, pos+len).
				//Now  getting the value in between the first comma and second comma
				cout << Store << endl;
				ITK(ITEM_find_item(Store.c_str(), &tItem));
				//Using c_str() as the iTEM iD is stored in the form of Char
				if (tItem != NULLTAG) {
					cout << "Item id " << Store << " is present" << endl;
				}
				else {
					cout << "Item id " << Store << " is not present" << endl;
				}
			}
		}
		else {
			cout << "\n couldnot open File";
		}
	}
	return 0;
	ITK(ITK_exit_module(true));
}
