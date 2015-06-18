#include "slurpjson.h"

#include "3rdparty/picojson/picojson.h"

sad::Maybe<picojson::value> sad::slurpJson(const sad::String fileName, sad::Renderer* renderer)
{
    sad::Maybe<sad::String> maybetext = sad::slurp(fileName, renderer);
    sad::Maybe<picojson::value> result;
    if (maybetext.exists())
    {
        if (maybetext.value().consistsOfWhitespaceCharacters() == false)
        {
            picojson::value rootvalue = picojson::parse_string(maybetext.value());
            if (picojson::get_last_error().size() == 0)
            {
                result.setValue(rootvalue);                
            }
        }
    }
    return result;
}
