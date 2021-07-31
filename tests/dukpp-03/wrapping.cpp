#pragma warning(push)
#pragma warning(disable: 4273)
#pragma warning(disable: 4351)
// ReSharper disable once CppUnusedIncludeDirective
#include <cstdio>
#include "dukpp-03/context.h"
#include "sadpoint.h"
#include "db/save.h"
#include "db/load.h"
#include "fuzzyequal.h"
#include "sprite2d.h"
#include "animations/animationscolor.h"
#define _INC_STDIO
#include "3rdparty/tpunit++/tpunit++.hpp"
#pragma warning(pop)

sad::db::Object* makeAnimationAsSadDbObject()
{
    sad::animations::Animation* result = new sad::animations::Color();
    result->MajorId = 15;
    return result;
}

sad::animations::Color* makeAnimation()
{
    sad::animations::Color* result = new sad::animations::Color();
    result->MajorId = 15;
    return result;
}

sad::animations::Animation* object = nullptr;

sad::db::Variant makeAnimationAsVariant()
{
    sad::animations::Animation* result = new sad::animations::Color();
    result->MajorId = 15;
    object = result;
    return sad::db::Variant(static_cast<sad::db::Object*>(result));
}

sad::db::Variant makeAnimationAsVariantOfAnimation()
{
    sad::animations::Color* result = new sad::animations::Color();
    result->MajorId = 15;
    object = result;
    return sad::db::Variant(result);
}

class Inheritance1: public sad::Object
{
    SAD_OBJECT
public:
    Inheritance1() 
    {

    }
    ~Inheritance1() 
    {
    
    }
    int exec() 
    { 
        return 2; 
    }
};

DECLARE_SOBJ(Inheritance1);
//DECLARE_TYPE_AS_SAD_OBJECT(Inheritance1);


class Inheritance2: public Inheritance1
{
    SAD_OBJECT
public:
    Inheritance2() 
    {

    }
    ~Inheritance2() 
    {
    
    }
    int execMe() 
    { 
        return 1; 
    }
};

DECLARE_SOBJ_INHERITANCE(Inheritance2,Inheritance1)
//DECLARE_TYPE_AS_SAD_OBJECT(Inheritance2);

struct WrappingTest : tpunit::TestFixture
{
public:
    WrappingTest() : tpunit::TestFixture(
       TEST(WrappingTest::testWrapSadDbObjectWithoutBinding),
       TEST(WrappingTest::testWrapSadDbObjectWithoutBindingFromVariant),
       TEST(WrappingTest::testWrapSadDbObjectWithBinding),
       TEST(WrappingTest::testWrapSadDbObjectWithBindingFromVariant),
       TEST(WrappingTest::testWrapObjectWithoutBinding),
       TEST(WrappingTest::testWrapObjectWithoutBindingFromVariant),
       TEST(WrappingTest::testWrapObjectWithBinding),
       TEST(WrappingTest::testWrapObjectWithBindingFromVariant),
       TEST(WrappingTest::testClone),
       TEST(WrappingTest::testInheritance)  
    ) {}

    /*! Tests automatical wrapping for sad::db::Object without binding
     */
    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
    void testWrapSadDbObjectWithoutBinding()
    {
        std::string error;  

        sad::dukpp03::Context ctx(true);
        ctx.bind("make", makeAnimationAsSadDbObject);
        bool eval_result = ctx.eval(" var f = make(); f.majorid = 22; f.majorid  ", false, &error);
        ASSERT_TRUE( eval_result );
        ::dukpp03::Maybe<int> result = DUKPP03_FROM_STACK(int, &ctx, -1);
        ASSERT_TRUE( result.exists() );
        ASSERT_TRUE( result.value() == 22 );
    }

    /*! Tests automatical wrapping for sad::db::Object without binding with functions, which returns it as variant
     */
    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
    void testWrapSadDbObjectWithoutBindingFromVariant()
    {
        std::string error;  

        sad::dukpp03::Context ctx(true);
        ctx.bind("make", makeAnimationAsVariant);
        bool eval_result = ctx.eval(" var f = make(); f.majorid = 22; f.majorid  ", false, &error);
        ASSERT_TRUE( eval_result );
        ::dukpp03::Maybe<int> result = DUKPP03_FROM_STACK(int, &ctx, -1);
        ASSERT_TRUE( result.exists() );
        ASSERT_TRUE( result.value() == 22 ); 

        delete object;
    }

