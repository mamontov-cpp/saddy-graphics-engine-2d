#pragma warning(push)
#pragma warning(disable: 4273)
#pragma warning(disable: 4351)
#include <sadraiiobjectvector.h>
#include <sprite2d.h>
#include "3rdparty/tpunit++/tpunit++.hpp"
#pragma warning(pop)

/*!
 * Tests object logic
 */
struct sadVectorTest : tpunit::TestFixture
{
public:
    sadVectorTest() : tpunit::TestFixture(
        TEST(sadVectorTest::testBasic),
        TEST(sadVectorTest::testCopyConstructorSelf),
        TEST(sadVectorTest::testAssignVector),
        TEST(sadVectorTest::testAssignSelf),
        TEST(sadVectorTest::testStripNull)
    ) {}

    /*! Basic test
     */
     // ReSharper disable once CppMemberFunctionMayBeStatic
    void testBasic()
    {
        {
            sad::Sprite2D* a = new sad::Sprite2D();
            sad::RAIIObjectVector<sad::Sprite2D> item({ a });
            ASSERT_TRUE(a->refsCount() == 1)
        }
        {
            sad::Sprite2D* a = new sad::Sprite2D();
            sad::Vector<sad::Sprite2D*> sprites({ a });
            sad::RAIIObjectVector<sad::Sprite2D> item(sprites);
            ASSERT_TRUE(a->refsCount() == 1)
        }
        {
            sad::Sprite2D* a = new sad::Sprite2D();
            sad::Vector<sad::Sprite2D*> sprites({ a });
            sad::RAIIObjectVector<sad::Sprite2D> item(std::move(sprites));
            ASSERT_TRUE(a->refsCount() == 1)
        }
        {
            sad::Sprite2D* a = new sad::Sprite2D();
            sad::Sprite2D* b = new sad::Sprite2D();
            a->addRef();
            b->addRef();
            sad::Vector<sad::Sprite2D*> items{ a, b };
            sad::RAIIObjectVector<sad::Sprite2D> raii_items(items, 1);
            ASSERT_TRUE(a->refsCount() == 2)
            ASSERT_TRUE(b->refsCount() == 1)
            ASSERT_TRUE(raii_items.size() == 1)
            ASSERT_TRUE(raii_items[0] == a)
        }
    }

    /*! Copy constructors on self data
     */
     // ReSharper disable once CppMemberFunctionMayBeStatic
    void testCopyConstructorSelf()
    {
        {
            sad::Sprite2D* a = new sad::Sprite2D();
            sad::RAIIObjectVector<sad::Sprite2D> item1({ a });
            sad::RAIIObjectVector<sad::Sprite2D> item2(item1);  // NOLINT(performance-unnecessary-copy-initialization)
            ASSERT_TRUE(a->refsCount() == 2)
        }
        {
            sad::Sprite2D* a = new sad::Sprite2D();
            sad::RAIIObjectVector<sad::Sprite2D> item1({ a });
            sad::RAIIObjectVector<sad::Sprite2D> item2(std::move(item1));
            ASSERT_TRUE(a->refsCount() == 1)
        }
    }

    /*! Tests vector assign for various variants
     */
    // ReSharper disable once CppMemberFunctionMayBeStatic
    void testAssignVector()
    {
        // Assign from full to empty
        {
            sad::Sprite2D* a = new sad::Sprite2D();
            sad::Vector<sad::Sprite2D*> v({ a });
            // ReSharper disable once CppEntityAssignedButNoRead
            sad::RAIIObjectVector<sad::Sprite2D> item;
            // ReSharper disable once CppJoinDeclarationAndAssignment
            item = v;
            ASSERT_TRUE(a->refsCount() == 1)
        }
        {
            sad::Sprite2D* a = new sad::Sprite2D();
            sad::Vector<sad::Sprite2D*> v({ a });
            // ReSharper disable once CppEntityAssignedButNoRead
            sad::RAIIObjectVector<sad::Sprite2D> item;
            // ReSharper disable once CppJoinDeclarationAndAssignment
            item = std::move(v);
            ASSERT_TRUE(a->refsCount() == 1)
        }
        // Assign from empty to full
        {
            sad::Sprite2D* a = new sad::Sprite2D();
            a->addRef();
            // ReSharper disable once CppTooWideScope
            // ReSharper disable once CppEntityAssignedButNoRead
            sad::RAIIObjectVector<sad::Sprite2D> item{ a };
            {
                sad::Vector<sad::Sprite2D*> v;
                // ReSharper disable once CppJoinDeclarationAndAssignment
                item = v;
            }
            ASSERT_TRUE(a->refsCount() == 1)
            a->delRef();
        }
        {
            sad::Sprite2D* a = new sad::Sprite2D();
            a->addRef();
            // ReSharper disable once CppTooWideScope
            sad::RAIIObjectVector<sad::Sprite2D> item{ a };
            {
                sad::Vector<sad::Sprite2D*> v;
                // ReSharper disable once CppJoinDeclarationAndAssignment
                item = std::move(v);
            }
            ASSERT_TRUE(a->refsCount() == 1)
            a->delRef();
        }
        // Assign from different, check for same item
        {
            sad::Sprite2D* a = new sad::Sprite2D();
            sad::Sprite2D* b = new sad::Sprite2D();
            // ReSharper disable once CppTooWideScope
            sad::RAIIObjectVector<sad::Sprite2D> item{ a };
            {
                sad::Vector<sad::Sprite2D*> v{ a, b };
                item = v;
            }
            ASSERT_TRUE(a->refsCount() == 1)
            ASSERT_TRUE(b->refsCount() == 1)
        }
        {
            sad::Sprite2D* a = new sad::Sprite2D();
            sad::Sprite2D* b = new sad::Sprite2D();
            // ReSharper disable once CppTooWideScope
            sad::RAIIObjectVector<sad::Sprite2D> item{ a };
            {
                sad::Vector<sad::Sprite2D*> v{ a, b };
                item = std::move(v);
            }
            ASSERT_TRUE(a->refsCount() == 1)
            ASSERT_TRUE(b->refsCount() == 1)
        }
    }

