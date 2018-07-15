#include "optionsscreen.h"

#include <db/dbdatabase.h>
#include <sprite2d.h>

// ==================================== PUBLIC METHODS  ====================================

OptionsScreen::OptionsScreen() : m_main_renderer(NULL), m_inventory_renderer(NULL)
{
    m_label_underlays << "LeftUnderlay" << "RightUnderlay" << "DownUnderlay" << "UpUnderlay" << "JumpActionUnderlay" << "SoundUnderlay" << "MusicUnderlay" << "PauseUnderlay" << "SaveUnderlay" << "ResetUnderlay";
}

void OptionsScreen::init(sad::Renderer* main_renderer, sad::Renderer* inventory_renderer)
{
    m_main_renderer = main_renderer;
    m_inventory_renderer = inventory_renderer;
}


void OptionsScreen::initForMainRenderer()
{
    this->initForRenderer(m_main_renderer);
}


void OptionsScreen::initForInventoryRenderer()
{
    this->initForRenderer(m_inventory_renderer);

}

// ==================================== PRIVATE METHODS  ====================================


void OptionsScreen::initForRenderer(sad::Renderer* renderer)
{
    sad::db::Database* db = renderer->database("optionsscreen");
    for (size_t i = 0; i < m_label_underlays.size(); i++)
    {
        sad::Sprite2D* o = db->objectByName<sad::Sprite2D>(m_label_underlays[i]);
        if (o)
        {
            o->setVisible(false);
        }
    }

    sad::Sprite2D* o = db->objectByName<sad::Sprite2D>(m_label_underlays[0]);
    if (o)
    {
        o->setVisible(true);
    }
}