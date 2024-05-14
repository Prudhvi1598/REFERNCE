
#include <iostream>
#include <me/me.h>
#include <tc/emh.h>
#include <tc/tc.h>
#include <tc/tc_startup.h>


#include <sa/user.h>
#include <tccore/project.h>
#include <tccore/aom.h>
#include <tccore/aom_prop.h>
#include <metaframework/BusinessObjectRef.hxx>
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
    logical done = FALSE;
    try
    {
        stat = ITK_initialize_text_services(ITK_BATCH_TEXT_MODE);         
        stat = ITK_auto_login();                                          
        stat = ITK_set_journalling(TRUE);
    
        tag_t user = NULLTAG;
        stat = SA_find_user2("gtac", &user); 
        
        
        tag_t group =  NULLTAG;
        stat = SA_find_group("Project Administration", &group);
                
        tag_t role = NULLTAG;
        stat = SA_find_role2("Project Administrator", &role);
        
        tag_t gm = NULLTAG;
        stat = SA_create_groupmember(group, user, FALSE, &gm);
        stat = SA_set_groupmember_role(gm, role);
        stat = AOM_save(gm);

        stat = SA_find_group("Engineering", &group);
        
        int n_gms = 0;
        tag_t *gms = NULL;
        stat = SA_find_groupmembers_by_group(group, &n_gms, &gms);
        
        char project_ids[4][10] = {"ProjectA", "ProjectB", "ProjectC", "ProjectD"};
        int n_projects = 4;
        tag_t project = NULLTAG;        
        for (int ii = 0; ii < n_projects; ii++)
        {
            stat = PROJ_create_project(project_ids[ii], project_ids[ii], "", &project);         
            stat = PROJ_assign_team_members(project, n_gms, gms, 1, &user, 1, &user);
        }
    }
    catch( const IFail &ex )
    {
        logger()->error( ex.ifail(), ex.getMessage());
        cout << ex.getMessage()  << endl;
        cout << "error " << ex.ifail() << endl;        
    }

    stat = ITK_exit_module(FALSE);                                        
    return ITK_ok;
}
