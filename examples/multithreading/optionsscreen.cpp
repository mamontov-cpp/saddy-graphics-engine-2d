#include "optionsscreen.h"
#include "game.h"

#include <db/dbdatabase.h>
#include <sprite2d.h>

#include <label.h>
#include <keycodes.h>

// ==================================== OptionsScreen::StateForThread ====================================

OptionsScreen::StateForThread::StateForThread() : Renderer(NULL), SoundVoumeSliderWidth(0.0), MusicVolumeSliderWidth(0.0)
{
    Blinking = new sad::animations::Blinking();
    Blinking->setFrequency(100);
    Blinking->setTime(10);
    Blinking->addRef();

    Instance = new sad::animations::Instance();
    Instance->setAnimation(Blinking);
    Instance->setStartTime(0.0);
    Instance->delRef();
}

OptionsScreen::StateForThread::~StateForThread()
{
    Instance->delRef();
    Blinking->delRef();
}

// ==================================== PUBLIC METHODS  ====================================

OptionsScreen::OptionsScreen() : m_game(NULL), m_editing(false), m_current_menu_item(0)
{
    m_label_underlays << "LeftUnderlay" << "RightUnderlay" << "DownUnderlay" << "UpUnderlay" << "JumpActionUnderlay" <<  "PauseUnderlay" <<  "SoundUnderlay" << "MusicUnderlay" << "SaveUnderlay" << "ResetUnderlay";
    m_blinking_parts  << "LeftLabelValue" << "RightLabelValue" << "DownLabelValue" << "UpLabelValue" << "JumpActionLabelValue" <<  "PauseLabelValue" << "SoundLabelValue" << "MusicLabelValue";
}

OptionsScreen::~OptionsScreen()
{
    
}

void OptionsScreen::init(Game* game, sad::Renderer* main_renderer, sad::Renderer* inventory_renderer)
{
    m_game = game;
    m_main_state.Renderer = main_renderer;
    m_inventory_state.Renderer = inventory_renderer;
}


void OptionsScreen::initForMainRenderer()
{
    this->initForRenderer(m_main_state);
}


void OptionsScreen::initForInventoryRenderer()
{
    this->initForRenderer(m_inventory_state);
}

void OptionsScreen::moveToNextItem()
{
    this->moveToItemByDifference(1);
}

void OptionsScreen::moveToPreviousItem()
{
    this->moveToItemByDifference(-1);
}


// ==================================== PRIVATE METHODS  ====================================

void OptionsScreen::moveToItemByDifference(int difference)
{
    m_game->playSound("misc_menu");
    sad::Renderer* main_renderer = m_main_state.Renderer;
    sad::db::Database* main_db = main_renderer->database("optionsscreen");
    sad::Sprite2D* main_old_label = main_db->objectByName<sad::Sprite2D>(m_label_underlays[m_current_menu_item]);
    if (main_old_label)
    {
        main_old_label->setVisible(false);
    }

    sad::Renderer* inventory_renderer = m_inventory_state.Renderer;
    sad::db::Database* inventory_db = inventory_renderer->database("optionsscreen");
    sad::Sprite2D* inventory_old_label = inventory_db->objectByName<sad::Sprite2D>(m_label_underlays[m_current_menu_item]);
    if (inventory_old_label)
    {
        inventory_old_label->setVisible(false);
    }

    m_current_menu_item += difference;
    if (m_current_menu_item == static_cast<int>(m_label_underlays.size()))
    {
        m_current_menu_item = 0;
    }
    if (m_current_menu_item < 0)
    {
        m_current_menu_item = m_label_underlays.size() - 1;
    }

    sad::Sprite2D* main_new_label = main_db->objectByName<sad::Sprite2D>(m_label_underlays[m_current_menu_item]);
    if (main_new_label)
    {
        main_new_label->setVisible(true);
    }

    sad::Sprite2D* inventory_new_label = inventory_db->objectByName<sad::Sprite2D>(m_label_underlays[m_current_menu_item]);
    if (inventory_new_label)
    {
        inventory_new_label->setVisible(true);
    }
}

void OptionsScreen::initForRenderer(OptionsScreen::StateForThread& state)
{
    sad::db::Database* db = state.Renderer->database("optionsscreen");
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
    setLabelValue(state.Renderer, "LeftLabelValue", sad::keyToString(m_game->options()->LeftKey));
    setLabelValue(state.Renderer, "RightLabelValue", sad::keyToString(m_game->options()->RightKey));
    setLabelValue(state.Renderer, "DownLabelValue", sad::keyToString(m_game->options()->DownKey));
    setLabelValue(state.Renderer, "UpLabelValue", sad::keyToString(m_game->options()->UpKey));

    setLabelValue(state.Renderer, "JumpLabelValue", sad::keyToString(m_game->options()->JumpActionKey));
    setLabelValue(state.Renderer, "PauseLabelValue", sad::keyToString(m_game->options()->PauseKey));

    sad::Sprite2D* sound_volume = db->objectByName<sad::Sprite2D>("SoundLabelValue");
    if (sound_volume)
    {
        state.SoundVoumeSliderWidth = sound_volume->area().width();
        double new_width =  (m_game->options()->SoundVolume) *  state.SoundVoumeSliderWidth;
        sad::Rect2D rect = sound_volume->area();
        sad::Rect2D new_rect(rect.p0().x(), rect.p0().y(), rect.p0().x() + new_width, rect.p2().y());
        sound_volume->setArea(new_rect);
    }

    sad::Sprite2D* music_volume = db->objectByName<sad::Sprite2D>("MusicLabelValue");
    if (sound_volume)
    {
        state.MusicVolumeSliderWidth = music_volume->area().width();
        double new_width = (m_game->options()->MusicVolume) *  state.MusicVolumeSliderWidth;
        sad::Rect2D rect = music_volume->area();
        sad::Rect2D new_rect(rect.p0().x(), rect.p0().y(), rect.p0().x() + new_width, rect.p2().y());
        music_volume->setArea(new_rect);
    }
    m_current_menu_item = 0;
}

void OptionsScreen::setLabelValue(sad::Renderer* renderer, const sad::String& label_name, const sad::String& value)
{
    sad::Label* label = renderer->database("optionsscreen")->objectByName<sad::Label>(label_name);
    sad::layouts::Grid* grid = OptionsScreen::getGridForRenderer(renderer);
    if (label && grid)
    {
        label->setString(value);
        grid->update();
    }
}

sad::layouts::Grid* OptionsScreen::getGridForRenderer(sad::Renderer* renderer)
{
    return renderer->database("optionsscreen")->objectByName<sad::layouts::Grid>("layout");
}