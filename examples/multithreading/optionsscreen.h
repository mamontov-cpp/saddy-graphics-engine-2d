/*! \file optionsscreen.h
 
    An options screen
 */
#pragma once
#include <sadvector.h>
#include <sadstring.h>

#include <layouts/grid.h>

#include <animations/animationsblinking.h>
#include <animations/animationsinstance.h>


namespace sad
{
class Renderer;
}

class Game;

/*! An options screen data
 */
class OptionsScreen  // NOLINT(hicpp-special-member-functions, cppcoreguidelines-special-member-functions)
{
public:
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
    /*! A current menu item
     */
    int m_current_menu_item;
};
