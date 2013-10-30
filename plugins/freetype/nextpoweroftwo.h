/*! \file nextpoweroftwo.h
	\author HiddenSeeker

	A definition of next power of two function, which rounds up number to
	next power of two.
 */
#pragma once

namespace sad
{

namespace freetype
{

/*! Computes nearest power of two to number. Guaranteed, that returned numberþ
    Taken from http://graphics.stanford.edu/~seander/bithacks.html#RoundUpPowerOf2
	will be more or equal to v
	\param[in] v number to be rounded
	\return v number to be rounded
 */
unsigned int next_power_of_two(unsigned int v);

}

}
