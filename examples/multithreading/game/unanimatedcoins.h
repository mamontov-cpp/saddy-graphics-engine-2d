/*! \file unanimatedcoins.h

    A list of unanimated coins to animate them, when user approaches them
 */
#pragma once
#include <db/dbdatabase.h>
#include <sprite2d.h>
#include <sadvector.h>
#include <renderer.h>

namespace game
{

/*! A list of unanimated coins to animate them, when user approaches them
 */
class UnanimatedCoins
{
public:
    /*! A maximal distance between coin and player to be animated
     */
    static const double MaxDistance;
    /*! Creates default empty coins list
     */
    UnanimatedCoins();
    /*! Fetches list of coin sprites from database
     * \param[in] db a source database
     * \return list of coins
     */
    static sad::Vector<sad::Sprite2D*> fetchCoinSprites(sad::db::Database* db);
    /*! Initializes list of unanimated coins with list and related database
     * \param[in] sprites list of sprites
     * \param[in] db related db for getting instance
     * \param[in] r renderer
     */
    void init(const sad::Vector<sad::Sprite2D*>& sprites, sad::db::Database* db, sad::Renderer* r);
    /*! Animates nearest coins to middle
     * \param[in] middle a middle to sprite coin
     */
    void animateNearestCoins(const sad::Point2D& middle);
protected:
    /*! A list of sprites
     */
    sad::Vector<sad::Sprite2D*> m_sprites;
    /*! A relates database for coins
     */
    sad::db::Database* m_database;
    /*! A relates renderer
     */
    sad::Renderer* m_renderer;
};

}

