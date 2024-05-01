#include "p2d/collisiontest.h"
#include "p2d/collides1d.h"
#include "fuzzyequal.h"
#include "opticksupport.h"

void sad::p2d::CollisionTest::init()
{
    PROFILER_EVENT;
    this->sad::p2d::CollisionMultiMethod<bool>::init();
    add(sad::p2d::CollisionTest::collidesRtoR);
    add(sad::p2d::CollisionTest::collidesRtoC);
    add(sad::p2d::CollisionTest::collidesRtoL);
    add(sad::p2d::CollisionTest::collidesCtoC);
    add(sad::p2d::CollisionTest::collidesCtoL);
    add(sad::p2d::CollisionTest::collidesLtoL);

    add(sad::p2d::CollisionTest::collidesBtoR);
    add(sad::p2d::CollisionTest::collidesBtoC);
    add(sad::p2d::CollisionTest::collidesBtoL);
    add(sad::p2d::CollisionTest::collidesBtoB);
}

// A function, which performs fast collision operations
static inline bool fast_collide(
                                sad::p2d::CollisionShape * s1, sad::p2d::CollisionShape * s2,
                                void (**callbacks)(sad::p2d::CollisionShape *, sad::p2d::CollisionShape *, sad::p2d::Axle &),
                                int  size
                                )
{
    PROFILER_EVENT;
    sad::p2d::Axle axle;
    bool collides = true;
    for(size_t i = 0; i < 4 && collides;i++)
    {
        callbacks[i](s1, s2, axle);
        sad::p2d::Cutter1D c1 = s1->project(axle);
        sad::p2d::Cutter1D c2 = s2->project(axle);
        bool axlecollides = sad::p2d::collides(c1, c2);
        collides = collides && axlecollides;
    }
    return collides;
}


// Defines compile-time generators for callbacks, for lazy axle-computation
template<typename _Class1, typename _Class2>
struct AxleFor
{
    template< const sad::p2d::Point & (_Class1::*p1)(int) const,
              int num1,
              int num2
            >
    static void p1callsi(
        sad::p2d::CollisionShape * s1, 
        sad::p2d::CollisionShape * s2, 
        sad::p2d::Axle & axle
    )
    {
        _Class1 * c1 = static_cast<_Class1 *>(s1);
        _Class2 * c2 = static_cast<_Class2 *>(s2);
        axle = sad::p2d::axle( (c1->*p1)(num1), (c1->*p1)(num2) );
    }

    template< const sad::Point2D & (_Class1::*p1)(int) const,
              int num1,
              int num2
            >
    static void hp1callsi(
        sad::p2d::CollisionShape * s1, 
        sad::p2d::CollisionShape * s2, 
        sad::p2d::Axle & axle
    )
    {
        _Class1 * c1 = static_cast<_Class1 *>(s1);
        _Class2 * c2 = static_cast<_Class2 *>(s2);
        axle = sad::p2d::axle( (c1->*p1)(num1), (c1->*p1)(num2) );
    }

    template< const sad::p2d::Point & (_Class2::*p1)(int) const,
              int num1,
              int num2
            >
    static void p2callsi(
        sad::p2d::CollisionShape * s1, 
        sad::p2d::CollisionShape * s2, 
        sad::p2d::Axle & axle
    )
    {
        _Class1 * c1 = static_cast<_Class1 *>(s1);
        _Class2 * c2 = static_cast<_Class2 *>(s2);
        axle = sad::p2d::axle( (c2->*p1)(num1), (c2->*p1)(num2) );
    }

    template< const sad::Point2D & (_Class2::*p1)(int) const,
              int num1,
              int num2
            >
    static void hp2callsi(
        sad::p2d::CollisionShape * s1, 
        sad::p2d::CollisionShape * s2, 
        sad::p2d::Axle & axle
    )
    {
        _Class1 * c1 = static_cast<_Class1 *>(s1);
        _Class2 * c2 = static_cast<_Class2 *>(s2);
        axle = sad::p2d::axle( (c2->*p1)(num1), (c2->*p1)(num2) );
    }

