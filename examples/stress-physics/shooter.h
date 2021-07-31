/*! \file shooter.h
    

    Describes a simple shooter, which shoots object
 */
#pragma once
#include <p2d/app/object.h>
// ReSharper disable once CppUnusedIncludeDirective
#include <p2d/app/constants.h>
#include <p2d/app/objectemitter.h>
#include "coloredbullet.h"
#pragma once

/*! A simple default object
 */
class Shooter: public sad::p2d::app::Object
{
 SAD_OBJECT

 class Gun: public sad::p2d::app::RandomDirectionObjectEmitter<ColoredBullet>
 {
  protected:
    Shooter * m_parent;
  public:
     Gun(Shooter * parent);
     virtual sad::p2d::app::App * app() override;
     virtual void perform() override;
 };
 protected:
     Gun * m_gun;
 public:
     Shooter();
     void startShooting();
};
