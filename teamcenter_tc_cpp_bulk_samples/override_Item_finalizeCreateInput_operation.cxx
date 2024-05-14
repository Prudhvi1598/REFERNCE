//@<COPYRIGHT>@
//==================================================
//Copyright $2023.
//Siemens Product Lifecycle Management Software Inc.
//All Rights Reserved.
//==================================================
//@<COPYRIGHT>@

// 
//  @file
//  This file contains the implementation for the Business Object A3_ItemImpl
//

#include <A3_extensions/A3_ItemImpl.hxx>
#include <iostream>
#include <stdio.h>
#include <stdarg.h>

#include <ug_va_copy.h>
#include <tc/emh.h>
#include <tccore/aom.h>

#include <base_utils/IFail.hxx>
#include <base_utils/ResultCheck.hxx>
#include <base_utils/TcResultStatus.hxx>
#include <Error_Exception.hxx>
#include <extensionframework/OperationDispatcherRegistry.hxx>
#include <form/Form.hxx>
#include <metaframework/BusinessObjectRef.hxx>
#include <metaframework/BusinessObjectRegistry.hxx>
#include <metaframework/CreateInput.hxx>
#include <metaframework/OperationInput.hxx>

using namespace std;
using namespace Teamcenter;

using namespace a3extensions;

//----------------------------------------------------------------------------------
// A3_ItemImpl::A3_ItemImpl(A3_Item& busObj)
// Constructor for the class
//----------------------------------------------------------------------------------
A3_ItemImpl::A3_ItemImpl( A3_Item& busObj )
   : A3_ItemGenImpl( busObj )
{
}

//----------------------------------------------------------------------------------
// A3_ItemImpl::~A3_ItemImpl()
// Destructor for the class
//----------------------------------------------------------------------------------
A3_ItemImpl::~A3_ItemImpl()
{
}

//----------------------------------------------------------------------------------
// A3_ItemImpl::initializeClass
// This method is used to initialize this Class
//----------------------------------------------------------------------------------
int A3_ItemImpl::initializeClass()
{
    int ifail = ITK_ok;
    static bool initialized = false;

    if( !initialized )
    {
        ifail = A3_ItemGenImpl::initializeClass( );
        if ( ifail == ITK_ok )
        {
            initialized = true;
        }
    }
    return ifail;
}



///
/// Description for the Finalize Create Input
/// @version Tc 8
/// @param pItemCreateInput - desc for  pItemCreateInput parameter
/// @return - Return desc for Initialize for Create
///
int  A3_ItemImpl::finalizeCreateInputBase( ::Teamcenter::CreateInput *pItemCreateInput )
{
    int ifail = ITK_ok;
    ResultStatus stat;
    
    // Call the parent Implementation
    ifail = super_finalizeCreateInputBase( pItemCreateInput );

    try
    {       
        string sItem_id = "";
        bool isNull1=false;
        pItemCreateInput->getString("item_id", sItem_id, isNull1);

        if(!sItem_id.empty())
        {
            string sNewItem_id = "000123"; 
            bool isNull=false;
            pItemCreateInput->setString("item_id", sNewItem_id, isNull);

            CreateInput *pMasterCreateInput = static_cast<CreateInput*>(BusinessObjectRegistry::instance().createInputObject("Item Master", OPERATIONINPUT_CREATE));
            pMasterCreateInput->setString("object_name", sNewItem_id, false);
            pMasterCreateInput->setString("user_data_1", "Some sNewItem_idue", false);
            
            Teamcenter::Form *pMaster = dynamic_cast<Form *>(BusinessObjectRegistry::instance().createBusinessObject(pMasterCreateInput));
            stat = AOM_save_with_extensions(pMaster->getTag());

            pItemCreateInput->setTag("item_master_tag", pMaster->getTag(), false);
            stat = AOM_save_with_extensions(pMaster->getTag());
        }
    }
    catch( const IFail &e )
    {
        cout << "error " << e.ifail() << endl;
        cout << e.getMessage() << endl;
    }
    return ifail;   
}
