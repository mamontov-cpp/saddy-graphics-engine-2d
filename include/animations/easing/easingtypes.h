/*! \file animations/easing/easingtypes.h

    Describes a types of tween motions in function
 */
#pragma once
// Author: Dmitry Mamontov
// Ported from C# to C++ - 08.02.2016
//
// Author: Daniele Giardini - http://www.demigiant.com
// Created: 2014/07/19 14:11
// 
// License Copyright (c) Daniele Giardini.
// This work is subject to the terms at http://dotween.demigiant.com/license.php
// 
// =============================================================
// Contains Daniele Giardini's C# port of the easing equations created by Robert Penner
// (all easing equations except for Flash, InFlash, OutFlash, InOutFlash,
// which use some parts of Robert Penner's equations but were created by Daniele Giardini)
// http://robertpenner.com/easing, see license below:
// =============================================================
//
// TERMS OF USE - EASING EQUATIONS
//
// Open source under the BSD License.
//
// Copyright © 2001 Robert Penner
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
// - Redistributions of source code must retain the above copyright notice,
// this list of conditions and the following disclaimer.
// - Redistributions in binary form must reproduce the above copyright notice,
// this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
// - Neither the name of the author nor the names of contributors may be used to endorse
// or promote products derived from this software without specific prior written permission.
// - THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
// IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
// STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
// EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

namespace sad
{

namespace animations
{
    
namespace easing
{
    
/*! A types of easing functions
 */
enum class Types: int
{
    ATTT_Linear = 0,
    ATTT_InSine = 1,
    ATTT_OutSine = 2,
    ATTT_InOutSine = 3,
    ATTT_InQuad = 4,
    ATTT_OutQuad = 5,
    ATTT_InOutQuad = 6,
    ATTT_InCubic = 7,
    ATTT_OutCubic = 8,
    ATTT_InOutCubic = 9,
    ATTT_InQuart = 10,
    ATTT_OutQuart = 11,
    ATTT_InOutQuart = 12,
    ATTT_InQuint = 13,
    ATTT_OutQuint = 14,
    ATTT_InOutQuint = 15,
    ATTT_InExpo = 16,
    ATTT_OutExpo = 17,
    ATTT_InOutExpo = 18,
    ATTT_InCirc = 19,
    ATTT_OutCirc = 20,
    ATTT_InOutCirc = 21,
    ATTT_InElastic = 22,
    ATTT_OutElastic = 23,
    ATTT_InOutElastic = 24,
    ATTT_InBack = 25,
    ATTT_OutBack = 26,
    ATTT_InOutBack = 27,
    ATTT_InBounce = 28,
    ATTT_OutBounce = 29,
    ATTT_InOutBounce = 30
};

/*! A basic callback for easing functions
    \param[in] time a current time, since animation has started playing
    \param[in] duration a duration for playing
    \param[in] overshootOrAmplitude an amplitude for some functions
    \param[in] period a period for some functions
    \return new value
 */
typedef double (*FunctionCallback)(double time, double duration, double overshootOrAmplitude, double period);

/*! Returns a callback function by it's type
    \param[in] t type
    \return callback
 */
sad::animations::easing::FunctionCallback callbackByType(sad::animations::easing::Types t);

}

}

}
