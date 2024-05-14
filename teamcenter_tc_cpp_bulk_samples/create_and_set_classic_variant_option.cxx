#include <gtac.hxx>
#include <iostream>
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
    stat = ITK_initialize_text_services(ITK_BATCH_TEXT_MODE);         
    stat = ITK_auto_login();                                          
    stat = ITK_set_journalling(TRUE);   
    try
    {
        tag_t tRev = NULLTAG;
        stat = ITEM_find_rev("10093399", "A", &tRev);
        
        tag_t tWindow = NULLTAG;
        stat = BOM_create_window (&tWindow);
        
        tag_t tLine = NULLTAG; // Top Line
        stat = BOM_set_window_top_line(tWindow, NULLTAG, tRev, NULLTAG, &tLine);
    
        int iLines = 0;
        tag_t* ptLines = NULL;
        stat = BOM_line_ask_child_lines(tLine, &iLines, &ptLines);
		tLine = ptLines[0]; // Configure first child line       
        if(ptLines) MEM_free(ptLines);

        tag_t tOpt = NULLTAG;
        tag_t tOptRev = NULLTAG;
        tag_t tExp = NULLTAG;
        tag_t tExpBlk = NULLTAG;
        stat = BOM_new_option(tRev, "Color", "", BOM_option_mode_enum, &tOpt, &tOptRev, &tExp, &tExpBlk);

        int iIndex = 0;
        stat = BOM_add_option_rev_value(tOptRev, "Red", &iIndex);
        stat = BOM_add_option_rev_value(tOptRev, "Blue", &iIndex);
        stat = BOM_add_option_rev_value(tOptRev, "Green", &iIndex);
    
        tag_t tList = NULLTAG;
        stat = BOM_variant_new_clause_list (tWindow , &tList);
        stat = BOM_variant_clause_append (tList, BOM_variant_operator_or, tOpt, BOM_variant_operator_is_equal, "Green");
      
        tag_t tCondition = NULLTAG;
        stat = BOM_variant_join_clause_list (tList , &tCondition);

        tag_t tLoadIfCondition = NULLTAG;
        stat = BOM_variant_expr_load_if(tCondition, &tLoadIfCondition);
        stat = BOM_variant_delete_clause_list(tList);
        stat = AOM_save(tLoadIfCondition);

        tag_t tExpBlock = NULLTAG;
        stat = BOM_new_variant_e_block(&tExpBlock);
        stat = BOM_variant_e_block_add(tExpBlock, tLoadIfCondition);
        stat = BOM_line_set_variant_e_block(tLine, tExpBlock);
        
        stat = AOM_save (tLoadIfCondition);
        stat = AOM_save (tExpBlock);                
        stat = AOM_save(tOptRev);
        stat = AOM_save(tOpt);
        stat = AOM_save(tExp);
        stat = AOM_save(tExpBlk);
        stat = BOM_save_window(tWindow);
        stat = BOM_close_window(tWindow);
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
