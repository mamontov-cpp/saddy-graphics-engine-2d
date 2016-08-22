#pragma warning(push)
#pragma warning(disable: 4273)
#pragma warning(disable: 4351)
#include <cstdio>


#include "resource/link.h"
#include "resource/resourcefile.h"
#include "resource/tree.h"

#include "util/free.h"

#include "renderer.h"

#include "fuzzyequal.h"

#include "animations/easing/easingfunction.h"

#include "animations/animationsblinking.h"
#include "animations/animationscamerarotation.h"
#include "animations/animationscamerashaking.h"
#include "animations/animationscolor.h"
#include "animations/animationsfontlist.h"
#include "animations/animationsfontsize.h"
#include "animations/animationsoptionlist.h"
#include "animations/animationsparallel.h"
#include "animations/animationsresize.h"
#include "animations/animationsrotate.h"
#include "animations/animationssequential.h"
#include "animations/animationssimplemovement.h"
#include "animations/animationstexturecoordinatescontinuous.h"
#include "animations/animationstexturecoordinateslist.h"
#include "animations/animationstyping.h"
#include "animations/animationswaymoving.h"

#define _INC_STDIO
#include "3rdparty/tpunit++/tpunit++.hpp"
#pragma warning(pop)

/*!
 * Tests sad::resource::TextureAtlasFile
 */
struct SadAnimationFileTest : tpunit::TestFixture
{
 public:
   SadAnimationFileTest() : tpunit::TestFixture(
       TEST(SadAnimationFileTest::testCommon)
   ) {}

    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
   void testCommon()
   {
       sad::Renderer r;
       sad::resource::Tree tree;
       tree.setStoreLinks(true);
       tree.setRenderer(&r);

       sad::Vector<sad::resource::Error *> errors = tree.loadFromString(
           "["
                "{"
                    "\"type\"   : \"sad::animations::File\","
                    "\"filename\": \"tests/animations/test_easing.json\""
                "}"
            "]"
        );
       int count = errors.size();
       sad::util::free(errors);
       ASSERT_TRUE(count == 0);

#define TEST_EASING_LOADING(TYPE, VARIABLE_NAME)    \
    sad::animations:: ##TYPE * VARIABLE_NAME = tree.get<sad::animations::  ##TYPE >(#VARIABLE_NAME);  \
    ASSERT_TRUE( (VARIABLE_NAME)->easing()->functionTypeAsUnsignedInt() == 3);  \
    ASSERT_TRUE( sad::is_fuzzy_equal((VARIABLE_NAME)->easing()->overshootAmplitude(), 2)); \
    ASSERT_TRUE( sad::is_fuzzy_equal((VARIABLE_NAME)->easing()->period(), 4));  

       TEST_EASING_LOADING(Blinking, blinking);
       TEST_EASING_LOADING(CameraRotation, camera_rotation);
       TEST_EASING_LOADING(CameraShaking, camera_shaking);
       TEST_EASING_LOADING(Color, color);
       TEST_EASING_LOADING(FontList, font_list);
       TEST_EASING_LOADING(FontSize, font_size);
       TEST_EASING_LOADING(OptionList, option_list);
       TEST_EASING_LOADING(Parallel, parallel);
       TEST_EASING_LOADING(Resize, resize);
       TEST_EASING_LOADING(Rotate, rotate);
       TEST_EASING_LOADING(Sequential, sequential);
       TEST_EASING_LOADING(SimpleMovement, simple_movement);
       TEST_EASING_LOADING(TextureCoordinatesContinuous, texture_coordinates_continous);
       TEST_EASING_LOADING(TextureCoordinatesList, texture_coordinates_list);
       TEST_EASING_LOADING(Typing, typing);
       TEST_EASING_LOADING(WayMoving, way_moving);

   }

} _sad_animation_file_test;