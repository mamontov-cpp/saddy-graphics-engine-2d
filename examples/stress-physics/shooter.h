/*! \file shooter.h
	\author HiddenSeeker

	Describes a simple shooter, which shoots object
 */
#include <p2d/app/object.h>
#include <p2d/app/constants.h>
#include <p2d/app/objectemitter.h>
#include "coloredbullet.h"
#pragma once

/*! A simple default object
 */
class Shooter: public p2d::app::Object
{
 SAD_OBJECT

 class Gun: public p2d::app::RandomDirectionObjectEmitter<ColoredBullet>
 {
  protected:
	Shooter * m_parent;
  public:
	 Gun(Shooter * parent);
	 virtual p2d::app::App * app();
	 virtual void perform();
 };
 protected:
	 Gun * m_gun;
 public:
	 Shooter();
	 void startShooting();
};
