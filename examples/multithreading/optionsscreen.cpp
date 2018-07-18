#include "optionsscreen.h"
#include "game.h"

#include <db/dbdatabase.h>
#include <pipeline/pipeline.h>
#include <sprite2d.h>

#include <label.h>
#include <keycodes.h>


// ==================================== OptionsScreen::StateForThread ====================================

OptionsScreen::StateForThread::StateForThread() : Renderer(NULL), SoundVoumeSliderWidth(0.0), MusicVolumeSliderWidth(0.0)
{
    Blinking = new sad::animations::Blinking();
    Blinking->setFrequency(10);
    Blinking->setTime(5000);
    Blinking->setLooped(true);
    Blinking->addRef();

    Instance = new sad::animations::Instance();
    Instance->setAnimation(Blinking);
    Instance->setStartTime(0.0);
    Instance->addRef();
}

OptionsScreen::StateForThread::~StateForThread()
{
    Instance->delRef();
    Blinking->delRef();
}

// ==================================== PUBLIC METHODS  ====================================

OptionsScreen::OptionsScreen() : m_game(NULL), m_editing(false), m_already_handled(false), m_current_menu_item(0)
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

void OptionsScreen::tryStartEditing()
{
    if (!m_editing)
    {
        m_game->playSound("misc_menu_2");
        if ((m_current_menu_item >= OptionsScreen::MIT_LEFT_KEY) && (m_current_menu_item <= OptionsScreen::MIT_MUSIC_VOLUME)) 
        {
            startEditingItem();
        } 
        else
        {
            if (m_current_menu_item == OptionsScreen::MIT_SAVE)
            {
                m_game->options()->save(m_main_state.Renderer);
                m_game->changeSceneToStartingScreen();
            }
            if (m_current_menu_item == OptionsScreen::MIT_RESET)
            {
                m_game->options()->LeftKey = sad::KeyLeft;
                m_game->options()->RightKey = sad::KeyRight;
                m_game->options()->UpKey = sad::KeyUp;
                m_game->options()->DownKey = sad::KeyDown;
                m_game->options()->JumpActionKey = sad::Space;
                m_game->options()->PauseKey = sad::P;

                m_game->conditions()->apply(*(m_game->options()));

                m_game->setThemeVolume(1.0);
                m_game->options()->SoundVolume = 1.0;

                this->initAccordingToConfig();
            }
        }
    }
}

void OptionsScreen::tryHandleEditing(const sad::input::KeyPressEvent& ev)
{
    if (m_already_handled)
    {
        m_already_handled = false;
        return;
    }
    if (m_editing)
    {
        m_game->playSound("misc_menu_2");
        switch (m_current_menu_item)
        {
            case OptionsScreen::MIT_LEFT_KEY:          trySetKeyboardOption(ev.Key, &game::Options::LeftKey, "LeftLabelValue"); break;
            case OptionsScreen::MIT_RIGHT_KEY:         trySetKeyboardOption(ev.Key, &game::Options::RightKey, "RightLabelValue"); break;
            case OptionsScreen::MIT_UP_KEY:            trySetKeyboardOption(ev.Key, &game::Options::UpKey, "UpLabelValue"); break;
            case OptionsScreen::MIT_DOWN_KEY:          trySetKeyboardOption(ev.Key, &game::Options::DownKey, "DownLabelValue"); break;
            case OptionsScreen::MIT_JUMP_ACTION_KEY:   trySetKeyboardOption(ev.Key, &game::Options::JumpActionKey, "JumpActionLabelValue"); break;
            case OptionsScreen::MIT_PAUSE_KEY:         trySetKeyboardOption(ev.Key, &game::Options::PauseKey, "PauseLabelValue"); break;
            case OptionsScreen::MIT_MUSIC_VOLUME:
                if (ev.Key == m_game->options()->LeftKey)
                {
                    changeMusicVolume(-0.05);
                }
                if (ev.Key == m_game->options()->RightKey)
                {
                    changeMusicVolume(0.05);
                }
                if (ev.Key == m_game->options()->JumpActionKey)
                {
                    exitEditingState();
                }
                break;
            case OptionsScreen::MIT_SOUND_VOLUME:
                if (ev.Key == m_game->options()->LeftKey)
                {
                    changeSoundVolume(-0.05);
                }
                if (ev.Key == m_game->options()->RightKey)
                {
                    changeSoundVolume(0.05);
                }
                if (ev.Key == m_game->options()->JumpActionKey)
                {
                    exitEditingState();
                }
                break;
            default:
                exitEditingState();
        }
    }
}


