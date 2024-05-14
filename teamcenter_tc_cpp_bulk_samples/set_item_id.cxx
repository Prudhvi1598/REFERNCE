

#include <tc/tc.h>
#include <tc/tc_startup.h>
#include <tccore/aom.h>
#include <tccore/item.h>

#include <iostream>
#include <base_utils/TcResultStatus.hxx>
#include <base_utils/IFail.hxx>

using namespace std;

int ITK_user_main(int argc, char* argv[])
{
    int ifail = ITK_ok;
    ResultStatus status;
    try
    {
        status = ITK_initialize_text_services(ITK_BATCH_TEXT_MODE);         
        status = ITK_auto_login();                                          
        status = ITK_set_journalling(TRUE);
        
        tag_t item_tag = NULLTAG;
        status = ITEM_find_item("9126039", &item_tag);
        if (item_tag != NULLTAG)
        {
            status = AOM_refresh(item_tag, true);
            status = ITEM_set_id(item_tag, "9126039-New");
            status = ITEM_save_item(item_tag);
            status = AOM_refresh(item_tag, true);
            status = AOM_unload(item_tag);
        }
		else cout << "\n   Item NOT found!"  << endl;
    }
    catch( const IFail &e )
    {       
        printf("\n\n\n");
        cout << "error " << e.ifail() << endl;
        cout << e.getMessage() << endl; 
    } 

    status = ITK_exit_module(FALSE);                                        
    return ITK_ok;
}
