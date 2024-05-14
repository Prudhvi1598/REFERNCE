
#include <tc/emh.h>
#include <tc/tc.h>
#include <tc/tc_startup.h>
#include <tccore/aom.h>
#include <tccore/item.h>

#include <iostream>
#include <vector>
#include <base_utils/IFail.hxx>
#include <base_utils/ScopedSmPtr.hxx>
#include <base_utils/TcResultStatus.hxx>
#include <metaframework/BusinessObjectRegistry.hxx>
#include <metaframework/ReviseInput.hxx>
#include <mld/logging/TcMainLogger.hxx>
#include <tccore/ItemRevision.hxx>
#include <metaframework/BusinessObjectRef.hxx>
#include <metaframework/BusinessObjectRegistry.hxx>
#include <metaframework/ReviseInput.hxx>

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
        
        char item_id[ITEM_id_size_c +1] = "0000000";
        char *i = ITK_ask_cli_argument("-i=");
        
        if (i != NULL) strcpy(item_id, i);
        cout << endl << "    ID: " << item_id << endl << endl;
        
        tag_t tItem = NULLTAG;
        stat = ITEM_find_item(item_id, &tItem);

        tag_t tLatestRev = NULLTAG;
        stat = ITEM_ask_latest_rev(tItem, &tLatestRev); 

        char *pszType = NULL;
        stat = WSOM_ask_object_type2(tLatestRev, &pszType);

        BusinessObjectRegistry& boReg = BusinessObjectRegistry::instance();
        
        string revOp = "Revise";
        ReviseInput* pReviseInput = 
            dynamic_cast<ReviseInput*>(boReg.createInputObject(pszType, revOp));

        BusinessObjectRef<ItemRevision > boLatestRev(tLatestRev);
        
        vector<Teamcenter::DeepCopyData* > deepCopyData;
        boLatestRev->getDeepCopyData(&revOp, &deepCopyData);
        
        pReviseInput->setLogical("performDeepCopy", true, false );
        
        tag_t tRev = NULLTAG;
        boLatestRev->revise(pReviseInput, &deepCopyData, &tRev);

        stat = AOM_save_with_extensions(tRev);
        
        char *pszId = NULL;
        stat = WSOM_ask_object_id_string(tRev, &pszId);
        cout << endl << "    Object String: " << pszId << endl << endl;
        
        if (pszType) MEM_free(pszType);
        if (pszId) MEM_free(pszId); 
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
