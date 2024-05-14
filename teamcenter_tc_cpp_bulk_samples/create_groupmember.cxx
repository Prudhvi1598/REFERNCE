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
#include <base_utils/ScopedSmPtr.hxx>
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

        tag_t person = NULLTAG;
        stat = SA_create_person("Doe, John", &person);  
        stat = AOM_save(person);
        
        tag_t user = NULLTAG;
        stat = SA_create_user2("jdoe", "Doe, John", "password", &user); 

        tag_t  group = NULLTAG;
        stat = SA_find_group("Engineering", &group); 
        if (group == NULLTAG)
        {
            cout << endl << "Group Engineering not found!"  << endl;
        }
        else
        {
            tag_t  role = NULLTAG;
            stat = SA_find_role("Designer", &role); 
            if (role == NULLTAG)
            {
                cout << endl << "Role Designer not found!"  << endl;
            }
            else
            {               
                stat = SA_set_user_login_group(user, group);  
                stat = SA_set_os_user_name(user, "jdoe");  
                
                tag_t member = NULLTAG;
                stat = SA_create_groupmember(group, user, false, &member);

                stat = SA_set_groupmember_role(member, role);       
                stat = AOM_save(user);
                
                scoped_smptr<char> id;
                stat = AOM_ask_value_string(member, "object_string", &id);
                cout << " Created GroupMember: " << id.get() << endl << endl;
            }
        }
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