// ==================================== PRIVATE METHODS  ====================================

void OptionsScreen::trySetKeyboardOption(sad::KeyboardKey key, sad::KeyboardKey(game::Options::*key_option), const sad::String& label_name)
{
    if (this->isKeyNotMatchingAnyFromConfigExceptCurrent(key))
    {
        (m_game->options())->*key_option = key;
        m_game->conditions()->apply(*(m_game->options()));
        setLabelValue(m_main_state.Renderer, label_name, sad::keyToString(key));
        setLabelValue(m_inventory_state.Renderer, label_name, sad::keyToString(key));
        exitEditingState();
    }
}

bool OptionsScreen::isKeyNotMatchingAnyFromConfigExceptCurrent(sad::KeyboardKey key) const
{
    sad::Vector<sad::KeyboardKey> keys;
    game::Options* opts = m_game->options();
    keys << opts->LeftKey
         << opts->RightKey
         << opts->DownKey
         << opts->UpKey
         << opts->JumpActionKey
         << opts->PauseKey;
    if (m_current_menu_item >= 0 && m_current_menu_item < keys.size())
    {
        keys.removeAt(m_current_menu_item);
    }
    for (size_t i = 0; i < keys.size(); i++)
    {
        if (keys[i] == key)
        {
            return false;
        }
    }
    return true;
}

void OptionsScreen::exitEditingState()
{
    sad::Renderer* main_renderer = m_main_state.Renderer;
    main_renderer->animations()->remove(m_main_state.Instance);

    sad::Renderer* inventory_renderer = m_inventory_state.Renderer;
    inventory_renderer->animations()->remove(m_inventory_state.Instance);


    m_editing = false;
}

void OptionsScreen::initAccordingToConfig()
{
    initAccordingToConfig(m_main_state);
    initAccordingToConfig(m_inventory_state);
}

void OptionsScreen::changeSoundVolume(double diff) const
{
    m_game->options()->SoundVolume += diff;
    if (m_game->options()->SoundVolume <= 0.0)
    {
        m_game->options()->SoundVolume = 0.0;
    }
    if (m_game->options()->SoundVolume >= 1.0)
    {
        m_game->options()->SoundVolume = 1.0;
    }
    OptionsScreen::setWidthForSliderValue(m_main_state.Renderer, m_game->options()->SoundVolume, "SoundLabelValue", m_main_state.SoundVoumeSliderWidth);
    OptionsScreen::setWidthForSliderValue(m_inventory_state.Renderer, m_game->options()->SoundVolume, "SoundLabelValue", m_inventory_state.SoundVoumeSliderWidth);
}

void OptionsScreen::changeMusicVolume(double diff) const
{
    m_game->options()->MusicVolume += diff;
    if (m_game->options()->MusicVolume <= 0.0)
    {
        m_game->options()->MusicVolume = 0.0;
    }
    if (m_game->options()->MusicVolume >= 1.0)
    {
        m_game->options()->MusicVolume = 1.0;
    }
    m_game->setThemeVolume(m_game->options()->MusicVolume);
    OptionsScreen::setWidthForSliderValue(m_main_state.Renderer, m_game->options()->MusicVolume, "MusicLabelValue", m_main_state.MusicVolumeSliderWidth);
    OptionsScreen::setWidthForSliderValue(m_inventory_state.Renderer, m_game->options()->MusicVolume, "MusicLabelValue", m_inventory_state.MusicVolumeSliderWidth);
}

