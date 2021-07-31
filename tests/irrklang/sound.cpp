#pragma warning(push)
#pragma warning(disable: 4273)
#pragma warning(disable: 4351)
#include <cstdio>
#include "irrklang/sound.h"
#include "resource/tree.h"
#include "util/free.h"
#include "db/save.h"
#include "sadsleep.h"
#define _INC_STDIO
#include "3rdparty/tpunit++/tpunit++.hpp"
#pragma warning(pop)


struct SoundTest : tpunit::TestFixture
{
public:
    SoundTest() : tpunit::TestFixture(
       TEST(SoundTest::testTypename),
       TEST(SoundTest::testValid),
       TEST(SoundTest::testInvalid)
    ) {}
    
    void testTypename()
    {
        sad::db::TypeName<sad::irrklang::Sound>::init();
        sad::String result = sad::db::TypeName<sad::irrklang::Sound>::name();
        ASSERT_TRUE( result == "sad::irrklang::Sound" );
        bool v =  sad::db::TypeName<sad::irrklang::Sound>::isSadObject();
        ASSERT_TRUE( v );
    }

    void testValid()
    {
       sad::Renderer r;
       sad::resource::Tree tree;
       tree.setStoreLinks(true);
       tree.setRenderer(&r);
       tree.factory()->registerResource<sad::irrklang::Sound>();
       tree.factory()->registerDefaultFileTypeFor<sad::irrklang::Sound>();

       sad::Vector<sad::resource::Error *> errors = tree.loadFromFile("tests/sound.json");
        
       int count = errors.size();
       sad::util::free(errors);
       ASSERT_TRUE(count == 0);
       sad::resource::Resource* k = tree.root()->resource("sound");
       ASSERT_TRUE( k );
       sad::irrklang::Sound* f = static_cast<sad::irrklang::Sound*>(k);
       ASSERT_TRUE( f );
       sad::irrklang::Engine::eref()->play2D(f->s());
       sad::sleep(2000);
    }

    void testInvalid()
    {
       sad::Renderer r;
       sad::resource::Tree tree;
       tree.setStoreLinks(true);
       tree.setRenderer(&r);
       // In debug, sad::fretype::Factory fonts becomes in font
       tree.factory()->registerResource<sad::irrklang::Sound>();
       tree.factory()->registerDefaultFileTypeFor<sad::irrklang::Sound>();

       sad::Vector<sad::resource::Error *> errors = tree.loadFromFile("tests/nosound.json");
        
       int count = errors.size();
       sad::util::free(errors);
       ASSERT_TRUE(count != 0);
    }

} _sound_test;