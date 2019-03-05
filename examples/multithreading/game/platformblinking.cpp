#include "platformblinking.h"
#include "../game.h"

#include <cassert>

// ==================================== PUBLIC METHODS ====================================

game::PlatformBlinking::PlatformBlinking(Game* game, sad::Sprite2D* platform, double time)
: m_game(game),
  m_time(time),
  m_hidden(false),
  m_enabled(true),
  m_platform(platform),
  m_body(NULL)
{
   assert(game);
   assert(platform);
   platform->addRef();
   sad::Vector<sad::p2d::Body*> bodies = m_game->physicsWorld()->allBodiesInGroup("platforms");

   for (size_t i = 0; i < bodies.size(); i++)
   {
       bool found = false;
       sad::Vector<sad::Object*> o = bodies[i]->userObjects();
       for(size_t j = 0; j < o.size(); j++)
       {
           if (o[j] == platform)
           {
               found = true;
           }
       }
       if (found)
       {
           m_body = bodies[i];
       }
   }

   if (m_body)
   {
       m_body->addRef();
   }

   m_hide_callback = [=]() {
       if (!this->m_enabled) {
           m_game->killPlatformBlinkingProcess(this);
           return;
       }
       this->m_hidden = true;
       this->hidePlatform();
       this->m_game->addDelayedTask(m_time, m_show_callback);
   };
   m_show_callback = [=]() {
       if (!this->m_enabled) {
           m_game->killPlatformBlinkingProcess(this);
           return;
       }
       this->m_hidden = false;
       this->showPlatform();
       this->m_game->addDelayedTask(m_time, m_hide_callback);
   };
}


game::PlatformBlinking::~PlatformBlinking()
{
    m_platform->delRef();
    if (m_body)
    {
        m_body->delRef();
    }
}

void game::PlatformBlinking::disable()
{
    this->showPlatform();
    m_enabled = false;
}

void game::PlatformBlinking::enable()
{
    if (m_hidden)
    {
        this->hidePlatform();
    }
    else
    {
        this->showPlatform();
    }
    m_enabled = true;
}


void game::PlatformBlinking::setTime(double time)
{
    m_time = time;
}


// ==================================== PRIVATE METHODS ====================================

void game::PlatformBlinking::showPlatform() const
{
    if (m_platform->visible())
    {
        return;
    }
    if (m_body)
    {
        sad::Rect2D common_rectangle = m_platform->area();
        // Small increase of area
        common_rectangle = sad::Rect2D(common_rectangle[0].x(), common_rectangle[0].y(), common_rectangle[2].x() + 1, common_rectangle[2].y());
        // Small fix for tiny platforms, to ensure their bounding box will be some times smaller than one
        if (common_rectangle.height() < 40) {
            common_rectangle = sad::Rect2D(common_rectangle[0].x(), common_rectangle[0].y() + 3, common_rectangle[2].x() - 1, common_rectangle[2].y());
        }
        sad::p2d::Rectangle* rect = new sad::p2d::Rectangle();
        rect->setRect(common_rectangle);
        m_body->setShape(rect);
        m_body->initPosition(m_platform->middle());

        m_game->physicsWorld()->addBodyToGroup("platforms", this->m_body);
    }
    m_platform->setVisible(true);
}


void game::PlatformBlinking::hidePlatform() const
{
    if (!m_platform->visible())
    {
        return;
    }

    if (m_body)
    {
        m_game->disableRestingForBodiesOnPlatform(this->m_body);
        m_game->physicsWorld()->removeBody(this->m_body);
    }
    m_platform->setVisible(false);
}
