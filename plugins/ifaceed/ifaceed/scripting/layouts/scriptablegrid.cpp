#include "scriptablegrid.h"

#include <renderer.h>

#include <layouts/grid.h>

#include <db/dbdatabase.h>


scripting::layouts::ScriptableGrid::ScriptableGrid(
		unsigned long long major_id,
		scripting::Scripting* s
) : m_majorid(major_id), m_scripting(s)
{
	
}

scripting::layouts::ScriptableGrid::~ScriptableGrid() 
{
	
}


QString scripting::layouts::ScriptableGrid::toString() const
{
    if (!valid())
    {
        return "ScriptableGrid(<invalid>)";
    }
    QString result = QString("ScriptableGrid(majorid : %1)")
                     .arg(m_majorid);
    return result;	
}

bool scripting::layouts::ScriptableGrid::valid() const
{
	sad::db::Database* db = sad::Renderer::ref()->database("");
	if (db)
	{
		sad::layouts::Grid* grid = db->objectByMajorId<sad::layouts::Grid>(m_majorid);
		if (grid)
		{
			if (grid->Active)
			{
				return true;
			}
		}
	}
	return false;
}
