#include "animations/easing/easingtypes.h"
#define _USE_MATH_DEFINES 1
#include <math.h>

#define PI_OVER_2 M_PI_2
#define TWO_PI (M_PI * 2.0)

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

static double easeLinear(double time, double duration, double /*overshootOrAmplitude*/, double /*period*/)
{
    return time / duration;
}

static double easeInSine(double time, double duration, double /*overshootOrAmplitude*/, double /*period*/)
{
    return -cos(time / duration * PI_OVER_2) + 1;
}

static double easeOutSine(double time, double duration, double /*overshootOrAmplitude*/, double /*period*/)
{
    return sin(time / duration * PI_OVER_2);
}

static double easeInOutSine(double time, double duration, double /*overshootOrAmplitude*/, double /*period*/)
{
    return -0.5f * (cos(M_PI * time / duration) - 1);
}

static double easeInQuad(double time, double duration, double /*overshootOrAmplitude*/, double /*period*/)
{
    return (time /= duration) * time;
}

static double easeOutQuad(double time, double duration, double /*overshootOrAmplitude*/, double /*period*/)
{
    return -(time /= duration) * (time - 2);
}

static double easeInOutQuad(double time, double duration, double /*overshootOrAmplitude*/, double /*period*/)
{
    if ((time /= duration * 0.5f) < 1) return 0.5f * time * time;
    return -0.5f * ((--time) * (time - 2) - 1);
}

static double easeInCubic(double time, double duration, double /*overshootOrAmplitude*/, double /*period*/)
{
    return (time /= duration) * time * time;
}

static double easeOutCubic(double time, double duration, double /*overshootOrAmplitude*/, double /*period*/)
{
    return ((time = time / duration - 1) * time * time + 1);
}

static double easeInOutCubic(double time, double duration, double /*overshootOrAmplitude*/, double /*period*/)
{
    if ((time /= duration * 0.5f) < 1) return 0.5f * time * time * time;
    return 0.5f * ((time -= 2) * time * time + 2);
}

static double easeInQuart(double time, double duration, double /*overshootOrAmplitude*/, double /*period*/)
{
    return  (time /= duration) * time * time * time;
}

static double easeOutQuart(double time, double duration, double /*overshootOrAmplitude*/, double /*period*/)
{
    return  -((time = time / duration - 1) * time * time * time - 1);
}

static double easeInOutQuart(double time, double duration, double /*overshootOrAmplitude*/, double /*period*/)
{
    if ((time /= duration * 0.5f) < 1) return 0.5f * time * time * time * time;
    return -0.5f * ((time -= 2) * time * time * time - 2);
}

static double easeInQuint(double time, double duration, double /*overshootOrAmplitude*/, double /*period*/)
{
    return (time /= duration) * time * time * time * time;
}

static double easeOutQuint(double time, double duration, double /*overshootOrAmplitude*/, double /*period*/)
{
    return ((time = time / duration - 1) * time * time * time * time + 1);;
}

static double easeInOutQuint(double time, double duration, double /*overshootOrAmplitude*/, double /*period*/)
{
    if ((time /= duration * 0.5f) < 1) return 0.5f * time * time * time * time * time;
    return 0.5f * ((time -= 2) * time * time * time * time + 2);
}

static double easeInExpo(double time, double duration, double /*overshootOrAmplitude*/, double /*period*/)
{
    return (fabs(time) < 0.001) ? 0 : pow(2, 10 * (time / duration - 1));
}

static double easeOutExpo(double time, double duration, double /*overshootOrAmplitude*/, double /*period*/)
{
    if (abs(time - duration) < 0.001) return 1;
    return (-pow(2, -10 * time / duration) + 1);
}

static double easeInOutExpo(double time, double duration, double /*overshootOrAmplitude*/, double /*period*/)
{
    if (abs(time) < 0.001) return 0;
    if (abs(time - duration) < 0.001) return 1;
    if ((time /= duration * 0.5f) < 1) return 0.5f * pow(2, 10 * (time - 1));
    return 0.5f * (-pow(2, -10 * --time) + 2);
}

static double easeInCirc(double time, double duration, double /*overshootOrAmplitude*/, double /*period*/)
{
   return -(sqrt(1 - (time /= duration) * time) - 1);
}

static double easeOutCirc(double time, double duration, double /*overshootOrAmplitude*/, double /*period*/)
{
   return sqrt(1 - (time = time / duration - 1) * time);
}

static double easeInOutCirc(double time, double duration, double /*overshootOrAmplitude*/, double /*period*/)
{
   if ((time /= duration * 0.5f) < 1) return -0.5f * (sqrt(1 - time * time) - 1);
    return 0.5f * (sqrt(1 - (time -= 2) * time) + 1);
}

