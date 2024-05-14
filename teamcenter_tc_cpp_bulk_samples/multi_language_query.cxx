
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <qry/qry.h>
#include <tc/emh.h>
#include <tc/tc.h>
#include <tc/tc_startup.h>

#include <metaframework/BusinessObjectRef.hxx>
#include <base_utils/IFail.hxx>
#include <base_utils/TcResultStatus.hxx>

using namespace std;
using namespace Teamcenter;

int ITK_user_main(int argc, char* argv[])
{
    int ifail = ITK_ok;
    ResultStatus stat;
    
    ifail = ITK_initialize_text_services(ITK_BATCH_TEXT_MODE);       
    ifail = ITK_auto_login();                                         
    ifail = ITK_set_journalling(TRUE);
    
    try
    {
        tag_t query = NULLTAG;
        stat = QRY_find2("Item...", &query );
        if (query != NULLTAG)
        {
           int n_ask_entries = 0;
           char** ask_entries = NULL;
           char** ask_values = NULL;
           stat = QRY_find_user_entries(query, &n_ask_entries, &ask_entries, &ask_values);
           
           int n_set_entries = 2;
           char** set_entries = NULL;
           set_entries  = (char **) MEM_alloc(n_set_entries * sizeof(char *));
           
           /* Position 1 = Item ID */
           printf("\t ask_entries[1]: %s \n", ask_entries[1]);
           set_entries[0] = (char *)MEM_alloc( strlen(ask_entries[1]) + 1);
           strcpy(set_entries[0], ask_entries[1] );
           
           /* Position 9 = Type */
           printf("\t ask_entries[9]: %s \n", ask_entries[9]);
           set_entries[1] = (char *)MEM_alloc( strlen(ask_entries[9]) + 1);
           strcpy(set_entries[1], ask_entries[9] );
           
           char *set_values[2] = {"105*", "Item"};
           int n_items = 0;
           tag_t* items = NULL;
           stat = QRY_execute(query, n_set_entries, set_entries, set_values, &n_items, &items);
           
           printf("\n\t n_items: %d \n", n_items);
           for (int ii = 0; ii < n_items; ii++) 
               printf("\t\t %u \n", items[ii]);
           
           MEM_free(items);
           MEM_free(ask_entries);
           MEM_free(ask_values);
           MEM_free(set_entries);  
        }
    }
    catch(IFail & ex)
    { 
        cout << "error " << ex.ifail() << endl;
        cout << ex.getMessage() << endl;
    }
    
    const char* syslog_path = EMH_ask_system_log();
    printf("\n\n%s\n", syslog_path);
    
    ifail = ITK_exit_module(FALSE);                                        
    return ifail;
}
