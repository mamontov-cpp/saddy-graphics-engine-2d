/*! \file    collision.h
    \author  HiddenSeeker
	
	\brief   Contains a collision detection functions.
	
	This file provides a collision detection.
*/
#include "hxyrect.h"
#include "hline.h"
#pragma once
/*! Function which return length of collision of two spaces on one axle
   \param[in] x11   coordinate X1  of first cutter
   \param[in] x12   coordinate X2  of first cutter
   \param[in] x21   coordinate X1 of second cutter
   \param[in] x22   coordinate X2 of second cutter
   \return  length of first collision, 0 if no collision detected.
*/
float get1DCollision(float x11,float x12, float x21,float x22);
/*! Function which return length of collision of two spaces on one axle
   \param[in] l1  coordinates of first line
   \param[in] l2  coordinates of second line
   \return length of first collision, 0 if no collision detected.
*/
float get1DCollision(const h1DLine  & l1, const h1DLine & l2);
/*! Function which returns a rectangle of common space of rectangles r1 and r2 
    \note For solving collision (to make their not colliding) you must just push one rectangle
          up and left on width and height of this result.
    \param[in] r1  first rectangle
    \param[in] r2  second rectangle
    \return out rectangle of collision space, or point (0,0) if collision is not detected.
*/
hst::xyrect getCollisionRect(const hst::xyrect & r1, const hst::xyrect & r2);
/*! Function, which return a common point of two lines or between point on 
    full collision and does not return anything, if no collision.
    \param[in] l1 first line
    \param[in] l2 second line
    \param[out] res reference to returning point	
	\return true if collision found
*/
bool    getCollisionPoint(const hLine & l1, const hLine & l2,hPointF & res); 
/*! Function, which detects a collision of two rectangles: of r1, related to r2.
    \param[in] r1  first rectangle
    \param[in] r2  second rectangle
    \return true if collides, false if not.
*/
bool     collides(const hst::xyrect & r1,const hst::xyrect & r2); 
/*! Function, which detects a collision of two lines. This function detects collision just on one axle.
    \param[in] li1  first line
	\param[in] li2  second line
	\param[in] ax   axle
	\return true if collides, false if not.
*/ 
bool     collides(const hLine & li1, const  hLine & li2, axle_t ax); 
/*! Function, which detects a collision of two lines. This function detects collision on all axis.
    \param[in] li1  first line
	\param[in] li2  second line
	\return true if collides, false if not.
*/ 
bool     collides(const hLine & li1, const  hLine & li2);
/*! Detect, whether is point within the rectangle
    \param[in] p point
	\param[in] r rectangle
	\return true if collides, false if not.
*/
bool     isWithin(const hPointF & p, const hst::xyrect & r);
/*! Detection of collision of line and rectangle
    \param[in] l line
	\param[in] r rectangle
	\return true if collides, false if not.
*/
bool     collides(const hLine & l,const hst::xyrect & r);
/*! Detection of collision of line and rectangle
    \param[in] p1 first  point of line
	\param[in] p2 second point of line
	\param[in] r  rectangle
	\return true if collides, false if not.
*/
bool     collides(const hPointF & p1,const hPointF & p2,const hst::xyrect & r);
/*! Detects a collision of two moving rectangles. Any two states MUST HAVE
    equal sizes.
     \param[in] r1f first state of first rectangle
	 \param[in] r1s second state of first rectangle
	 \param[in] r2f first state of second rectangle
	 \param[in] r2s second state of second rectangle
*/
bool collides(const hst::xyrect & r1f,
			  const hst::xyrect & r1s,
			  const hst::xyrect & r2f,
			  const hst::xyrect & r2s
			  );
/*! Detects a collision of two moving rectangles. Any two states MUST HAVE
    equal sizes. Used collides for two rects on less, or collides for four
     \param[in] r1f first state of first rectangle
	 \param[in] r1s second state of first rectangle
	 \param[in] r2f first state of second rectangle
	 \param[in] r2s second state of second rectangle
*/
bool optimizedcollides(const hst::xyrect & r1f,
					   const hst::xyrect & r1s,
					   const hst::xyrect & r2f,
					   const hst::xyrect & r2s
			          );

