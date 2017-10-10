#include "sadmutex.h"
#include "animations/easing/easingfunction.h"

#include "db/schema/schema.h"
#include "db/dbproperty.h"
#include "db/save.h"
#include "db/load.h"
#include "db/dbfield.h"
#include "db/dbmethodpair.h"

DECLARE_SOBJ(sad::animations::easing::Function)

sad::animations::easing::Function::Function()
: m_type(sad::animations::easing::ATTT_Linear),
m_overshoot_amplitude(0),
m_period(0)
{
    m_callback = sad::animations::easing::callbackByType(m_type);    
}


sad::animations::easing::Function::Function(unsigned int type, double overshootAmplitude, double period) : m_overshoot_amplitude(overshootAmplitude), m_period(period)
{
    if (type > static_cast<unsigned int>(sad::animations::easing::ATTT_InOutBounce))
    {
        type = sad::animations::easing::ATTT_InOutBounce;
    }
    m_type = static_cast<sad::animations::easing::Types>(type);
    m_callback = sad::animations::easing::callbackByType(m_type);
}

sad::animations::easing::Function::Function(sad::animations::easing::Types type, double overshootAmplitude, double period)
: m_type(type), m_overshoot_amplitude(overshootAmplitude), m_period(period)
{
    m_callback = sad::animations::easing::callbackByType(m_type);    
}

sad::animations::easing::Function::~Function()
{
    
}

sad::animations::easing::Function* sad::animations::easing::Function::clone() const
{
    return new sad::animations::easing::Function(m_type, m_overshoot_amplitude, m_period);
}

double sad::animations::easing::Function::eval(double time, double duration)
{
    if (fabs(duration) < 0.001)
    {
        return 1;
    }
    return m_callback(time, duration, m_overshoot_amplitude, m_period);
}

double sad::animations::easing::Function::evalBounded(double time, double duration)
{
    double val = eval(time, duration);
    if (val < 0)
    {
        val = 0;
    }
    if (val > 1)
    {
        val = 1;
    }
    return val;
}

void sad::animations::easing::Function::setFunctionTypeAsUnsignedInt(unsigned int type)
{
    if (static_cast<int>(type) > sad::animations::easing::ATTT_InOutBounce)
    {
        m_type = sad::animations::easing::ATTT_Linear;
    }
    else
    {
        m_type = static_cast<sad::animations::easing::Types>(type);
    }
    setFunctionType(m_type);
}

unsigned int sad::animations::easing::Function::functionTypeAsUnsignedInt() const
{
    return static_cast<unsigned int>(m_type);
}

void sad::animations::easing::Function::setFunctionType(sad::animations::easing::Types type)
{
    m_type = type;
    m_callback = sad::animations::easing::callbackByType(m_type);   
}

sad::animations::easing::Types sad::animations::easing::Function::functionType() const
{
    return m_type;
}

void sad::animations::easing::Function::setOvershootAmplitude(double a)
{
    m_overshoot_amplitude = a;
}

double sad::animations::easing::Function::overshootAmplitude() const
{
    return m_overshoot_amplitude;
}

void sad::animations::easing::Function::setPeriod(double a)
{
    m_period = a;
}

double sad::animations::easing::Function::period() const
{
    return m_period;
}

static sad::db::schema::Schema* AnimationsEasingFunctionSchema = NULL;
static sad::Mutex AnimationsEasingFunctionSchemaInit;

sad::db::schema::Schema* sad::animations::easing::Function::basicSchema()
{
    if (AnimationsEasingFunctionSchema == NULL)
    {
        AnimationsEasingFunctionSchemaInit.lock();
        if (AnimationsEasingFunctionSchema == NULL)
        {
            AnimationsEasingFunctionSchema = new sad::db::schema::Schema();
            AnimationsEasingFunctionSchema->addParent(sad::Object::basicSchema());
            AnimationsEasingFunctionSchema->add(
                "functiontype", 
                new sad::db::MethodPair<sad::animations::easing::Function, unsigned int>(
                    &sad::animations::easing::Function::functionTypeAsUnsignedInt,
                    &sad::animations::easing::Function::setFunctionTypeAsUnsignedInt
                )
            );	
            AnimationsEasingFunctionSchema->add(
                "overshootamplitude", 
                new sad::db::MethodPair<sad::animations::easing::Function, double>(
                    &sad::animations::easing::Function::overshootAmplitude,
                    &sad::animations::easing::Function::setOvershootAmplitude
                )
            );	
            AnimationsEasingFunctionSchema->add(
                "period", 
                new sad::db::MethodPair<sad::animations::easing::Function, double>(
                    &sad::animations::easing::Function::period,
                    &sad::animations::easing::Function::setPeriod
                )
            );	

            sad::ClassMetaDataContainer::ref()->pushGlobalSchema(AnimationsEasingFunctionSchema);
        }
        AnimationsEasingFunctionSchemaInit.unlock();
    }
    return AnimationsEasingFunctionSchema;
}

sad::db::schema::Schema* sad::animations::easing::Function::schema() const
{
    return sad::animations::easing::Function::basicSchema();
}

static sad::db::ObjectFactory EasingFunctionFactory(true); 
static bool EasingFunctionFactoryInitialized = false;
static sad::Mutex EasingFunctionFactoryLock;

sad::db::ObjectFactory* sad::animations::easing::Function::factory()
{
    if (!EasingFunctionFactoryInitialized)
    {
        EasingFunctionFactoryLock.lock();
        if (!EasingFunctionFactoryInitialized)
        {
             EasingFunctionFactory.add<sad::animations::easing::Function>(
                 "sad::animations::easing::Function", 
                 sad::animations::easing::Function::basicSchema(), 
                 false
             );
        }
        EasingFunctionFactoryLock.unlock();
    }
    return &EasingFunctionFactory;
}
