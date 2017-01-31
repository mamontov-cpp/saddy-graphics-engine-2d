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
#define _INC_STDIO
#include "3rdparty/tpunit++/tpunit++.hpp"
#pragma warning(pop)

inline bool toIntAndEqual(int a, int b)
{
    return a == b;
}

struct ConvertTest : tpunit::TestFixture
{
public:
    ConvertTest() : tpunit::TestFixture(
       TEST(ConvertTest::testConvertNumeric)
    ) {}


    /*! Tests converting object to numeric
     */
    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
    void testConvertNumeric()
    {
        typedef ::dukpp03::PushValue<sad::db::Variant, sad::dukpp03::BasicContext> PushValue;
        // Sanity checks
        {
            sad::dukpp03::Context ctx;
            sad::db::Variant v(sad::String("22"));
            PushValue::perform(&ctx, v);
            ASSERT_TRUE(DUKPP03_FROM_STACK(std::string, &ctx, -1).exists())
            ASSERT_TRUE(DUKPP03_FROM_STACK(std::string, &ctx, -1).value() == "22");
        }

        {
            sad::dukpp03::Context ctx;
            sad::db::Variant v(std::string("22"));
            PushValue::perform(&ctx, v);
            ASSERT_TRUE(DUKPP03_FROM_STACK(sad::String, &ctx, -1).exists())
            ASSERT_TRUE(DUKPP03_FROM_STACK(sad::String, &ctx, -1).value() == "22");
        }

        // Sanity checks
        {
            sad::dukpp03::Context ctx;
            sad::db::Variant v(2);
            PushValue::perform(&ctx, v);
            ASSERT_TRUE(DUKPP03_FROM_STACK(int, &ctx, -1).exists())
            ASSERT_TRUE(DUKPP03_FROM_STACK(int, &ctx, -1).value() == 2);
        }

        {
            sad::dukpp03::Context ctx;
            sad::db::Variant v(2);
            PushValue::perform(&ctx, v);
            ASSERT_TRUE(DUKPP03_FROM_STACK(sad::Object*, &ctx, -1).exists() == false)
        }

        {
            sad::dukpp03::Context ctx;
            sad::db::Variant v(2);
            PushValue::perform(&ctx, v);
            ASSERT_TRUE(DUKPP03_FROM_STACK(long, &ctx, -1).exists())
            ASSERT_TRUE(DUKPP03_FROM_STACK(long, &ctx, -1).value() == 2);
        }

#define CONVERSION_TEST(X, Y)                                                                         \
        {                                                                                             \
           std::cout << "Starting test: " << #X  << " -> " << #Y << "\n";                             \
           sad::dukpp03::Context ctx;                                                                 \
           sad::db::Variant v((DUKPP03_TYPE(X))2);                                                    \
           PushValue::perform(&ctx, v);                                                               \
           ASSERT_TRUE(DUKPP03_FROM_STACK(DUKPP03_TYPE(Y), &ctx, -1).exists())                        \
           ASSERT_TRUE(toIntAndEqual((int)DUKPP03_FROM_STACK(DUKPP03_TYPE(Y), &ctx, -1).value(), 2)); \
           std::cout << "Successfully passsed test: " << #X  << " -> " << #Y << "\n";                 \
        }

#define CONVERSION_TEST_PAIR(X, Y) CONVERSION_TEST(X, Y) CONVERSION_TEST(Y, X)

        CONVERSION_TEST(char, char);
        CONVERSION_TEST(unsigned char, unsigned char);
        CONVERSION_TEST_PAIR(char, unsigned char);
        

        CONVERSION_TEST_PAIR(float, double);
        CONVERSION_TEST_PAIR(float, long double);        
        CONVERSION_TEST_PAIR(double, long double);

        CONVERSION_TEST(float, float);
        CONVERSION_TEST_PAIR(float, short);
        CONVERSION_TEST_PAIR(float, unsigned short);
        CONVERSION_TEST_PAIR(float, int);
        CONVERSION_TEST_PAIR(float, unsigned int);
        CONVERSION_TEST_PAIR(float, long);
        CONVERSION_TEST_PAIR(float, unsigned long);
        CONVERSION_TEST_PAIR(float, long long);
        CONVERSION_TEST_PAIR(float, unsigned long long);

        CONVERSION_TEST(double, double);
        CONVERSION_TEST_PAIR(double, short);
        CONVERSION_TEST_PAIR(double, unsigned short);
        CONVERSION_TEST_PAIR(double, int);
        CONVERSION_TEST_PAIR(double, unsigned int);
        CONVERSION_TEST_PAIR(double, long);
        CONVERSION_TEST_PAIR(double, unsigned long);
        CONVERSION_TEST_PAIR(double, long long);
        CONVERSION_TEST_PAIR(double, unsigned long long);

        CONVERSION_TEST(long double, long double);
        CONVERSION_TEST_PAIR(long double, short);
        CONVERSION_TEST_PAIR(long double, unsigned short);
        CONVERSION_TEST_PAIR(long double, int);
        CONVERSION_TEST_PAIR(long double, unsigned int);
        CONVERSION_TEST_PAIR(long double, long);
        CONVERSION_TEST_PAIR(long double, unsigned long);
        CONVERSION_TEST_PAIR(long double, long long);
        CONVERSION_TEST_PAIR(long double, unsigned long long);
        CONVERSION_TEST_PAIR(long double, double);
        CONVERSION_TEST_PAIR(long double, float);

        CONVERSION_TEST(short, short);
        CONVERSION_TEST_PAIR(short, unsigned short);
        CONVERSION_TEST_PAIR(short, int);
        CONVERSION_TEST_PAIR(short, unsigned int);
        CONVERSION_TEST_PAIR(short, long);
        CONVERSION_TEST_PAIR(short, unsigned long);
        CONVERSION_TEST_PAIR(short, long long);
        CONVERSION_TEST_PAIR(short, unsigned long long);

        CONVERSION_TEST(unsigned short, unsigned short);
        CONVERSION_TEST_PAIR(unsigned short, int);
        CONVERSION_TEST_PAIR(unsigned short, unsigned int);
        CONVERSION_TEST_PAIR(unsigned short, long);
        CONVERSION_TEST_PAIR(unsigned short, unsigned long);
        CONVERSION_TEST_PAIR(unsigned short, long long);
        CONVERSION_TEST_PAIR(unsigned short, unsigned long long);

        CONVERSION_TEST(int, int);
        CONVERSION_TEST_PAIR(int, unsigned int);
        CONVERSION_TEST_PAIR(int, long);
        CONVERSION_TEST_PAIR(int, unsigned long);
        CONVERSION_TEST_PAIR(int, long long);
        CONVERSION_TEST_PAIR(int, unsigned long long);

        CONVERSION_TEST(unsigned int, unsigned int);
        CONVERSION_TEST_PAIR(unsigned int, long);
        CONVERSION_TEST_PAIR(unsigned int, unsigned long);
        CONVERSION_TEST_PAIR(unsigned int, long long);
        CONVERSION_TEST_PAIR(unsigned int, unsigned long long);

        CONVERSION_TEST(long, long);
        CONVERSION_TEST_PAIR(long, unsigned long);
        CONVERSION_TEST_PAIR(long, long long);
        CONVERSION_TEST_PAIR(long, unsigned long long);

        CONVERSION_TEST(unsigned long, unsigned long);
        CONVERSION_TEST_PAIR(unsigned long, long long);
        CONVERSION_TEST_PAIR(unsigned long, unsigned long long);

        CONVERSION_TEST(long long, long long);
        CONVERSION_TEST_PAIR(long long, unsigned long long);

        CONVERSION_TEST(unsigned long long, unsigned long long);
#undef CONVERSION_TEST_PAIR
#undef CONVERSION_TEST
    }

} _convert_test;