    template< const sad::p2d::Point & (_Class1::*p1)(int) const,
              int num1,
              const sad::p2d::Point & (_Class2::*p2)() const
            >
    static void p1ip2(
        sad::p2d::CollisionShape * s1, 
        sad::p2d::CollisionShape * s2, 
        sad::p2d::Axle & axle
    )
    {
        _Class1 * c1 = static_cast<_Class1 *>(s1);
        _Class2 * c2 = static_cast<_Class2 *>(s2);
        axle = sad::p2d::axle( (c1->*p1)(num1), (c2->*p2)() );
    }

    template< const sad::Point2D & (_Class1::*p1)(int) const,
              int num1,
              const sad::p2d::Point & (_Class2::*p2)() const
            >
    static void hp1ip2(
        sad::p2d::CollisionShape * s1, 
        sad::p2d::CollisionShape * s2, 
        sad::p2d::Axle & axle
    )
    {
        _Class1 * c1 = static_cast<_Class1 *>(s1);
        _Class2 * c2 = static_cast<_Class2 *>(s2);
        axle = sad::p2d::axle( (c1->*p1)(num1), (c2->*p2)() );
    }

    template<
              const sad::Point2D & (_Class2::*p1)() const,
              const sad::Point2D & (_Class2::*p2)() const			  
            >
    static void p2calls(
        sad::p2d::CollisionShape * s1, 
        sad::p2d::CollisionShape * s2, 
        sad::p2d::Axle & axle
    )
    {
        _Class1 * c1 = static_cast<_Class1 *>(s1);
        _Class2 * c2 = static_cast<_Class2 *>(s2);
        axle = sad::p2d::axle( (c2->*p1)(), (c2->*p2)() );
    }

    template<
              const sad::Point2D & (_Class2::*p1)() const,
              const sad::Point2D & (_Class2::*p2)() const			  
            >
    static void op2calls(
        sad::p2d::CollisionShape * s1, 
        sad::p2d::CollisionShape * s2, 
        sad::p2d::Axle & axle
    )
    {
        _Class1 * c1 = static_cast<_Class1 *>(s1);
        _Class2 * c2 = static_cast<_Class2 *>(s2);
        axle = sad::p2d::ortho(sad::p2d::axle( (c2->*p1)(), (c2->*p2)() ), sad::p2d::OrthoVectorIndex::OVI_DEG_90);
    }


    template<
              const sad::Point2D & (_Class2::*p1)() const,
              const sad::Point2D & (_Class2::*p2)() const			  
            >
    static void p1calls(
        sad::p2d::CollisionShape * s1, 
        sad::p2d::CollisionShape * s2, 
        sad::p2d::Axle & axle
    )
    {
        _Class1 * c1 = static_cast<_Class1 *>(s1);
        _Class2 * c2 = static_cast<_Class2 *>(s2);
        axle = sad::p2d::axle( (c1->*p1)(), (c1->*p2)() );
    }

    template<
              const sad::Point2D & (_Class2::*p1)() const,
              const sad::Point2D & (_Class2::*p2)() const			  
            >
    static void op1calls(
        sad::p2d::CollisionShape * s1, 
        sad::p2d::CollisionShape * s2, 
        sad::p2d::Axle & axle
    )
    {
        _Class1 * c1 = static_cast<_Class1 *>(s1);
        _Class2 * c2 = static_cast<_Class2 *>(s2);
        axle = sad::p2d::ortho(sad::p2d::axle( (c1->*p1)(), (c1->*p2)() ), sad::p2d::OrthoVectorIndex::OVI_DEG_90);
    }