    /*! Tests vector assign for RAII variants
     */
    // ReSharper disable once CppMemberFunctionMayBeStatic
    void testAssignSelf()
    {
        // Assign from full to empty
        {
            sad::Sprite2D* a = new sad::Sprite2D();
            sad::RAIIObjectVector<sad::Sprite2D> v({ a });
            // ReSharper disable once CppEntityAssignedButNoRead
            sad::RAIIObjectVector<sad::Sprite2D> item;
            // ReSharper disable once CppJoinDeclarationAndAssignment
            item = v;
            ASSERT_TRUE(a->refsCount() == 2)
        }
        {
            sad::Sprite2D* a = new sad::Sprite2D();
            sad::RAIIObjectVector<sad::Sprite2D> v({ a });
            // ReSharper disable once CppEntityAssignedButNoRead
            sad::RAIIObjectVector<sad::Sprite2D> item;
            // ReSharper disable once CppJoinDeclarationAndAssignment
            item = std::move(v);
            ASSERT_TRUE(a->refsCount() == 1)
        }
        // Assign from empty to full
        {
            sad::Sprite2D* a = new sad::Sprite2D();
            a->addRef();
            // ReSharper disable once CppTooWideScope
            // ReSharper disable once CppEntityAssignedButNoRead
            sad::RAIIObjectVector<sad::Sprite2D> item{ a };
            {
                sad::RAIIObjectVector<sad::Sprite2D> v;
                // ReSharper disable once CppJoinDeclarationAndAssignment
                item = v;
            }
            ASSERT_TRUE(a->refsCount() == 1)
            a->delRef();
        }
        {
            sad::Sprite2D* a = new sad::Sprite2D();
            a->addRef();
            // ReSharper disable once CppTooWideScope
            // ReSharper disable once CppEntityAssignedButNoRead
            sad::RAIIObjectVector<sad::Sprite2D> item{ a };
            {
                sad::RAIIObjectVector<sad::Sprite2D> v;
                // ReSharper disable once CppJoinDeclarationAndAssignment
                item = std::move(v);
            }
            ASSERT_TRUE(a->refsCount() == 1)
            a->delRef();
        }

        // Assign from different, check for same item
        {
            sad::Sprite2D* a = new sad::Sprite2D();
            sad::Sprite2D* b = new sad::Sprite2D();
            // ReSharper disable once CppTooWideScope
            sad::RAIIObjectVector<sad::Sprite2D> item{ a };
            {
                sad::RAIIObjectVector<sad::Sprite2D> v{ a, b };
                item = v;
            }
            ASSERT_TRUE(a->refsCount() == 1)
            ASSERT_TRUE(b->refsCount() == 1)
        }
        {
            sad::Sprite2D* a = new sad::Sprite2D();
            sad::Sprite2D* b = new sad::Sprite2D();
            // ReSharper disable once CppTooWideScope
            sad::RAIIObjectVector<sad::Sprite2D> item{ a };
            {
                sad::RAIIObjectVector<sad::Sprite2D> v{ a, b };
                item = std::move(v);
            }
            ASSERT_TRUE(a->refsCount() == 1)
            ASSERT_TRUE(b->refsCount() == 1)
        }
    }

    /*! Test for strip null
     */
     // ReSharper disable once CppMemberFunctionMayBeStatic
    void testStripNull()
    {
        sad::Sprite2D* a = new sad::Sprite2D();
        sad::Sprite2D* b = new sad::Sprite2D();
        // ReSharper disable once CppTooWideScope
        sad::RAIIObjectVector<sad::Sprite2D> item{ a, nullptr, nullptr, b, nullptr, nullptr };
        item.stripNull();
        ASSERT_TRUE(a->refsCount() == 1)
        ASSERT_TRUE(b->refsCount() == 1)
        ASSERT_TRUE(item.size() == 2)
        ASSERT_TRUE(item[0] == a)
        ASSERT_TRUE(item[1] == b)
    }

} sad_vector_test;  // NOLINT(misc-use-internal-linkage)


