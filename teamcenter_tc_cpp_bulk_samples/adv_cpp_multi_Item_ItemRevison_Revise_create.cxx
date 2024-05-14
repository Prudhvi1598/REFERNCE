/*
The following code is a C++14 using the ITK C++ interface. 

The code will create a number of Items usng the list of names in: std::vector<std::string> names;
Then it will iterate over the list of newly created Items and revise each Item's ItemRevision twice.

Each Item will have three ItemRevisions, Revision, A, B, C. 

This is an example in how to use templates and lambdas to effectively leverage the ITK C++ interface.

Author: Patrick Hoonhout
Date: 5/23/2022
*/


#include <iostream>

#include <tccore/item.h>
#include <tccore/item.hxx>
#include <tccore/ItemRevision.hxx>
#include <metaframework/BusinessObjectRef.hxx>
#include <metaframework/CreateInput.hxx>
#include <metaframework/BusinessObjectRegistry.hxx>

#include <tccore/tctype.h>
#include <tccore/aom.h>
#include <tccore/aom_prop.h>

// lambda to create a Teamcenter::CreateInput given a type name. 
auto createInput = [](const std::string& type, Teamcenter::BusinessObjectRegistry& boReg = Teamcenter::BusinessObjectRegistry::instance()) { return dynamic_cast<Teamcenter::CreateInput*>(boReg.createInputObject(type.c_str(), OPERATIONINPUT_CREATE)); };
// lambda to create a Teamcenter::ReviseInput given a type name. 
auto createReviseInput = [](const std::string& type, Teamcenter::BusinessObjectRegistry& boReg = Teamcenter::BusinessObjectRegistry::instance()) { return dynamic_cast<Teamcenter::ReviseInput*>(boReg.createInputObject(type.c_str(), OPERATIONINPUT_REVISE)); };
	
// helper template to call BusinessObjectRegistry::ceateBusinessObject(...).
template<typename T>
T* createObject(Teamcenter::CreateInput* input, Teamcenter::BusinessObjectRegistry& boReg = Teamcenter::BusinessObjectRegistry::instance()) { return dynamic_cast<T*>(boReg.createBusinessObject(input)); }
// helper template to get a specialized Teamcenter::POM_object from a tag_t.
template<typename T>
T* getObject(const tag_t tag, Teamcenter::BusinessObjectRegistry& boReg = Teamcenter::BusinessObjectRegistry::instance()) { return dynamic_cast<T*>(boReg.getObject(tag)); }
// helper template to load a specialized Teamcenter::POM_object from a tag_t.
template<typename T>
T* loadObject(const tag_t tag, Teamcenter::BusinessObjectRegistry& boReg = Teamcenter::BusinessObjectRegistry::instance()) { return dynamic_cast<T*>(boReg.load(tag)); }

std::vector<Teamcenter::Item*> createItems(const std::vector<std::string>& nameList)
{
	using namespace Teamcenter;
	// lambda to create an individual Item.
	const auto itemCreate = [](const std::string& name) {
		scoped_ptr<CreateInput> itemCreInput(createInput("Item"));

		itemCreInput->setString("object_name", name.c_str(), false);

		auto pItem = createObject<Teamcenter::Item>(itemCreInput);

		if(pItem != nullptr)
			AOM_save_with_extensions(pItem->getTag());

		return pItem;
	};
	std::vector<Item*> items;
	// create individual Item objects and add them to a list.
	for (const auto& name : nameList)
	{
		auto pItem = itemCreate(name);

		if (pItem != nullptr)
			items.push_back(pItem);
	}
	
	return items;
}


void testCreateObjects()
{
	using namespace Teamcenter;
	// lambda to revise an ItemRevision
	const auto reviseItemRev = [](ItemRevision* pItemRev)
	{
		if (pItemRev != nullptr)
		{
			scoped_ptr<ReviseInput> reviseInput(createReviseInput("ItemRevision"));
			tag_t reviseTag = NULLTAG;

			pItemRev->revise(reviseInput, nullptr, &reviseTag);

			if (reviseTag != NULLTAG)
			{
				AOM_save_with_extensions(reviseTag);

				return getObject<ItemRevision>(reviseTag);
			}
		}

		return static_cast<ItemRevision*>(nullptr);
	};
	// lambda to get an Item's revision list.
	const auto getItemRevList = [](Item* pItem)
	{
		std::vector<tag_t> tagList;

		if (pItem != nullptr)
		{
			std::vector<int> isNullList;

			pItem->getTagArray("revision_list", tagList, isNullList);
		}

		return tagList;
	};

	std::vector<std::string> names { "iOne", "iTwo", "iThree", "iFour", "iFive"  };

	auto objList = createItems(names);

	for (auto pItem : objList)
	{
		if (pItem != nullptr)
		{
			auto tagList = getItemRevList(pItem);

			for (auto tag : tagList)
			{
				auto pItemRev = reviseItemRev(getObject<Teamcenter::ItemRevision>(tag));
				
				pItemRev = reviseItemRev(pItemRev);
			}
		}
	}
}