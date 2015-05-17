#include "animations/animationssequential.h"
#include "animations/animationsinstance.h"

#include <util/fs.h>

#include <resource/physicalfile.h>

#include <3rdparty/picojson/valuetotype.h>

#include <fstream>

DECLARE_SOBJ_INHERITANCE(sad::animations::Sequential, sad::animations::Composite);

// ====================================== PUBLIC METHODS ======================================

sad::animations::Sequential::Sequential()
{

}

sad::animations::Sequential::~Sequential()
{

}

void sad::animations::Sequential::setState(sad::animations::Instance* i, double time)
{
    double timebegin = 0;
    if (m_commands.contains(i->object()))
    {
        Commands& c  = m_commands[i->object()];
        for(size_t ii = 0; ii < m_links.size(); ii++)
        {
            sad::animations::Animation* animation = m_links[ii]->object(false);
            if (animation)
            {
                double t = time - timebegin;
                if (t < animation->time())
                {
                    i->setStateCommand(c[ii], false);
                    animation->setState(i, t);
                    return;
                }
                else
                {
                    timebegin += animation->time();
                }
            }
        }
    }
}

void sad::animations::Sequential::setTime(double)
{
    this->sad::animations::Animation::setTime(this->time());
}

double sad::animations::Sequential::time() const
{
    double time = 0;
    for(size_t ii = 0; ii < m_links.size(); ii++)
    {
        sad::animations::Animation* animation = m_links[ii]->object(true);
        if (animation)
        {
           time += animation->time();
        }
    }
    return time;
}

void sad::animations::Sequential::updateBeforePlaying()
{
	if (m_valid == false)
	{
		this->sad::animations::Animation::setTime(this->time());
		updateValidFlag();
	}
}
