/*! \file options.h
    
    An options structure for storing options for main game
 */
#pragma once
#include <keycodes.h>
#include <string>

namespace sad
{
class Renderer;    
}

namespace game
{

/*! An options for storing data
 */
struct Options
{
    /*! A key, used for moving left
     */
    sad::KeyboardKey LeftKey;
    /*! A key, used for moving right
     */
    sad::KeyboardKey RightKey;
    /*! A key, used for moving down
     */
    sad::KeyboardKey DownKey;
    /*! A key, used for moving up
     */
    sad::KeyboardKey UpKey;
    /*! A jump action key value
     */
    sad::KeyboardKey JumpActionKey;
    /*! A pause key
     */
    sad::KeyboardKey PauseKey;
    /*! A sound volume for options
     */
    double SoundVolume;
    /*! A music volume for options
     */
    double MusicVolume;

    /*! Constructs a default options
     */
    Options();
    /*! Saves an options to file
        \param[in] renderer a renderer for saving file
     */
    void save(sad::Renderer* renderer) const;
    /*! Loads an options from file
        \param[in] renderer a renderer for saving file
     */
    void load(sad::Renderer* renderer);

private:
    /*! Tries a load as value, defined in out_key from property, named key. If fails, sets it to default_value
        \param[in] obj object
        \param[out] out_key output key
        \param[in] property_name a property name
        \param[in] default_value a default value for options
     */
    static void tryLoadFromProperty(void* obj, sad::KeyboardKey* out_key, const std::string& property_name, sad::KeyboardKey default_value);
    /*! Tries a load as value, defined in out_key from property, named key.If fails, sets it to default_value of 1.0
        \param[in] obj object
        \param[out] out_value output value
        \param[in] property_name a property name
     */
    static void tryLoadFromProperty(void* obj, double* out_value, const std::string& property_name);
};

}

