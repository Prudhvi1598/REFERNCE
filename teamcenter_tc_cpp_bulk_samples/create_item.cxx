
#include <tc/emh.h>
#include <tc/tc.h>
#include <tc/tc_startup.h>
#include <tccore/aom.h>

#include <iostream>
#include <base_utils/IFail.hxx>
#include <base_utils/TcResultStatus.hxx>
#include <metaframework/BusinessObjectRegistry.hxx>
#include <metaframework/CreateInput.hxx>
#include <mld/logging/TcMainLogger.hxx>
#include <tccore/Item.hxx>

using namespace std;
using namespace Teamcenter;
using Teamcenter::Main::logger;

int ITK_user_main(int argc, char* argv[])
{
    int ifail = ITK_ok;
    ResultStatus stat;
    try
    {
        stat = ITK_initialize_text_services(ITK_BATCH_TEXT_MODE);         
        stat = ITK_auto_login();                                          
        stat = ITK_set_journalling(TRUE);


        BusinessObjectRegistry& boReg = BusinessObjectRegistry::instance();
        CreateInput* pCreateInput = 
            dynamic_cast<CreateInput*>(boReg.createInputObject("Item", "Create"));

        Item *pItem = dynamic_cast<Item *>(boReg.createBusinessObject(pCreateInput));

        tag_t tItem = pItem->getTag();

        string sId;
        bool isNull = false;
        pItem->getItem_id(sId, isNull);
        
        pItem->setObject_name(sId, isNull);
        
        stat = AOM_save_with_extensions(pItem->getTag());

        cout << endl << "    ID: " << sId << endl << endl;
    }
    catch( const IFail &ex )
    {
        logger()->error( ex.ifail(), ex.getMessage());
        cout << ex.getMessage()  << endl;
        cout << "error " << ex.ifail() << endl;        
    }
    const char* syslog_path;
    syslog_path = EMH_ask_system_log();
    cout << endl << endl;
    cout << syslog_path << endl;

    stat = ITK_exit_module(FALSE);                                        
    return ITK_ok;
}
