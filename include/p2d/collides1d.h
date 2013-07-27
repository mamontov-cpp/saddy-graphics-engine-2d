/*! \file collides1d.h
	\author HiddenSeeker

	Declares a functionfor solving collision problem in 1d case
 */


/*! Determines, whether two 1-dimensional cutters collide
	\param[in] x11 first point of first cutter
	\param[in] x12 second point of first cutter
	\param[in] x21 first  point of second cutter
	\param[in] x22 second point of second cutter
	\return whether two 1D cutters collid
*/
inline bool collides1D(float x11, float x12, float x21, float x22)
{
	if (x11>x12) { float tmp=x11; x11=x12; x12=tmp; }
	if (x21>x22) { float tmp=x21; x21=x22; x22=tmp; }
	return (x21<=x12) && (x22>=x11);
}