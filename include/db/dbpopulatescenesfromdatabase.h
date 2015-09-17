/*! \file dbpopulatescenesfromdatabase.h
    

    Populates scene from database.
 */
#pragma once

namespace sad
{

class Renderer;

namespace db
{

class Database;

/*! Populates a scenes for renderer from database
    \param[in] r a renderer
    \param[in] db a database
 */
void populateScenesFromDatabase(sad::Renderer * r, sad::db::Database * db);

}

}
