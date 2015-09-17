#include "animations/animationsfile.h"


// =================================== PUBLIC METHODS  ===================================

sad::animations::File::File(const sad::String& name)
: sad::resource::PhysicalFile(name)
{
    
}

sad::animations::File::~File()
{

}

sad::Vector<sad::resource::Error*> sad::animations::File::load(
    sad::resource::Folder * parent
)
{
    sad::Vector<sad::resource::Error*> errors;
    sad::Vector<sad::animations::File::parse_result> parseresult;
    this->tryParsePartial(parseresult, errors);
    if (errors.size() == 0)
    {	
        // Resource list
        sad::resource::ResourceEntryList resourcelist;
        for(size_t i = 0; i < parseresult.size(); i++)
        {
            sad::Maybe<sad::String> maybetype = picojson::to_type<sad::String>(
                            picojson::get_property(parseresult[i].p2(), "type")
                        );
            sad::animations::Animation* a = m_factory.create(maybetype.value());
            bool error = true;
            if (a)
            {
                if (a->loadFromValue(parseresult[i].p2()))
                {
                    if (this->tree()->shouldStoreLinks())
                    {
                        a->enableStoringLinks();
                    }
                    a->setPhysicalFile(this);
                    error = false;
                    resourcelist << sad::resource::ResourceEntry(parseresult[i].p1(), a);
                }
                else
                {
                    delete a;
                }
            }
            if (error)
            {
                errors << new sad::resource::MalformedResourceEntry(parseresult[i].p2());
            }
        }

        sad::resource::ResourceEntryList oldresourcelist;
        sad::resource::ResourceEntryList tobeadded, tobereplaced, toberemoved;
        if (errors.size() == 0)
        {
            this->createOldResourceList(oldresourcelist);
            this->diffResourcesLists(oldresourcelist, resourcelist, tobeadded, tobereplaced, toberemoved);
            errors << this->tree()->duplicatesToErrors(this->tree()->root()->duplicatesBetween(tobeadded));
            convertReferencedOptionsToBeRemovedToErrors(toberemoved, errors);
            if (errors.size() == 0)
            {
                sad::resource::Folder * root = this->tree()->root();
                // Add an added resources
                parent->addResources(tobeadded);
                // Replace replaced resources
                parent->replaceResources(tobereplaced);
                // Remove removable resources
                parent->removeResources(toberemoved, true);
                // Replace own resource list, setting correct reference to this
                this->replaceResources(resourcelist);
            }

        }
        if (errors.size() != 0)
        {
            sad::resource::free(resourcelist);
        }
    }
    return errors;
}

sad::Vector<sad::resource::Error*> sad::animations::File::reload()
{
    return load(this->tree()->root());
}

// =================================== PROTECTED METHODS  ===================================

void sad::animations::File::tryParsePartial(
        sad::Vector<sad::animations::File::parse_result> & result,
        sad::Vector<sad::resource::Error *> & errors 
)
{
    sad::Maybe<sad::String> maybecontent = this->tryReadToString();
    if (maybecontent.exists())
    {
        if (maybecontent.value().consistsOfWhitespaceCharacters())
        {
            return;
        }
        picojson::value rootvalue = picojson::parse_string(maybecontent.value());
        if (picojson::get_last_error().size() == 0)
        {
            if (rootvalue.is<picojson::object>())
            {
                picojson::object obj = rootvalue.get<picojson::object>();
                for(picojson::object::iterator it = obj.begin();
                    it != obj.end();
                    ++it)
                {
                    sad::Maybe<sad::String> maybetype = picojson::to_type<sad::String>(
                            picojson::get_property(it->second, "type")
                        );
                    if (maybetype.exists())
                    {
                        result << sad::animations::File::parse_result(it->first, it->second);
                    }
                    else
                    {
                        errors << new sad::resource::MalformedResourceEntry(it->second);
                    }
                }
            }
            else
            {
                errors << new sad::resource::MalformedResourceEntry(rootvalue);
            }
        }
        else
        {
            errors << new sad::resource::JSONParseError();
        }
    }
    else
    {
        errors << new sad::resource::FileLoadError(m_name);
    }
}