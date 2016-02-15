#include "command.h"

history::Command::Command() 
{
    
}

history::Command::~Command()
{

}

void history::Command::commitWithoutUpdatingUI(core::Editor * e)
{
    this->commit(e);
}
