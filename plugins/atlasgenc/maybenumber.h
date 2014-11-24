/*! \file maybenumber.h
    \author HiddenSeeker

    Defines a number as maybe item
 */
#pragma once
#include "../../include/maybe.h"


typedef sad::Maybe<double> MaybeNumber;

/*!
 * MaybeNumber behaves as infinite if not set
 * \param n1 first number
 * \param n2 second number
 * \return comparison result
 */
bool operator>(const MaybeNumber& n1, const MaybeNumber& n2);

/*!
 * Whether numbers are equal
 * \param n1 first number
 * \param n2 second number
 * \return comparison result
 */
bool operator==(const MaybeNumber& n1, const MaybeNumber& n2);
