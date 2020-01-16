#include <p2d/app/object.h>


DECLARE_SOBJ_INHERITANCE(sad::p2d::app::Object, sad::SceneNode);

sad::p2d::app::Object::Object()
{
    // We don't init sprite as valid, to preserve working with simple
    // objects
    m_sprite = new Sprite2D(NULL, sad::Rect2D(), sad::Rect2D());
    m_body = new sad::p2d::Body();
    m_body->addRef();
    m_sprite->addRef();

    
    // Set self as user object to make type inference inside collisions
    // possible
    m_body->setUserObject(this);
    
    m_listener1 = new p2d::MovementDeltaListener<p2d::app::Object, p2d::Vector>(
            this, 
            &sad::p2d::app::Object::notifyMove
    );
    m_listener2 = new p2d::MovementDeltaListener<p2d::app::Object, double>(
            this, 
            &sad::p2d::app::Object::notifyRotate
    );
    // Add listeners, needed to synchronize sprite and a game object
    m_body->addMoveListener(m_listener1);
    m_body->addRotateListener(m_listener2);
}

sad::p2d::app::Object::~Object()
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

void sad::p2d::app::Object::setApp(p2d::app::App * g)
{
    m_app = g;
}

void sad::p2d::app::Object::setScene(sad::Scene * s)
{
    this->sad::SceneNode::setScene(s);
    m_sprite->setScene(s);
}

void sad::p2d::app::Object::rendererChanged()
{
    m_sprite->rendererChanged();
}

void sad::p2d::app::Object::notifyMove(const p2d::Vector & dist)
{
    m_sprite->moveBy(dist);
}

void sad::p2d::app::Object::notifyRotate(const double & angle)
{
    m_sprite->rotate(angle);
}

void sad::p2d::app::Object::render()
{
    m_sprite->render();
}

void sad::p2d::app::Object::setAngularVelocity(double v)
{
    m_body->setCurrentAngularVelocity(v);
}

void sad::p2d::app::Object::setHorizontalSpeed(double v)
{
    sad::p2d::Vector velocity = m_body->tangentialVelocity();
    velocity.setX(v);
    m_body->setCurrentTangentialVelocity(velocity);
}

void sad::p2d::app::Object::setVerticalSpeed(double v)
{
    sad::p2d::Vector velocity = m_body->tangentialVelocity();
    velocity.setY(v);
    m_body->setCurrentTangentialVelocity(velocity);
}

void sad::p2d::app::Object::stopHorizontal()
{
    sad::p2d::Vector velocity = m_body->tangentialVelocity();
    velocity.setX(0);
    m_body->setCurrentTangentialVelocity(velocity);
}

void sad::p2d::app::Object::stopVertical()
{
    sad::p2d::Vector velocity = m_body->tangentialVelocity();
    velocity.setY(0);
    m_body->setCurrentTangentialVelocity(velocity);
} 

void sad::p2d::app::Object::stop()
{
    m_body->setCurrentTangentialVelocity(p2d::Vector(0,0)); 
}

sad::p2d::Body * sad::p2d::app::Object::body()
{
    return m_body;
}

void sad::p2d::app::Object::setAngle(double angle)
{
    this->m_body->setCurrentAngle(angle);
}

void sad::p2d::app::Object::lookAt(const sad::Point2D & p)
{
    sad::Point2D c = p - this->m_body->position();
    double angle = sad::angleOf(c.x(), c.y());
    // We roate it counter-clockwise, because object looks to pi
    setAngle(angle);
}

sad::p2d::app::App * sad::p2d::app::Object::app()
{
    return m_app;
}

sad::p2d::Point sad::p2d::app::Object::position() const
{
    return m_body->position();
}

double sad::p2d::app::Object::angle() const
{
    return m_body->angle();
}

void sad::p2d::app::Object::setPosition(const sad::p2d::Point & p)
{
    m_body->setCurrentPosition(p);
}

void sad::p2d::app::Object::moveBy(sad::Point2D& p)
{
    setPosition(position() + p);    
}


void sad::p2d::app::Object::addTangentialForce(sad::p2d::TangentialForce * f)
{
    this->body()->tangentialForces().add(f);
}

void sad::p2d::app::Object::addAngularForce(sad::p2d::AngularForce * f)
{
    this->body()->angularForces().add(f);
}

void sad::p2d::app::Object::setTangentialVelocity(const sad::p2d::Vector & v)
{
    this->body()->setCurrentTangentialVelocity(v);
}

void sad::p2d::app::Object::setShaderFunction(sad::ShaderFunction* fun)
{
    m_sprite->setShaderFunction(fun);
    this->sad::SceneNode::setShaderFunction(fun);
}

void sad::p2d::app::Object::onAddedToScene()
{
    m_sprite->onAddedToScene();
}

void sad::p2d::app::Object::onRemovedFromScene()
{
    m_sprite->onRemovedFromScene();
}