    template<
              const sad::p2d::Point & (_Class1::*p1)() const,
              const sad::Point2D & (_Class2::*p2)() const			  
            >
    static void p1hp2(sad::p2d::CollisionShape * s1,
                      sad::p2d::CollisionShape * s2, 
                      sad::p2d::Axle & axle)
    {
        _Class1 * c1 = static_cast<_Class1 *>(s1);
        _Class2 * c2 = static_cast<_Class2 *>(s2);
        axle = sad::p2d::axle( (c1->*p1)(), (c2->*p2)() );
    }

    template<
              const sad::p2d::Point & (_Class1::*p1)() const,
              const sad::Point2D & (_Class2::*p2)() const			  
            >
    static void op1hp2(sad::p2d::CollisionShape * s1, 
                       sad::p2d::CollisionShape * s2, 
                       sad::p2d::Axle & axle)
    {
        _Class1 * c1 = static_cast<_Class1 *>(s1);
        _Class2 * c2 = static_cast<_Class2 *>(s2);
        axle = sad::p2d::ortho(sad::p2d::axle( (c1->*p1)(), (c2->*p2)() ), sad::p2d::OrthoVectorIndex::OVI_DEG_90);
    }

};

static void (*r_to_r_callbacks[4])(sad::p2d::CollisionShape *, sad::p2d::CollisionShape *, sad::p2d::Axle &) =
{
    AxleFor<sad::p2d::Rectangle, sad::p2d::Rectangle>::hp1callsi<&sad::p2d::Rectangle::point, 0, 1>,
    AxleFor<sad::p2d::Rectangle, sad::p2d::Rectangle>::hp1callsi<&sad::p2d::Rectangle::point, 1, 2>,
    AxleFor<sad::p2d::Rectangle, sad::p2d::Rectangle>::hp2callsi<&sad::p2d::Rectangle::point, 0, 1>,
    AxleFor<sad::p2d::Rectangle, sad::p2d::Rectangle>::hp2callsi<&sad::p2d::Rectangle::point, 1, 2>
};


bool sad::p2d::CollisionTest::collidesRtoR(sad::p2d::Rectangle * p1, sad::p2d::Rectangle * p2)
{
    PROFILER_EVENT;
    return fast_collide(p1, p2, r_to_r_callbacks, 4);
}

static void (*r_to_c_callbacks[6])(sad::p2d::CollisionShape *,sad::p2d::CollisionShape *, sad::p2d::Axle &) =
{
    AxleFor<sad::p2d::Rectangle, sad::p2d::Circle>::hp1callsi<&sad::p2d::Rectangle::point, 0, 1>,
    AxleFor<sad::p2d::Rectangle, sad::p2d::Circle>::hp1callsi<&sad::p2d::Rectangle::point, 1, 2>,
    AxleFor<sad::p2d::Rectangle, sad::p2d::Circle>::hp1ip2<&sad::p2d::Rectangle::point, 0, &sad::p2d::Circle::centerRef>,
    AxleFor<sad::p2d::Rectangle, sad::p2d::Circle>::hp1ip2<&sad::p2d::Rectangle::point, 1, &sad::p2d::Circle::centerRef>,
    AxleFor<sad::p2d::Rectangle, sad::p2d::Circle>::hp1ip2<&sad::p2d::Rectangle::point, 2, &sad::p2d::Circle::centerRef>,
    AxleFor<sad::p2d::Rectangle, sad::p2d::Circle>::hp1ip2<&sad::p2d::Rectangle::point, 3, &sad::p2d::Circle::centerRef>
};

bool sad::p2d::CollisionTest::collidesRtoC(sad::p2d::Rectangle * p1, sad::p2d::Circle * p2)
{
    PROFILER_EVENT;
    return fast_collide(p1, p2, r_to_c_callbacks, 6);
}

