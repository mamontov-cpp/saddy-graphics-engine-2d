#include <sadsemaphore.h>
#include <os/semaphoreimpl.h>

sad::Semaphore::Semaphore(unsigned int current_value, unsigned int max_value)
: m_dptr(new sad::os::SemaphoreImpl(current_value, max_value))
{

}

sad::Semaphore::Semaphore(const sad::Semaphore &)
: m_dptr(new sad::os::SemaphoreImpl(0, 32767))
{

}

sad::Semaphore & sad::Semaphore::operator=(const sad::Semaphore & o)
{
    return *this;
}

sad::Semaphore::~Semaphore()
{
    delete m_dptr;
}

void sad::Semaphore::consume(unsigned int amount)
{
    m_dptr->consume(amount);
}

void sad::Semaphore::release(unsigned int amount)
{
    m_dptr->release(amount);
}

int sad::Semaphore::value() const
{
    return m_dptr->value();
}
