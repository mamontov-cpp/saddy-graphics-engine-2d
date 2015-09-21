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
    \param[in] clear whether we should clear scenes of renderer before populating scenes
 */
void populateScenesFromDatabase(sad::Renderer* r, sad::db::Database* db, bool clear = true);

}

}
