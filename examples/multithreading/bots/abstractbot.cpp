#include "abstractbot.h"

bots::AbstractBot::AbstractBot() : m_strategy(NULL)
{
    
}

bots::AbstractBot::~AbstractBot()
{
    if (m_strategy)
    {
        m_strategy->delRef();
    }
}

void bots::AbstractBot::setStrategy(bots::shootingstrategies::ShootingStrategy* strategy)
{
    if (m_strategy)
    {
        m_strategy->delRef();
    }
    m_strategy = strategy;
    if (m_strategy)
    {
        m_strategy->addRef();
    }
}

bots::shootingstrategies::ShootingStrategy* bots::AbstractBot::strategy() const
{
    return m_strategy;
}
