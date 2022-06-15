#pragma warning(push)
#pragma warning(disable: 4273)
#pragma warning(disable: 4351)
#include <cstdio>
#include <refcountable.h>
#define _INC_STDIO
#include "3rdparty/tpunit++/tpunit++.hpp"
#pragma warning(pop)

struct Object : public sad::RefCountable
{
    Object() : A(5)
    {
        printf("Constructed\n");
    }

    Object(int o) : A(o)
    {
        printf("Constructed\n");
    }

    Object(Object& a) = delete;
    Object& operator=(Object& a) = delete;


    ~Object()
    {
        printf("Free");
    }

    int A;
};


struct Holder
{
    sad::RCUPtr<Object> Item;
};


struct RefCountableTest : tpunit::TestFixture
{
public:

    RefCountableTest() : tpunit::TestFixture(
        TEST(RefCountableTest::test)
    ) {}

    void test()
    {
        Object* o = nullptr;
        {
            printf("Started\n");
            Holder h;
            h.Item = sad::new_rcu<Object>(10);
            o = h.Item.get();
            o->addRef();
            printf("%d\n", o->A);
            ASSERT_TRUE(o->A == 10)
        }

        printf("%d\n", o->A);
        ASSERT_TRUE(o->A == 10)
        o->delRef();
        printf("Finished\n");
    }
} _ref_countable_tes;