    /*! Tests automatical wrapping for sad::db::Object with bindings
     */
    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
    void testWrapSadDbObjectWithBinding()
    {
        std::string error;  

        sad::dukpp03::Context ctx(true);
        ctx.bind("make", makeAnimationAsSadDbObject);

        sad::dukpp03::ClassBinding* b = new sad::dukpp03::ClassBinding();
        b->registerAsObjectWithSchema<sad::animations::Color>();
        b->addObjectConstructor<sad::animations::Color>("SadAnimationColor");
        b->addMethod("getLooped", sad::dukpp03::bind_method::from(&sad::animations::Animation::looped));
        
        ctx.addClassBinding("sad::animations::Color", b);
        
        bool eval_result = ctx.eval(" var f = make(); var m = new SadAnimationColor(); f.majorid = 22; m.majorid = 22; f.majorid + m.majorid + f.getLooped()  ", false, &error);        
        ASSERT_TRUE( eval_result );
        ::dukpp03::Maybe<int> result = DUKPP03_FROM_STACK(int, &ctx, -1);
        ASSERT_TRUE( result.exists() );
        ASSERT_TRUE( result.value() == 44 );
    }

    /*! Tests automatical wrapping for sad::db::Object with binding with functions, which returns it as variant
     */
    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
    void testWrapSadDbObjectWithBindingFromVariant()
    {
        std::string error;  

        sad::dukpp03::Context ctx(true);
        ctx.bind("make", makeAnimationAsVariant);

        sad::dukpp03::ClassBinding* b = new sad::dukpp03::ClassBinding();
        b->registerAsObjectWithSchema<sad::animations::Color>();
        b->addObjectConstructor<sad::animations::Color>("SadAnimationColor");
        b->addMethod("getLooped", sad::dukpp03::bind_method::from(&sad::animations::Animation::looped));
        
        ctx.addClassBinding("sad::animations::Color", b);
        
        bool eval_result = ctx.eval(" var f = make(); var m = new SadAnimationColor(); f.majorid = 22; m.majorid = 22; f.majorid + m.majorid + f.getLooped()  ", false, &error);        
        ASSERT_TRUE( eval_result );
        ::dukpp03::Maybe<int> result = DUKPP03_FROM_STACK(int, &ctx, -1);
        ASSERT_TRUE( result.exists() );
        ASSERT_TRUE( result.value() == 44 );  

        delete object;
    }
    
    /*! Tests automatical wrapping for object without binding
     */
    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
    void testWrapObjectWithoutBinding()
    {
        std::string error;  

        sad::dukpp03::Context ctx(true);
        ctx.bind("make", makeAnimation);
        bool eval_result = ctx.eval(" var f = make(); f.majorid = 22; f.majorid  ", false, &error);
        ASSERT_TRUE( eval_result );
        ::dukpp03::Maybe<int> result = DUKPP03_FROM_STACK(int, &ctx, -1);
        ASSERT_TRUE( result.exists() );
        ASSERT_TRUE( result.value() == 22 );        
    }

    /*! Tests automatical wrapping for object without binding with functions, which returns it as variant
     */
    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
    void testWrapObjectWithoutBindingFromVariant()
    {
        std::string error;  

        sad::dukpp03::Context ctx(true);
        ctx.bind("make", makeAnimationAsVariantOfAnimation);
        bool eval_result = ctx.eval(" var f = make(); f.majorid = 22; f.majorid  ", false, &error);
        ASSERT_TRUE( eval_result );
        ::dukpp03::Maybe<int> result = DUKPP03_FROM_STACK(int, &ctx, -1);
        ASSERT_TRUE( result.exists() );
        ASSERT_TRUE( result.value() == 22 ); 

        delete object;        
    }

