/*! \file scorebar.h
 *   
 *  A score bar for game
 */
#pragma once
#include <label.h>
// ReSharper disable once CppUnusedIncludeDirective
#include <scene.h>

class Game;

namespace game
{
/*! A score for bar
 */
class ScoreBar
{
public:
    /*! Makes new game
     */
    ScoreBar(Game* game);
    /*! Initializes score bar
     */
    void init();
    /*! De-initializes score bar
     */
    void deinit();
    /*! Updates score bar
     */
    void update() const;
protected:
    /*! A game part
     */
    Game* m_game;
    /*! A scene data
     */
    sad::Scene* m_scene;
    /*! A score label with simple text
     */
    sad::Label* m_score_label;
    /*! A highscore label with simple text
     */
    sad::Label* m_highscore_label;
    /*! A score label data
     */
    sad::Label* m_score_value;
    /*! A highscore label
     */
    sad::Label* m_highscore_value;
    /*! A score offset
     */
    sad::Point2D m_score_offset;
    /*! A highscore offset
     */
    sad::Point2D m_highscore_offset;
    /*! A score label offset
     */
    sad::Point2D m_score_value_offset;
    /*! A highscore label offset
     */
    sad::Point2D m_highscore_value_offset;
};

}
