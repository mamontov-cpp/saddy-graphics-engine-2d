/*! \file optionsscreen.h
 
    An options screen
 */
#pragma once
#include <sadvector.h>
#include <sadstring.h>

#include <layouts/grid.h>

#include <animations/animationsblinking.h>
#include <animations/animationsinstance.h>
#include <input/events.h>


namespace sad
{
class Renderer;
}

namespace game
{
class Options;
}

class Game;

/*! An options screen data
 */
class OptionsScreen  // NOLINT(hicpp-special-member-functions, cppcoreguidelines-special-member-functions)
{
public:
    /*! A type of selected menu ite
     */ 
    enum MenuItemType
    {
        MIT_LEFT_KEY  = 0,       //!< A left key is selected
        MIT_RIGHT_KEY = 1,       //!< A right key is selected
        MIT_DOWN_KEY  = 2,       //!< A down key is selected
        MIT_UP_KEY    = 3,       //!< An up key is selected
        MIT_JUMP_ACTION_KEY = 4, //!< A jump key is being selected
        MIT_PAUSE_KEY = 5,       //!< A pause key is being selected,
        MIT_SOUND_VOLUME = 6,    //!< A sound volume is being selected
        MIT_MUSIC_VOLUME = 7,    //!< A music volume is being selected
        MIT_SAVE = 8,            //!< A save button is selected
        MIT_RESET = 9            //!< A reset button is selected
    };
    /*! An options screen
     */
    OptionsScreen();
    /*! An options screen
     */
    ~OptionsScreen();
    /*! Inits an options screen
        \param[in] game a game
        \param[in] main_renderer a renderer for main thread
        \param[in] inventory_renderer a renderer for inventory thread
     */
    void init(Game* game, sad::Renderer* main_renderer, sad::Renderer* inventory_renderer);
    /*! Inits for main renderer
     */
    void initForMainRenderer();
    /*! Inits for inventory renderer
     */
    void initForInventoryRenderer();
    /*! Moves to next item
     */
    void moveToNextItem();
    /*! Moves to previous item
     */
    void moveToPreviousItem();
    /*! Tries to start editing option or press button
     */
    void tryStartEditing();
    /*! Tries to perform editing action
        \param[in] ev event
     */
    void tryHandleEditing(const sad::input::KeyPressEvent& ev);
private:
/*! A state for thread
 */
struct StateForThread  // NOLINT(hicpp-special-member-functions, cppcoreguidelines-special-member-functions)
{
    /*! A renderer
     */
    sad::Renderer* Renderer;
    /*! Sound volume slider width
     */
    double SoundVoumeSliderWidth;
    /*! Music volume slider width
     */
    double MusicVolumeSliderWidth;
    /*! A blinking data
     */
    sad::animations::Blinking* Blinking;
    /*! An instance data
     */
    sad::animations::Instance* Instance;

    /*! A default constructor for thread
     */
    StateForThread();
    /*! Frees all data for thread
     */
    ~StateForThread();
};
    /*! Changes sound volume by difference that increments or decrement a screen
        \param[in] diff a difference
    */
    void changeSoundVolume(double diff) const;
    /*! Changes music volume by difference that increments or decrement a screen
        \param[in] diff a difference
     */
    void changeMusicVolume(double diff) const;
    /*! Sets width for slider value
        \param[in] renderer a renderer data
        \param[in] value a value data
        \param[in] sprite_name a sprite name
        \param[in] max_width a max width
     */
    static void setWidthForSliderValue(sad::Renderer* renderer, double value, const sad::String& sprite_name, double max_width);
    /*! Tries set keyboard option, according to value
        \param[in] key key value
        \param[in] key_option a key options
        \param[in] label_name a name for label
     */
    void trySetKeyboardOption(sad::KeyboardKey key, sad::KeyboardKey (game::Options::*key_option), const sad::String& label_name);
    /*! Returns true if key is not maching any key from config, except current selected key
        \param[in] key keyboard key
        \return true if key is not matching
     */
    bool isKeyNotMatchingAnyFromConfigExceptCurrent(sad::KeyboardKey key) const;
    /*! Exists editing state
     */
    void exitEditingState();
    /*! Reinitializes label values, according to config
     */
    void initAccordingToConfig();
    /*! Reinitializes label values, according to config
        \param[in] state a state for thread
     */
    void initAccordingToConfig(OptionsScreen::StateForThread& state) const;
    /*! Starts editing item, moving to specific data
     */
    void startEditingItem();
    /*! Moves to item for difference 
        \param[in] difference a difference between old value and new value
     */
    void moveToItemByDifference(int difference);
    /*! Inits screen for renderer
        \param[in] state a state value
     */
    void initForRenderer(OptionsScreen::StateForThread& state);
    /*! Sets label value, updating cell
        \param[in] renderer a renderer
        \param[in] label_name a name for label
        \param[in] value a value
     */
    static void setLabelValue(sad::Renderer* renderer, const sad::String& label_name, const sad::String& value);
    /*! Returns grid for renderer
        \param[in] renderer a renderer
        \return grid
     */
    static sad::layouts::Grid* getGridForRenderer(sad::Renderer* renderer);
    /*! A label underlays
     */
    sad::Vector<sad::String> m_label_underlays;
    /*! The parts that should blink when edited
     */
    sad::Vector<sad::String> m_blinking_parts;
    /*! A game
     */
    Game* m_game;
    /*! A main thread state
     */
    OptionsScreen::StateForThread m_main_state;
    /*! An inventory thread state
    */
    OptionsScreen::StateForThread m_inventory_state;
    /*! Is editing a menu item options
     */
    bool m_editing;
    /*! Whether event has been already handled, to ensure that several key events, based on same key won't work
     */
    bool m_already_handled;
    /*! A current menu item
     */
    int m_current_menu_item;
};
