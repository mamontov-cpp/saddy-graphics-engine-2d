/*! \file exposespawnenemy.h
 *
 *   Exposes enemy spawning options
 */
#pragma once

class Game;

namespace scripting
{
/*! Expose spawnXXX functions to script
 * \param[in] c context
 * \param[in] game a game
 */
void exposeSpawnEnemy(void* c, Game* game);

}
