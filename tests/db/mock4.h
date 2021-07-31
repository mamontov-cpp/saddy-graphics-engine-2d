/*! \file mock4.h

    A mock object for testing non-required fields
 */
#pragma once
#include "object.h"
#include "db/schema/schema.h"

class Mock4: public sad::Object
{
SAD_OBJECT
public:
    Mock4();
    virtual sad::db::schema::Schema * schema() const override;
    
    int m_id;

    sad::db::schema::Schema m_schema;
    
};
