#include <iostream>
#include <me/me.h>
#include <sa/user.h>
#include <sa/group.h>
#include <sa/role.h>
#include <sa/groupmember.h>
#include <tc/emh.h>
#include <tc/tc.h>
#include <tc/tc_startup.h>
#include <tccore/aom.h>
#include <tccore/aom_prop.h>
#include <base_utils/IFail.hxx>
#include <base_utils/TcResultStatus.hxx>
#include <mld/logging/TcMainLogger.hxx>

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
        
        stat = SA_init_module(); 
        
        tag_t  dba_tag = NULLTAG;
        stat = SA_find_group("dba", &dba_tag);
        
        tag_t infodba_tag = NULLTAG;
        stat = SA_find_user("infodba", &infodba_tag); 
        
        tag_t user_tag = NULLTAG;
        stat = SA_find_user("jdoe", &user_tag); 
        if (user_tag != NULLTAG)
        {
            stat = AOM_lock_for_delete(user_tag);
            stat = SA_delete_user(user_tag, infodba_tag, dba_tag, FALSE);
        }
        else cout << endl << "User jdoe not found!"  << endl;

        tag_t person_tag = NULLTAG;
        stat = SA_find_person("Doe, John", &person_tag);
        if (user_tag != NULLTAG)
        {        
            stat = AOM_lock_for_delete(person_tag);
            stat = AOM_delete(person_tag);
        }
        else cout << endl << "Person Doe, John not found!"  << endl;
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
