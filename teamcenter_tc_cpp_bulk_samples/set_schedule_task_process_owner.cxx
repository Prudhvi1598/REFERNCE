#include <iostream>
#include <tc/emh.h>
#include <tc/tc.h>
#include <tc/tc_startup.h>
#include <sa/user.h>
#include <schmgt/schmgt_bridge_itk.h>
#include <schmgt/schmgt_itk.h>
#include <tccore/aom.h>

#include <base_utils/IFail.hxx>
#include <base_utils/TcResultStatus.hxx>
#include <metaframework/BusinessObjectRef.hxx>
#include <mld/logging/TcMainLogger.hxx>
#include <schmgt/ScheduleTask.hxx>

using namespace std;
using namespace Teamcenter;
using Teamcenter::Main::logger;

int setWorkflowProcessOwner(tag_t taskTag)
{
    int ifail = ITK_ok;
    ResultStatus stat;
    try
    {
        tag_t userTag = NULLTAG;
        stat = SA_find_user2("infodba", &userTag);
            
        stat = AOM_lock(taskTag);

        BusinessObjectRef<Teamcenter::ScheduleTask> taskBusObj(taskTag);
        stat = taskBusObj->setFnd0workflow_owner(userTag, false);
        
        stat = AOM_save(taskTag);
        stat = AOM_unlock(taskTag);         
    }
    catch( const IFail &e )
    {
        ifail = e.ifail();
        cout << "\n    error " << ifail << endl;
        cout << "    "<< e.getMessage() <<endl;
    }
    return ifail;
}