static double easeInElastic(double time, double duration, double overshootOrAmplitude, double period)
{
    float s0;
    if (fabs(time) < 0.001) return 0;
    if (fabs((time /= duration) - 1) < 0.001) return 1;
    if (fabs(period) < 0.001) period = duration * 0.3f;
    if (overshootOrAmplitude < 1) {
        overshootOrAmplitude = 1;
        s0 = period / 4;
    } else s0 = period / TWO_PI * asin(1 / overshootOrAmplitude);
    return -(overshootOrAmplitude * pow(2, 10 * (time -= 1)) * sin((time * duration - s0) * TWO_PI / period));            
}

static double easeOutElastic(double time, double duration, double overshootOrAmplitude, double period)
{
    float s1;
    if (fabs(time) < 0.001) return 0;
    if (fabs((time /= duration) - 1) < 0.001) return 1;
    if (fabs(period) < 0.001) period = duration * 0.3f;
    if (overshootOrAmplitude < 1) {
        overshootOrAmplitude = 1;
        s1 = period / 4;
    } else s1 = period / TWO_PI * asin(1 / overshootOrAmplitude);
    return (overshootOrAmplitude * pow(2, -10 * time) * sin((time * duration - s1) * TWO_PI / period) + 1);                      
}

static double easeInOutElastic(double time, double duration, double overshootOrAmplitude, double period)
{
    float s;
    if (fabs(time) < 0.001) return 0;
    if (fabs(time /= duration * 0.5 - 2) < 0.001) return 1;
    if (abs(period) < 0.001) period = duration * (0.3f * 1.5f);
    if (overshootOrAmplitude < 1) {
        overshootOrAmplitude = 1;
        s = period / 4;
    } else s = period / TWO_PI * asin(1 / overshootOrAmplitude);
    if (time < 1) return -0.5f * (overshootOrAmplitude * pow(2, 10 * (time -= 1)) * sin((time * duration - s) * TWO_PI / period));
    return overshootOrAmplitude * pow(2, -10 * (time -= 1)) * sin((time * duration - s) * TWO_PI / period) * 0.5f + 1;                            
}

static double easeInBack(double time, double duration, double overshootOrAmplitude, double /*period*/)
{
    return (time /= duration) * time * ((overshootOrAmplitude + 1) * time - overshootOrAmplitude);                       
}

static double easeOutBack(double time, double duration, double overshootOrAmplitude, double /*period*/)
{
    return ((time = time / duration - 1) * time * ((overshootOrAmplitude + 1) * time + overshootOrAmplitude) + 1);                     
}

static double easeInOutBack(double time, double duration, double overshootOrAmplitude, double /*period*/)
{
    if ((time /= duration * 0.5f) < 1) return 0.5f * (time * time * (((overshootOrAmplitude *= (1.525f)) + 1) * time - overshootOrAmplitude));
    return 0.5f * ((time -= 2) * time * (((overshootOrAmplitude *= (1.525f)) + 1) * time + overshootOrAmplitude) + 2);                      
}

static double easeOutBounce(double time, double duration, double /*overshootOrAmplitude*/, double /*period*/)
{
    if ((time /= duration) < (1 / 2.75f)) {
        return (7.5625f * time * time);
    }
    if (time < (2 / 2.75f)) {
        return (7.5625f * (time -= (1.5f / 2.75f)) * time + 0.75f);
    }
    if (time < (2.5f / 2.75f)) {
        return (7.5625f * (time -= (2.25f / 2.75f)) * time + 0.9375f);
    }
    return (7.5625f * (time -= (2.625f / 2.75f)) * time + 0.984375f);                     
}

static double easeInBounce(double time, double duration, double /*overshootOrAmplitude*/, double /*period*/)
{
    return 1 - easeOutBounce(duration - time, duration, -1, -1);                    
}

static double easeInOutBounce(double time, double duration, double /*overshootOrAmplitude*/, double /*period*/)
{
    if (time < duration*0.5f)
    {
        return easeInBounce(time*2, duration, -1, -1)*0.5f;
    }
    return easeOutBounce(time*2 - duration, duration, -1, -1)*0.5f + 0.5f;                    
}