    /*! Tests automatical wrapping for object with bindings
     */
    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
    void testWrapObjectWithBinding()
    {
        std::string error;  

        sad::dukpp03::Context ctx(true);
        ctx.bind("make", makeAnimation);

        sad::dukpp03::ClassBinding* b = new sad::dukpp03::ClassBinding();
        b->registerAsObjectWithSchema<sad::animations::Color>();
        b->addObjectConstructor<sad::animations::Color>("SadAnimationColor");
        b->addMethod("getLooped", sad::dukpp03::bind_method::from(&sad::animations::Animation::looped));
        
        ctx.addClassBinding("sad::animations::Color", b);
        
        bool eval_result = ctx.eval(" var f = make(); var m = new SadAnimationColor(); f.majorid = 22; m.majorid = 22; f.majorid + m.majorid + f.getLooped()  ", false, &error);        
        ASSERT_TRUE( eval_result );
        ::dukpp03::Maybe<int> result = DUKPP03_FROM_STACK(int, &ctx, -1);
        ASSERT_TRUE( result.exists() );
        ASSERT_TRUE( result.value() == 44 );        
    }

    /*! Tests automatical wrapping for object with binding with functions, which returns it as variant
     */
    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
    void testWrapObjectWithBindingFromVariant()
    {
        std::string error;  

        sad::dukpp03::Context ctx(true);
        ctx.bind("make", makeAnimationAsVariantOfAnimation);

        sad::dukpp03::ClassBinding* b = new sad::dukpp03::ClassBinding();
        b->registerAsObjectWithSchema<sad::animations::Color>();
        b->addObjectConstructor<sad::animations::Color>("SadAnimationColor");
        b->addMethod("getLooped", sad::dukpp03::bind_method::from(&sad::animations::Animation::looped));
        
        ctx.addClassBinding("sad::animations::Color", b);
        
        bool eval_result = ctx.eval(" var f = make(); var m = new SadAnimationColor(); f.majorid = 22; m.majorid = 22; f.majorid + m.majorid + f.getLooped()  ", false, &error);        
        ASSERT_TRUE( eval_result );
        ::dukpp03::Maybe<int> result = DUKPP03_FROM_STACK(int, &ctx, -1);
        ASSERT_TRUE( result.exists() );
        ASSERT_TRUE( result.value() == 44 );  

        delete object;        
    }

    /*! Tests simple cloning method
     */
    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
    void testClone()
    {
        std::string error;  
        
        sad::dukpp03::Context ctx(true);
        
        sad::dukpp03::ClassBinding* b = new sad::dukpp03::ClassBinding();
        b->registerAsObjectWithSchema<sad::animations::Color>();
        b->addCloneObjectMethodFor<sad::animations::Color>();
        b->addObjectConstructor<sad::animations::Color>("SadAnimationColor");
        
        ctx.addClassBinding("sad::animations::Color", b);
        
        bool eval_result = ctx.eval(" var m = new  SadAnimationColor(); m.time = 222; var b = m.clone(); m.time = 234; b.time ", false, &error);        
        ASSERT_TRUE( eval_result );
        ::dukpp03::Maybe<int> result = DUKPP03_FROM_STACK(int, &ctx, -1);
        ASSERT_TRUE( result.exists() );
        ASSERT_TRUE( result.value() == 222 );        
    }

    /*! Tests simple inheritance
     */
    void testInheritance()
    {
        std::string error;  
        
        sad::dukpp03::Context ctx(true);
        
        sad::dukpp03::ClassBinding* b1 = new sad::dukpp03::ClassBinding();
        b1->addObjectConstructor<Inheritance1>("Inheritance1");
        b1->addMethod("exec", sad::dukpp03::bind_method::from(&Inheritance1::exec));
        
        ctx.addClassBinding("Inheritance1", b1);

        sad::dukpp03::ClassBinding* b2 = new sad::dukpp03::ClassBinding();
        b2->addObjectConstructor<Inheritance2>("Inheritance2");
        b2->addMethod("execMe", sad::dukpp03::bind_method::from(&Inheritance2::execMe));
        b2->addMethod("exec", sad::dukpp03::bind_method::from(&Inheritance1::exec));
        
        ctx.addClassBinding("Inheritance2", b2);
                
        bool eval_result = ctx.eval(" var i = new Inheritance2(); i.exec() + i.execMe() ", false, &error);
        if (!eval_result)
        {
            std::cout << error << "\n";
        }
        ASSERT_TRUE( eval_result );
        ::dukpp03::Maybe<int> result = DUKPP03_FROM_STACK(int, &ctx, -1);
        ASSERT_TRUE( result.exists() );
        ASSERT_TRUE( result.value() == 3 ); 
    }
} _wrapping_test;