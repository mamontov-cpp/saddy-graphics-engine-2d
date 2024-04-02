#include <phy52d/object.h>


DECLARE_SOBJ_INHERITANCE(sad::phy52d::Object, sad::SceneNode)

sad::phy52d::Object::Object() : m_app(nullptr)
{
    // We don't init sprite as valid, to preserve working with simple
    // objects
    m_sprite = new sad::Sprite2D(nullptr, sad::Rect2D(), sad::Rect2D());
    m_body = new sad::p2d::Body();
    m_body->addRef();
    m_sprite->addRef();

    
    // Set self as user object to make type inference inside collisions
    // possible
    m_body->setUserObject(this);
    
    m_listener1 = new p2d::MovementDeltaListener<sad::phy52d::Object, p2d::Vector>(
            this, 
            &sad::phy52d::Object::notifyMove
    );
    m_listener2 = new p2d::MovementDeltaListener<sad::phy52d::Object, double>(
            this, 
            &sad::phy52d::Object::notifyRotate
    );
    // Add listeners, needed to synchronize sprite and a game object
    m_body->addMoveListener(m_listener1);
    m_body->addRotateListener(m_listener2);
}

sad::phy52d::Object::~Object()
{
    //printf("%p: destroyed\n", static_cast<sad::Object*>(this));    
    if (m_body) 
    {
        m_body->delRef();
    }
    if (m_sprite) 
    {
        m_sprite->delRef();
    }
}

void sad::phy52d::Object::setApp(sad::phy52d::App * g)
{
    m_app = g;
}

void sad::phy52d::Object::setScene(sad::Scene * s)
{
    this->sad::SceneNode::setScene(s);
    m_sprite->setScene(s);
}

void sad::phy52d::Object::rendererChanged()
{
    m_sprite->rendererChanged();
}

// ReSharper disable once CppMemberFunctionMayBeConst
void sad::phy52d::Object::notifyMove(const p2d::Vector & dist)
{
    m_sprite->moveBy(dist);
}

// ReSharper disable once CppMemberFunctionMayBeConst
void sad::phy52d::Object::notifyRotate(const double & angle)
{
    m_sprite->rotate(angle);
}

void sad::phy52d::Object::render()
{
    m_sprite->render();
}

void sad::phy52d::Object::setAngularVelocity(double v) const
{
    m_body->setCurrentAngularVelocity(v);
}

void sad::phy52d::Object::setHorizontalSpeed(double v) const
{
    sad::p2d::Vector velocity = m_body->tangentialVelocity();
    velocity.setX(v);
    m_body->setCurrentTangentialVelocity(velocity);
}

void sad::phy52d::Object::setVerticalSpeed(double v) const
{
    sad::p2d::Vector velocity = m_body->tangentialVelocity();
    velocity.setY(v);
    m_body->setCurrentTangentialVelocity(velocity);
}

void sad::phy52d::Object::stopHorizontal() const
{
    sad::p2d::Vector velocity = m_body->tangentialVelocity();
    velocity.setX(0);
    m_body->setCurrentTangentialVelocity(velocity);
}

void sad::phy52d::Object::stopVertical() const
{
    sad::p2d::Vector velocity = m_body->tangentialVelocity();
    velocity.setY(0);
    m_body->setCurrentTangentialVelocity(velocity);
} 

void sad::phy52d::Object::stop() const
{
    m_body->setCurrentTangentialVelocity(p2d::Vector(0,0)); 
}

sad::p2d::Body * sad::phy52d::Object::body() const
{
    return m_body;
}

void sad::phy52d::Object::setAngle(double angle) const
{
    this->m_body->setCurrentAngle(angle);
}

void sad::phy52d::Object::lookAt(const sad::Point2D & p) const
{
    const sad::Point2D c = p - this->m_body->position();
    const double angle = sad::angleOf(c.x(), c.y());
    // We rotate it counter-clockwise, because object looks to pi
    setAngle(angle);
}

sad::phy52d::App * sad::phy52d::Object::app() const
{
    return m_app;
}

sad::p2d::Point sad::phy52d::Object::position() const
{
    return m_body->position();
}

double sad::phy52d::Object::angle() const
{
    return m_body->angle();
}

void sad::phy52d::Object::setPosition(const sad::p2d::Point & p) const
{
    m_body->setCurrentPosition(p);
}

void sad::phy52d::Object::moveBy(sad::Point2D& p)
{
    setPosition(position() + p);    
}


void sad::phy52d::Object::addTangentialForce(sad::p2d::TangentialForce * f) const
{
    this->body()->tangentialForces().add(f);
}

void sad::phy52d::Object::addAngularForce(sad::p2d::AngularForce * f) const
{
    this->body()->angularForces().add(f);
}

void sad::phy52d::Object::setTangentialVelocity(const sad::p2d::Vector & v) const
{
    this->body()->setCurrentTangentialVelocity(v);
}


void sad::phy52d::Object::setOptions(sad::Sprite2D::Options* options) const
{
    m_sprite->set(*options);
}

void sad::phy52d::Object::setShape(sad::p2d::CollisionShape* shape) const
{
    m_body->setShape(shape);
}

void sad::phy52d::Object::setShaderFunction(sad::ShaderFunction* fun)
{
    m_sprite->setShaderFunction(fun);
    this->sad::SceneNode::setShaderFunction(fun);
}

void sad::phy52d::Object::onAddedToScene()
{
    m_sprite->onAddedToScene();
}

void sad::phy52d::Object::onRemovedFromScene()
{
    m_sprite->onRemovedFromScene();
}