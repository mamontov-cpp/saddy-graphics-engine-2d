/*! \file optionsscreen.h
 
    An options screen
 */
#pragma once
#include <sadvector.h>
#include <sadstring.h>


namespace sad
{
class Renderer;
}

/*! An options screen data
 */
class OptionsScreen
{
public:
    /*! An options screen
     */
    OptionsScreen();
    /*! Inits an options screen
        \param[in] main_renderer a renderer for main thread
        \param[in] inventory_renderer a renderer for inventory thread
     */
    void init(sad::Renderer* main_renderer, sad::Renderer* inventory_renderer);
    /*! Inits for main renderer
     */
    void initForMainRenderer();
    /*! Inits for inventory renderer
     */
    void initForInventoryRenderer();
private:
    /*! Inits screen for renderer
        \param[in] renderer a renderer
     */
    void initForRenderer(sad::Renderer* renderer);
    /*! A label underlays
     */
    sad::Vector<sad::String> m_label_underlays;
    /*! A renderer for main thread
     */
    sad::Renderer* m_main_renderer;
    /*! A renderer for inventory thread
     */
    sad::Renderer* m_inventory_renderer;
};
