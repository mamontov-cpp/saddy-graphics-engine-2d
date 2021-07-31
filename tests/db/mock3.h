/*! \file mock3.h

    A mock object for item
 */
#pragma once
#include "object.h"
#include "db/schema/schema.h"

class Mock3: public sad::Object
{
SAD_OBJECT
public:
    Mock3();
    
    int id();
    int id_c() const;
    int& id_r();
    int& id_rc() const;
    const int& id_cr();
    const int& id_crc() const;

    void setId(int i);
    void setIdC(int i) const;
    void setIdR(int & i);
    void setIdRC(int & i) const;
    void setIdCR(const int & i);
    void setIdCRC(const int & i) const;
    virtual sad::db::schema::Schema * schema() const override;

    int m_id;


    sad::db::schema::Schema m_schema;
};
