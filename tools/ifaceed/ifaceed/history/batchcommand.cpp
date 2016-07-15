#include "batchcommand.h"

history::BatchCommand::BatchCommand()
{
    
}

history::BatchCommand::~BatchCommand()
{
    for(size_t i = 0; i < m_commands.size(); i++)
    {
        delete m_commands[i];
    }
}

void history::BatchCommand::commit(core::Editor * ob)
{
    for(size_t i = 0; i < m_commands.size(); i++)
    {
        m_commands[i]->commit(ob);
    }
}

void history::BatchCommand::rollback(core::Editor * ob)
{
    for(int i = m_commands.size() - 1; i > -1; i--)
    {
        m_commands[i]->rollback(ob);
    }
}

void history::BatchCommand::add(history::Command* c)
{
    m_commands << c;
}

void history::BatchCommand::remove(history::Command* c)
{
    m_commands.removeAll(c);	
}

size_t history::BatchCommand::count() const
{
    return m_commands.size();
}