void OptionsScreen::setWidthForSliderValue(sad::Renderer* renderer, double value, const sad::String& sprite_name, double max_width)
{
    sad::db::Database* db = renderer->database("optionsscreen");
    sad::Sprite2D* sprite = db->objectByName<sad::Sprite2D>(sprite_name);
    if (sprite)
    {
        double new_width = value * max_width;
        sad::Rect2D rect = sprite->area();
        sad::Rect2D new_rect(rect.p0().x(), rect.p0().y(), rect.p0().x() + new_width, rect.p2().y());
        sprite->setArea(new_rect);
    }
}

void OptionsScreen::initAccordingToConfig(OptionsScreen::StateForThread& state) const
{
    sad::db::Database* db = state.Renderer->database("optionsscreen");
    setLabelValue(state.Renderer, "LeftLabelValue", sad::keyToString(m_game->options()->LeftKey));
    setLabelValue(state.Renderer, "RightLabelValue", sad::keyToString(m_game->options()->RightKey));
    setLabelValue(state.Renderer, "DownLabelValue", sad::keyToString(m_game->options()->DownKey));
    setLabelValue(state.Renderer, "UpLabelValue", sad::keyToString(m_game->options()->UpKey));

    setLabelValue(state.Renderer, "JumpLabelValue", sad::keyToString(m_game->options()->JumpActionKey));
    setLabelValue(state.Renderer, "PauseLabelValue", sad::keyToString(m_game->options()->PauseKey));

    OptionsScreen::setWidthForSliderValue(state.Renderer, m_game->options()->SoundVolume, "SoundLabelValue", state.SoundVoumeSliderWidth);
    OptionsScreen::setWidthForSliderValue(state.Renderer, m_game->options()->MusicVolume, "MusicLabelValue", state.MusicVolumeSliderWidth);
}

void OptionsScreen::startEditingItem()
{
    sad::Renderer* main_renderer = m_main_state.Renderer;
    sad::db::Database* main_db = main_renderer->database("optionsscreen");
    sad::SceneNode* main_node = main_db->objectByName<sad::SceneNode>(m_blinking_parts[m_current_menu_item]);
    if (main_node)
    { 
        sad::animations::Instance* instance = m_main_state.Instance;
        instance->clearFinished();
        instance->setObject(main_node);
        main_renderer->pipeline()->appendTask([=]() {
            main_renderer->animations()->add(instance);
        });
    }

    sad::Renderer* inventory_renderer = m_inventory_state.Renderer;
    sad::db::Database* inventory_db = inventory_renderer->database("optionsscreen");
    sad::SceneNode* inventory_node = inventory_db->objectByName<sad::SceneNode>(m_blinking_parts[m_current_menu_item]);
    if (inventory_node)
    {
        sad::animations::Instance* instance = m_inventory_state.Instance;
        instance->clearFinished();
        instance->setObject(inventory_node);
        inventory_renderer->pipeline()->appendTask([=]() {
            inventory_renderer->animations()->add(instance);
        });
    }

    m_editing = true;
    m_already_handled = true;
}

void OptionsScreen::moveToItemByDifference(int difference)
{
    if (!m_editing)
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
    
    sad::Sprite2D* sound_volume = db->objectByName<sad::Sprite2D>("SoundLabelValue");
    if (sound_volume)
    {
        state.SoundVoumeSliderWidth = sound_volume->area().width();
    }
    
    sad::Sprite2D* music_volume = db->objectByName<sad::Sprite2D>("MusicLabelValue");
    if (music_volume)
    {
        state.MusicVolumeSliderWidth = music_volume->area().width();
    }
    
    initAccordingToConfig(state);
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