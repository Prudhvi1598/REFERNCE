
#include <tc/emh.h>
#include <tc/tc.h>
#include <tc/tc_startup.h>
#include <ae/dataset.h>
#include <sa/tcfile.h>
#include <tccore/aom.h>
#include <sa/tcfile_cache.h>


#include <iostream>
#include <base_utils/IFail.hxx>
#include <base_utils/TcResultStatus.hxx>

using namespace std;

int ITK_user_main(int argc, char* argv[])
{
    int ifail = ITK_ok;
    ResultStatus rstat;
    try
    {
        rstat = ITK_initialize_text_services(ITK_BATCH_TEXT_MODE);         
        rstat = ITK_auto_login();                                          
        rstat = ITK_set_journalling(TRUE);
           
        tag_t dataset = NULLTAG;
        rstat = AE_find_dataset("9227931/A", &dataset);
        
        AE_reference_type_t ref_type;
        tag_t old_file = NULLTAG;
        rstat = AE_ask_dataset_named_ref(dataset, "Text", &ref_type, &old_file);
        
        IMF_file_data_p_t file_data; 
        rstat = IMF_get_file_access(old_file, 0, &file_data);
        
        tag_t new_file_tag = NULLTAG; 
        rstat = AOM_lock(old_file);
        rstat = IMF_replace_file_and_get_new_tag(old_file, "W:\\two.txt", FALSE,
                    &new_file_tag);

        rstat = AOM_lock(dataset);
        rstat = AE_replace_dataset_named_ref(dataset, old_file, "Text", AE_PART_OF, 
                    new_file_tag);

        rstat = AE_save_myself(dataset);
        rstat = AOM_unload(dataset);
        rstat = IMF_release_file_access (&file_data);

        rstat = AOM_lock_for_delete(old_file);
        rstat = AOM_delete(old_file);
    }
   catch( const IFail &e )
    {
        cout << "error " << e.ifail() << endl;
        cout << e.getMessage() << endl;
    }
    
    const char* syslog_path;
    syslog_path = EMH_ask_system_log();
    cout << endl << endl;
    cout << syslog_path << endl; 
    
    rstat = ITK_exit_module(FALSE);                                        
    return ITK_ok;
}
