#include "scorebar.h"

#include "../game.h"

#include <camera.h>

#include <db/dbdatabase.h>

game::ScoreBar::ScoreBar(Game* game) 
: m_game(game),
m_scene(NULL),
m_score_label(NULL),
m_highscore_label(NULL),
m_score_value(NULL),
m_highscore_value(NULL)
{
    
}

void game::ScoreBar::init()
{
    sad::db::Database* db = m_game->rendererForMainThread()->database("gamescreen");
    if (db)
    {
        m_scene = db->objectByName<sad::Scene>("gui");

        m_score_label = db->objectByName<sad::Label>("ScoreLabel");
        m_highscore_label = db->objectByName<sad::Label>("HighscoreLabel");

        m_score_value = db->objectByName<sad::Label>("ScoreValue");
        m_highscore_value = db->objectByName<sad::Label>("HighscoreValue");

        if (m_score_value)
        { 
            m_score_value_offset = m_score_value->point();
        }

        if (m_highscore_value)
        {
            m_highscore_value_offset = m_highscore_value->point();
        }

        if (m_score_label)
        {
            m_score_offset = m_score_label->point();
        }

        if (m_highscore_label)
        {
            m_highscore_offset = m_highscore_label->point();
        }
    }
}

void game::ScoreBar::deinit()
{
    m_scene = NULL;

    m_score_label = NULL;
    m_highscore_label = NULL;

    m_score_value = NULL;
    m_highscore_value = NULL;
}

void game::ScoreBar::update() const
{
    if (m_scene)
    {
        sad::Point2D offset(m_scene->camera().TranslationOffset.x(), m_scene->camera().TranslationOffset.y());
        m_score_label->setPoint(offset + m_score_offset);
        m_highscore_label->setPoint(offset + m_highscore_offset);

        m_score_value->setPoint(offset + m_score_value_offset);
        m_highscore_value->setPoint(offset + m_highscore_value_offset);

        m_score_value->setString(sad::String::number(m_game->score()));
        m_highscore_value->setString(sad::String::number(m_game->highscore()));
    }
}