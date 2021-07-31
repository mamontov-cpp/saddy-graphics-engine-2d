#include "options.h"

#include <spitjson.h>
#include <slurpjson.h>

game::Options::Options() : LeftKey(sad::KeyboardKey::KeyLeft),
RightKey(sad::KeyboardKey::KeyRight),
DownKey(sad::KeyboardKey::KeyDown),
UpKey(sad::KeyboardKey::KeyUp),
JumpActionKey(sad::KeyboardKey::Space),
PauseKey(sad::KeyboardKey::P),
SoundVolume(1.0),
MusicVolume(1.0)
{
    
}


void game::Options::save(sad::Renderer* renderer) const
{
    picojson::value::object object;
    picojson::value value(object);
    value.insert("left", picojson::value(static_cast<double>(LeftKey)));
    value.insert("right", picojson::value(static_cast<double>(RightKey)));
    value.insert("down", picojson::value(static_cast<double>(DownKey)));
    value.insert("up", picojson::value(static_cast<double>(UpKey)));
    value.insert("jump_action", picojson::value(static_cast<double>(JumpActionKey)));
    value.insert("pause", picojson::value(static_cast<double>(PauseKey)));
    value.insert("sound_volume", picojson::value(SoundVolume));
    value.insert("music_volume", picojson::value(MusicVolume));

    sad::spitJson("examples/multithreading/options.json", value, renderer);
}


void game::Options::load(sad::Renderer* renderer)
{
    sad::Maybe<picojson::value> maybe_value = sad::slurpJson("examples/multithreading/options.json", renderer);
    if (maybe_value.exists())
    {
        picojson::value& val = maybe_value.mutableValue();
        if (val.is<picojson::value::object>())
        {
            picojson::value::object obj = val.get<picojson::value::object>();
            game::Options::tryLoadFromProperty(&obj, &(this->LeftKey) , "left" , sad::KeyboardKey::KeyLeft);
            game::Options::tryLoadFromProperty(&obj, &(this->RightKey), "right", sad::KeyboardKey::KeyRight);
            game::Options::tryLoadFromProperty(&obj, &(this->DownKey) , "down" , sad::KeyboardKey::KeyDown);
            game::Options::tryLoadFromProperty(&obj, &(this->UpKey)   , "up"   , sad::KeyboardKey::KeyUp);
            game::Options::tryLoadFromProperty(&obj, &(this->JumpActionKey), "jump_action", sad::KeyboardKey::Space);
            game::Options::tryLoadFromProperty(&obj, &(this->PauseKey)     , "pause"      , sad::KeyboardKey::P);

            game::Options::tryLoadFromProperty(&obj, &(this->SoundVolume), "sound_volume");
            game::Options::tryLoadFromProperty(&obj, &(this->MusicVolume), "music_volume");
        }
    }
}

void game::Options::tryLoadFromProperty(void* obj, sad::KeyboardKey* out_key, const std::string& property_name, sad::KeyboardKey default_value)
{
    picojson::value::object* object = reinterpret_cast<picojson::value::object*>(obj);
    if (object->find(property_name) != object->end())
    {
        *out_key = sad::defaultKeyIfNotValid(static_cast<sad::KeyboardKey>(static_cast<int>((*object)[property_name].get<double>())), default_value);
    }
}

void game::Options::tryLoadFromProperty(void* obj, double* out_value, const std::string& property_name)
{
    picojson::value::object* object = reinterpret_cast<picojson::value::object*>(obj);
    if (object->find(property_name) != object->end())
    {
        picojson::value val = (*object)[property_name];
        if (val.is<double>())
        {
            double real_value = val.get<double>();
            if (real_value < 0)
            {
                real_value = 0.0;
            }

            if (real_value > 1.0)
            {
                real_value = 1.0;
            }

            *out_value = real_value;
        }
        else
        {
            *out_value = 1.0;
        }
    }
}