#include "enemycounter.h"

DECLARE_SOBJ(EnemyCounter);

EnemyCounter::EnemyCounter() : m_enemy_counter(0)
{
    
}

void EnemyCounter::reset()
{
    m_enemy_counter = 0;
    m_list.clear();
}


void EnemyCounter::clear()
{
    m_enemy_counter = 0;
}

void EnemyCounter::set(int result)
{
    m_enemy_counter = result;
}

void EnemyCounter::increment()
{
    m_enemy_counter += 1;
}

void EnemyCounter::decrement()
{
    m_enemy_counter -= 1;
    if (m_enemy_counter <= 0)
    {
        for(size_t i = 0; i < m_list.size(); i++)
        {
            m_list[i]();
        }
        m_list.clear();
    }
}

void EnemyCounter::onZeroEnemies(const std::function<void()>& f)
{
    m_list << f;
}