static void (*r_to_l_callbacks[4])(sad::p2d::CollisionShape *, sad::p2d::CollisionShape *, sad::p2d::Axle &) =
{
    AxleFor<sad::p2d::Rectangle, sad::p2d::Line>::hp1callsi<&sad::p2d::Rectangle::point, 0, 1>,
    AxleFor<sad::p2d::Rectangle, sad::p2d::Line>::hp1callsi<&sad::p2d::Rectangle::point, 1, 2>,
    AxleFor<sad::p2d::Rectangle, sad::p2d::Line>::p2calls<&sad::p2d::Line::p1, &sad::p2d::Line::p2>,
    AxleFor<sad::p2d::Rectangle, sad::p2d::Line>::op2calls<&sad::p2d::Line::p1, &sad::p2d::Line::p2>
};

bool sad::p2d::CollisionTest::collidesRtoL(sad::p2d::Rectangle * p1, sad::p2d::Line * p2)
{
    PROFILER_EVENT;
    return fast_collide(p1, p2, r_to_l_callbacks, 4);
}

bool sad::p2d::CollisionTest::collidesCtoC(sad::p2d::Circle * p1, sad::p2d::Circle * p2)
{
    PROFILER_EVENT;
    // A small optimization, since distance runs slow
    double dist2 = p1->radius() + p2->radius();
    if ( fabs(p1->centerRef().x() - p2->centerRef().x()) > dist2)
    {
        return false;
    }
    if (fabs(p1->centerRef().y() - p2->centerRef().y()) > dist2)
    {
        return false;
    }
    double dist1 = p1->centerRef().distance(p2->centerRef());
    // The precision was found empirical, because sometimes object collides in
    // very strange phase
    bool collides = (dist1 < dist2) || sad::is_fuzzy_equal(dist1, dist2, 1.0E-6);
    return collides;
}

static void (*c_to_l_callbacks[6])(sad::p2d::CollisionShape *, sad::p2d::CollisionShape *, sad::p2d::Axle &) =
{
    AxleFor<sad::p2d::Circle, sad::p2d::Line>::p2calls<&sad::p2d::Line::p1, &sad::p2d::Line::p2>,
    AxleFor<sad::p2d::Circle, sad::p2d::Line>::op2calls<&sad::p2d::Line::p1, &sad::p2d::Line::p2>,
    AxleFor<sad::p2d::Circle, sad::p2d::Line>::p1hp2<&sad::p2d::Circle::centerRef, &sad::p2d::Line::p1>,
    AxleFor<sad::p2d::Circle, sad::p2d::Line>::p1hp2<&sad::p2d::Circle::centerRef, &sad::p2d::Line::p2>,
    AxleFor<sad::p2d::Circle, sad::p2d::Line>::op1hp2<&sad::p2d::Circle::centerRef, &sad::p2d::Line::p1>,
    AxleFor<sad::p2d::Circle, sad::p2d::Line>::op1hp2<&sad::p2d::Circle::centerRef, &sad::p2d::Line::p2>,
};

bool sad::p2d::CollisionTest::collidesCtoL(sad::p2d::Circle * p1, sad::p2d::Line * p2)
{
    PROFILER_EVENT;
    return fast_collide(p1, p2, c_to_l_callbacks, 6);
}


static void (*l_to_l_callbacks[4])(sad::p2d::CollisionShape *, sad::p2d::CollisionShape *, sad::p2d::Axle &) =
{
    AxleFor<sad::p2d::Line, sad::p2d::Line>::p2calls<&sad::p2d::Line::p1, &sad::p2d::Line::p2>,
    AxleFor<sad::p2d::Line, sad::p2d::Line>::op2calls<&sad::p2d::Line::p1, &sad::p2d::Line::p2>,
    AxleFor<sad::p2d::Line, sad::p2d::Line>::p1calls<&sad::p2d::Line::p1, &sad::p2d::Line::p2>,
    AxleFor<sad::p2d::Line, sad::p2d::Line>::op1calls<&sad::p2d::Line::p1, &sad::p2d::Line::p2>
};
bool sad::p2d::CollisionTest::collidesLtoL(sad::p2d::Line * p1, sad::p2d::Line * p2)
{
    PROFILER_EVENT;
    return fast_collide(p1, p2, l_to_l_callbacks, 4);
}


