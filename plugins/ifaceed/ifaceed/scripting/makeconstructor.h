/*! \file makeconstructor.h
	

	Defines a functions for making constructor
 */
#pragma once
#include "callable.h"
#include "scripting.h"

namespace scripting
{
/*! Makes a constructor call
	\param[in] s scripting object
	\return constructor call
 */
template<
	typename _Constructed
>
scripting::Callable* make_constructor(scripting::Scripting* s)
{
	return new scripting::ConstructorCall0<_Constructed>(s->engine(), "");
}

/*! Makes a constructor call
	\param[in] s scripting object
	\return constructor call
 */
template<
	typename _Constructed,
	typename _Arg0
>
scripting::Callable* make_constructor(scripting::Scripting* s)
{
	return new scripting::ConstructorCall1<_Constructed, _Arg0>(s->engine(), "");
}

/*! Makes a constructor call
	\param[in] s scripting object
	\return constructor call
 */
template<
	typename _Constructed,
	typename _Arg0,
	typename _Arg1
>
scripting::Callable* make_constructor(scripting::Scripting* s)
{
	return new scripting::ConstructorCall2<_Constructed, _Arg0, _Arg1>(s->engine(), "");
}

/*! Makes a constructor call
	\param[in] s scripting object
	\return constructor call
 */
template<
	typename _Constructed,
	typename _Arg0,
	typename _Arg1,
	typename _Arg2
>
scripting::Callable* make_constructor(scripting::Scripting* s)
{
	return new scripting::ConstructorCall3<_Constructed, _Arg0, _Arg1, _Arg2>(s->engine(), "");
}

/*! Makes a constructor call
	\param[in] s scripting object
	\return constructor call
 */
template<
	typename _Constructed,
	typename _Arg0,
	typename _Arg1,
	typename _Arg2,
	typename _Arg3
>
scripting::Callable* make_constructor(scripting::Scripting* s)
{
	return new scripting::ConstructorCall4<_Constructed, _Arg0, _Arg1, _Arg2, _Arg3>(s->engine(), "");
}

/*! Makes a constructor call
	\param[in] s scripting object
	\return constructor call
 */
template<
	typename _Constructed,
	typename _Arg0,
	typename _Arg1,
	typename _Arg2,
	typename _Arg3,
	typename _Arg4
>
scripting::Callable* make_constructor(scripting::Scripting* s)
{
	return new scripting::ConstructorCall5<_Constructed, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4>(s->engine(), "");
}

/*! Makes a constructor call
	\param[in] s scripting object
	\return constructor call
 */
template<
	typename _Constructed,
	typename _Arg0,
	typename _Arg1,
	typename _Arg2,
	typename _Arg3,
	typename _Arg4,
	typename _Arg5
>
scripting::Callable* make_constructor(scripting::Scripting* s)
{
	return new scripting::ConstructorCall6<_Constructed, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5>(s->engine(), "");
}

/*! Makes a constructor call
	\param[in] s scripting object
	\return constructor call
 */
template<
	typename _Constructed,
	typename _Arg0,
	typename _Arg1,
	typename _Arg2,
	typename _Arg3,
	typename _Arg4,
	typename _Arg5,
	typename _Arg6
>
scripting::Callable* make_constructor(scripting::Scripting* s)
{
	return new scripting::ConstructorCall7<_Constructed, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6>(s->engine(), "");
}

/*! Makes a constructor call
	\param[in] s scripting object
	\return constructor call
 */
template<
	typename _Constructed,
	typename _Arg0,
	typename _Arg1,
	typename _Arg2,
	typename _Arg3,
	typename _Arg4,
	typename _Arg5,
	typename _Arg6,
	typename _Arg7
>
scripting::Callable* make_constructor(scripting::Scripting* s)
{
	return new scripting::ConstructorCall8<_Constructed, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7>(s->engine(), "");
}

/*! Makes a constructor call
	\param[in] s scripting object
	\return constructor call
 */
template<
	typename _Constructed,
	typename _Arg0,
	typename _Arg1,
	typename _Arg2,
	typename _Arg3,
	typename _Arg4,
	typename _Arg5,
	typename _Arg6,
	typename _Arg7,
	typename _Arg8
>
scripting::Callable* make_constructor(scripting::Scripting* s)
{
	return new scripting::ConstructorCall9<_Constructed, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8>(s->engine(), "");
}

/*! Makes a constructor call
	\param[in] s scripting object
	\return constructor call
 */
template<
	typename _Constructed,
	typename _Arg0,
	typename _Arg1,
	typename _Arg2,
	typename _Arg3,
	typename _Arg4,
	typename _Arg5,
	typename _Arg6,
	typename _Arg7,
	typename _Arg8,
	typename _Arg9
>
scripting::Callable* make_constructor(scripting::Scripting* s)
{
	return new scripting::ConstructorCall10<_Constructed, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9>(s->engine(), "");
}

/*! Makes a constructor call
	\param[in] s scripting object
	\return constructor call
 */
template<
	typename _Constructed,
	typename _Arg0,
	typename _Arg1,
	typename _Arg2,
	typename _Arg3,
	typename _Arg4,
	typename _Arg5,
	typename _Arg6,
	typename _Arg7,
	typename _Arg8,
	typename _Arg9,
	typename _Arg10
>
scripting::Callable* make_constructor(scripting::Scripting* s)
{
	return new scripting::ConstructorCall11<_Constructed, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10>(s->engine(), "");
}

/*! Makes a constructor call
	\param[in] s scripting object
	\return constructor call
 */
template<
	typename _Constructed,
	typename _Arg0,
	typename _Arg1,
	typename _Arg2,
	typename _Arg3,
	typename _Arg4,
	typename _Arg5,
	typename _Arg6,
	typename _Arg7,
	typename _Arg8,
	typename _Arg9,
	typename _Arg10,
	typename _Arg11
>
scripting::Callable* make_constructor(scripting::Scripting* s)
{
	return new scripting::ConstructorCall12<_Constructed, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11>(s->engine(), "");
}

/*! Makes a constructor call
	\param[in] s scripting object
	\return constructor call
 */
template<
	typename _Constructed,
	typename _Arg0,
	typename _Arg1,
	typename _Arg2,
	typename _Arg3,
	typename _Arg4,
	typename _Arg5,
	typename _Arg6,
	typename _Arg7,
	typename _Arg8,
	typename _Arg9,
	typename _Arg10,
	typename _Arg11,
	typename _Arg12
>
scripting::Callable* make_constructor(scripting::Scripting* s)
{
	return new scripting::ConstructorCall13<_Constructed, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11, _Arg12>(s->engine(), "");
}

/*! Makes a constructor call
	\param[in] s scripting object
	\return constructor call
 */
template<
	typename _Constructed,
	typename _Arg0,
	typename _Arg1,
	typename _Arg2,
	typename _Arg3,
	typename _Arg4,
	typename _Arg5,
	typename _Arg6,
	typename _Arg7,
	typename _Arg8,
	typename _Arg9,
	typename _Arg10,
	typename _Arg11,
	typename _Arg12,
	typename _Arg13
>
scripting::Callable* make_constructor(scripting::Scripting* s)
{
	return new scripting::ConstructorCall14<_Constructed, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11, _Arg12, _Arg13>(s->engine(), "");
}

/*! Makes a constructor call
	\param[in] s scripting object
	\return constructor call
 */
template<
	typename _Constructed,
	typename _Arg0,
	typename _Arg1,
	typename _Arg2,
	typename _Arg3,
	typename _Arg4,
	typename _Arg5,
	typename _Arg6,
	typename _Arg7,
	typename _Arg8,
	typename _Arg9,
	typename _Arg10,
	typename _Arg11,
	typename _Arg12,
	typename _Arg13,
	typename _Arg14
>
scripting::Callable* make_constructor(scripting::Scripting* s)
{
	return new scripting::ConstructorCall15<_Constructed, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11, _Arg12, _Arg13, _Arg14>(s->engine(), "");
}

/*! Makes a constructor call
	\param[in] s scripting object
	\return constructor call
 */
template<
	typename _Constructed,
	typename _Arg0,
	typename _Arg1,
	typename _Arg2,
	typename _Arg3,
	typename _Arg4,
	typename _Arg5,
	typename _Arg6,
	typename _Arg7,
	typename _Arg8,
	typename _Arg9,
	typename _Arg10,
	typename _Arg11,
	typename _Arg12,
	typename _Arg13,
	typename _Arg14,
	typename _Arg15
>
scripting::Callable* make_constructor(scripting::Scripting* s)
{
	return new scripting::ConstructorCall16<_Constructed, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11, _Arg12, _Arg13, _Arg14, _Arg15>(s->engine(), "");
}

}
