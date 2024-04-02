/*! \file shooter.h
    

    Describes a simple shooter, which shoots object
 */
#pragma once
#include <phy52d/object.h>
#include <phy52d/objectemitter.h>
#include "coloredbullet.h"
#pragma once

/*! A simple default object
 */
class Shooter: public sad::phy52d::Object
{
 SAD_OBJECT

 class Gun: public sad::phy52d::RandomDirectionObjectEmitter<ColoredBullet>
 {
  protected:
    Shooter * m_parent;
  public:
     Gun(Shooter * parent);
     virtual sad::phy52d::App * app() override;
     virtual void perform() override;
 };
 protected:
     Gun * m_gun;
 public:
     Shooter();
     void startShooting() const;
};