static double easeFlashWeighted(double overshootOrAmplitude, double period, int stepIndex, double /*stepDuration*/, double dir, double res)
{
    double easedRes = 0;
    double finalDecimals = 0;
    // Use previous stepIndex in case of odd ones, so that back ease is not clamped
    if (dir > 0 && static_cast<int>(overshootOrAmplitude) % 2 == 0) stepIndex++;
    else if (dir < 0 && static_cast<int>(overshootOrAmplitude) % 2 != 0) stepIndex++;

    if (period > 0) {
        int finalTruncated = (static_cast<int>(overshootOrAmplitude));
        finalDecimals = overshootOrAmplitude - finalTruncated;
        if (finalTruncated % 2 > 0) finalDecimals = 1 - finalDecimals;
        finalDecimals = (finalDecimals * stepIndex) / overshootOrAmplitude;
        easedRes = (res * (overshootOrAmplitude - stepIndex)) / overshootOrAmplitude;
    } else if (period < 0) {
        period = -period;
        easedRes = (res * stepIndex) / overshootOrAmplitude;
    }
    double diff = easedRes - res;
    res += (diff * period) + finalDecimals;
    if (res > 1) res = 1;
    return res;
}

static double easeFlash(double time, double duration, double overshootOrAmplitude, double period)
{
    int stepIndex = static_cast<int>(ceil((time / duration) * overshootOrAmplitude)); // 1 to overshootOrAmplitude
    float stepDuration = duration / overshootOrAmplitude;
    time -= stepDuration * (stepIndex - 1);
    float dir = (stepIndex % 2 != 0) ? 1 : -1;
    if (dir < 0) time -= stepDuration;
    float res = (time * dir) / stepDuration;
    return easeFlashWeighted(overshootOrAmplitude, period, stepIndex, stepDuration, dir, res);             
}

static double easeFlashIn(double time, double duration, double overshootOrAmplitude, double period)
{
    int stepIndex = static_cast<int>(ceil((time / duration) * overshootOrAmplitude)); // 1 to overshootOrAmplitude
    float stepDuration = duration / overshootOrAmplitude;
    time -= stepDuration * (stepIndex - 1);
    float dir = (stepIndex % 2 != 0) ? 1 : -1;
    if (dir < 0) time -= stepDuration;
    time = time * dir;
    float res = (time /= stepDuration) * time;
    return easeFlashWeighted(overshootOrAmplitude, period, stepIndex, stepDuration, dir, res);        
}

static double easeFlashOut(double time, double duration, double overshootOrAmplitude, double period)
{
    int stepIndex = static_cast<int>(ceil((time / duration) * overshootOrAmplitude)); // 1 to overshootOrAmplitude
    float stepDuration = duration / overshootOrAmplitude;
    time -= stepDuration * (stepIndex - 1);
    float dir = (stepIndex % 2 != 0) ? 1 : -1;
    if (dir < 0) time -= stepDuration;
    time = time * dir;
    float res = -(time /= stepDuration) * (time - 2);           
    return easeFlashWeighted(overshootOrAmplitude, period, stepIndex, stepDuration, dir, res);        
}

static double easeFlashInOut(double time, double duration, double overshootOrAmplitude, double period)
{
    int stepIndex = static_cast<int>(ceil((time / duration) * overshootOrAmplitude)); // 1 to overshootOrAmplitude
    float stepDuration = duration / overshootOrAmplitude;
    time -= stepDuration * (stepIndex - 1);
    float dir = (stepIndex % 2 != 0) ? 1 : -1;
    if (dir < 0) time -= stepDuration;
    time = time * dir;
    float res = (time /= stepDuration * 0.5f) < 1
        ? 0.5f * time * time
        : -0.5f * ((--time) * (time - 2) - 1);         
    return easeFlashWeighted(overshootOrAmplitude, period, stepIndex, stepDuration, dir, res);        
}


static sad::animations::easing::FunctionCallback callbacks[] = {
    easeLinear,
    easeInSine,
    easeOutSine,
    easeInOutSine,
    easeInQuad,
    easeOutQuad,
    easeInOutQuad,
    easeInCubic,
    easeOutCubic,
    easeInOutCubic,
    easeInQuart,
    easeOutQuart,
    easeInOutQuart,
    easeInQuint,
    easeOutQuint,
    easeInOutQuint,
    easeInExpo,
    easeOutExpo,
    easeInOutExpo,
    easeInCirc,
    easeOutCirc,
    easeInOutCirc,
    easeInElastic,
    easeOutElastic,
    easeInOutElastic,
    easeInBack,
    easeOutBack,
    easeInOutBack,
    easeInBounce,
    easeOutBounce,
    easeInOutBounce,
    easeFlash,
    easeFlashIn,
    easeFlashOut,
    easeFlashInOut
};


sad::animations::easing::FunctionCallback sad::animations::easing::callbackByType(sad::animations::easing::Types t)
{
    if (static_cast<int>(t) < 0 || static_cast<int>(t) > sad::animations::easing::ATTT_InOutFlash)
    {
        return easeLinear;
    }
    return callbacks[static_cast<int>(t)];
}