bool sad::p2d::CollisionTest::collidesBtoB(sad::p2d::Bound * p1, sad::p2d::Bound * p2)
{
    PROFILER_EVENT;
    sad::p2d::BoundType bt1 = p1->type();
    sad::p2d::BoundType bt2 = p2->type();
    if (p1->isOrthogonal(p2) || bt1 == bt2)
    {
        return true;
    }
    // Check opposite
    if ((bt1 == sad::p2d::BoundType::BT_LEFT  &&  bt2 == sad::p2d::BoundType::BT_RIGHT)
        || (bt1 == sad::p2d::BoundType::BT_DOWN  &&  bt2 == sad::p2d::BoundType::BT_UP))
    {
        return p1->position() >= p2->position();	
    }
    if ((bt1 == sad::p2d::BoundType::BT_RIGHT  &&  bt2 == sad::p2d::BoundType::BT_LEFT)
        || (bt1 == sad::p2d::BoundType::BT_UP  &&  bt2 == sad::p2d::BoundType::BT_DOWN))
    {
        return p1->position() <= p2->position();	
    }
    return false;
}


bool sad::p2d::CollisionTest::collidesBtoR(sad::p2d::Bound * p1, sad::p2d::Rectangle * p2)
{
    PROFILER_EVENT;
    return sad::p2d::CollisionTest::collidesBtoS(p1, p2);
}

bool sad::p2d::CollisionTest::collidesBtoC(sad::p2d::Bound * p1, sad::p2d::Circle * p2)
{
    PROFILER_EVENT;
    if (p1->type() == sad::p2d::BoundType::BT_LEFT)
    {
        double p = p2->center().x() - p2->radius();
        return p <= p1->position();
    }
    if (p1->type() == sad::p2d::BoundType::BT_RIGHT)
    {
        double p = p2->center().x() + p2->radius();
        return p >= p1->position();
    }
    if (p1->type() == sad::p2d::BoundType::BT_UP)
    {
        double p = p2->center().y() + p2->radius();
        return p >= p1->position();
    }
    if (p1->type() == sad::p2d::BoundType::BT_DOWN)
    {
        double p = p2->center().y() - p2->radius();
        return p <= p1->position();
    }
    return false;
}

bool sad::p2d::CollisionTest::collidesBtoL(sad::p2d::Bound * p1, sad::p2d::Line * p2)
{
    PROFILER_EVENT;
    return sad::p2d::CollisionTest::collidesBtoS(p1, p2);
}

bool sad::p2d::CollisionTest::collidesBtoS(sad::p2d::Bound * p1, sad::p2d::CollisionShape * p2)
{
    PROFILER_EVENT;
    if (p1->type() == sad::p2d::BoundType::BT_LEFT)
    {
        sad::p2d::Vector v(1, 0);
        sad::p2d::Cutter1D c = p2->project(v);
        return c.p1() <= p1->position();
    }
    if (p1->type() == sad::p2d::BoundType::BT_RIGHT)
    {
        sad::p2d::Vector v(1, 0);
        sad::p2d::Cutter1D c = p2->project(v);
        return c.p2() >= p1->position();
    }
    if (p1->type() == sad::p2d::BoundType::BT_UP)
    {
        sad::p2d::Vector v(0, 1);
        sad::p2d::Cutter1D c = p2->project(v);
        return c.p2() >= p1->position();
    }
    if (p1->type() == sad::p2d::BoundType::BT_DOWN)
    {
        sad::p2d::Vector v(0, 1);
        sad::p2d::Cutter1D c = p2->project(v);
        return c.p1() <= p1->position();
    }
    return false;
}
