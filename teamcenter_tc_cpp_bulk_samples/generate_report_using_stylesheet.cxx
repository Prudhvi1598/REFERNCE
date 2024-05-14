
#include <iostream>
#include <qry/rep.h>
#include <sa/tcfile.h>
#include <tc/emh.h>
#include <tc/tc.h>
#include <tc/tc_startup.h>

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
    try
    {
        stat = ITK_initialize_text_services(ITK_BATCH_TEXT_MODE);         
        stat = ITK_auto_login();                                          
        stat = ITK_set_journalling(TRUE);
        
        char cmd[256] = "";
        
        char item_id[] = "9732552";
        char revrule[] = "\"Latest Working\"";
        char transfermode[]= "TIEExportDefaultTM";
        char dir[] = "C:\\Temp\\Reports";
            
        /********************************************************************/
            cout << endl << "Copying stylesheet to report directory" << endl; 
        /*********************************************************************/
        char xsl_file[] = "GTAC_Object_Sample_Template.xsl";    
        sprintf(cmd, "copy %s %s\\%s", xsl_file, dir, xsl_file);
        system(cmd);
        
        /*********************************************************************/
           cout << endl << "Exporting BOM using tcxml_export utility" << endl;        
        /*********************************************************************/
        char tcxml_file[IMF_filename_size_c + 1] = "";
        sprintf(tcxml_file, "TCXML_%s.xml", item_id); 
        
        sprintf(cmd, "tcxml_export -u=infodba -p=infodba -item=%s -revrule=%s -transfermode=%s -file=%s\\%s",
            item_id, revrule, transfermode, dir, tcxml_file);
        system(cmd);
        
        /*********************************************************************/
           cout << endl << "Creating HTML report" << endl;        
        /*********************************************************************/
        char html_file[IMF_filename_size_c + 1] = "";
        sprintf(html_file, "%s.html", item_id);     
        stat = REP_apply_stylesheet(tcxml_file, xsl_file, html_file, dir);
		
        sprintf(cmd, "explorer %s\\%s",  dir, html_file);
        system(cmd);
    }
    catch( const IFail &ex )
    {
        logger()->error( ex.ifail(), ex.getMessage());
        cout << ex.getMessage()  << endl;
        cout << "error " << ex.ifail() << endl;        
    }

    stat = ITK_exit_module(FALSE);                                        
    return ifail;
}
