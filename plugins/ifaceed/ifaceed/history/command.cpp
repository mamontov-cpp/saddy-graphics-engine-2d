#include "command.h"

history::Command::Command() : m_enable_update_ui(true)
{
    
}

history::Command::~Command()
{

}

bool history::Command::enableUpdatingUIWhenCommittingOrRollingBack(bool value)
{
    bool result = m_enable_update_ui;
    m_enable_update_ui = value;
    return result;
}
