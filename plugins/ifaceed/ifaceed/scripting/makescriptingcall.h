/*! \file makescriptingcall.h
	\author HiddenSeeker

	Contains a function for making a call for function with scripting
 */
#pragma once
#include "scriptingcall.h"
#include "scripting.h"

namespace scripting
{
/*! Makes new scripting call
	\param[in] f a function
	\param[in] s engine
 */
template<
	typename _ReturnType
>
scripting::Callable* make_scripting_call(_ReturnType (*f)(scripting::Scripting*), scripting::Scripting* s)
{
	return new typename scripting::ScriptingFunctionCall<_ReturnType>::template WithArgs0<>(s->engine(), "", f);
}

/*! Makes new scripting call
	\param[in] f a function
	\param[in] s engine
 */
template<
	typename _ReturnType,
	typename _ArgType0
>
scripting::Callable* make_scripting_call(_ReturnType (*f)(scripting::Scripting*,_ArgType0), scripting::Scripting* s)
{
	return new typename scripting::ScriptingFunctionCall<_ReturnType>::template WithArgs1<_ArgType0>(s->engine(), "", f);
}

/*! Makes new scripting call
	\param[in] f a function
	\param[in] s engine
 */
template<
	typename _ReturnType,
	typename _ArgType0,
	typename _ArgType1
>
scripting::Callable* make_scripting_call(_ReturnType (*f)(scripting::Scripting*,_ArgType0, _ArgType1), scripting::Scripting* s)
{
	return new typename scripting::ScriptingFunctionCall<_ReturnType>::template WithArgs2<_ArgType0, _ArgType1>(s->engine(), "", f);
}

/*! Makes new scripting call
	\param[in] f a function
	\param[in] s engine
 */
template<
	typename _ReturnType,
	typename _ArgType0,
	typename _ArgType1,
	typename _ArgType2
>
scripting::Callable* make_scripting_call(_ReturnType (*f)(scripting::Scripting*,_ArgType0, _ArgType1, _ArgType2), scripting::Scripting* s)
{
	return new typename scripting::ScriptingFunctionCall<_ReturnType>::template WithArgs3<_ArgType0, _ArgType1, _ArgType2>(s->engine(), "", f);
}

/*! Makes new scripting call
	\param[in] f a function
	\param[in] s engine
 */
template<
	typename _ReturnType,
	typename _ArgType0,
	typename _ArgType1,
	typename _ArgType2,
	typename _ArgType3
>
scripting::Callable* make_scripting_call(_ReturnType (*f)(scripting::Scripting*,_ArgType0, _ArgType1, _ArgType2, _ArgType3), scripting::Scripting* s)
{
	return new typename scripting::ScriptingFunctionCall<_ReturnType>::template WithArgs4<_ArgType0, _ArgType1, _ArgType2, _ArgType3>(s->engine(), "", f);
}

/*! Makes new scripting call
	\param[in] f a function
	\param[in] s engine
 */
template<
	typename _ReturnType,
	typename _ArgType0,
	typename _ArgType1,
	typename _ArgType2,
	typename _ArgType3,
	typename _ArgType4
>
scripting::Callable* make_scripting_call(_ReturnType (*f)(scripting::Scripting*,_ArgType0, _ArgType1, _ArgType2, _ArgType3, _ArgType4), scripting::Scripting* s)
{
	return new typename scripting::ScriptingFunctionCall<_ReturnType>::template WithArgs5<_ArgType0, _ArgType1, _ArgType2, _ArgType3, _ArgType4>(s->engine(), "", f);
}

/*! Makes new scripting call
	\param[in] f a function
	\param[in] s engine
 */
template<
	typename _ReturnType,
	typename _ArgType0,
	typename _ArgType1,
	typename _ArgType2,
	typename _ArgType3,
	typename _ArgType4,
	typename _ArgType5
>
scripting::Callable* make_scripting_call(_ReturnType (*f)(scripting::Scripting*,_ArgType0, _ArgType1, _ArgType2, _ArgType3, _ArgType4, _ArgType5), scripting::Scripting* s)
{
	return new typename scripting::ScriptingFunctionCall<_ReturnType>::template WithArgs6<_ArgType0, _ArgType1, _ArgType2, _ArgType3, _ArgType4, _ArgType5>(s->engine(), "", f);
}

/*! Makes new scripting call
	\param[in] f a function
	\param[in] s engine
 */
template<
	typename _ReturnType,
	typename _ArgType0,
	typename _ArgType1,
	typename _ArgType2,
	typename _ArgType3,
	typename _ArgType4,
	typename _ArgType5,
	typename _ArgType6
>
scripting::Callable* make_scripting_call(_ReturnType (*f)(scripting::Scripting*,_ArgType0, _ArgType1, _ArgType2, _ArgType3, _ArgType4, _ArgType5, _ArgType6), scripting::Scripting* s)
{
	return new typename scripting::ScriptingFunctionCall<_ReturnType>::template WithArgs7<_ArgType0, _ArgType1, _ArgType2, _ArgType3, _ArgType4, _ArgType5, _ArgType6>(s->engine(), "", f);
}

/*! Makes new scripting call
	\param[in] f a function
	\param[in] s engine
 */
template<
	typename _ReturnType,
	typename _ArgType0,
	typename _ArgType1,
	typename _ArgType2,
	typename _ArgType3,
	typename _ArgType4,
	typename _ArgType5,
	typename _ArgType6,
	typename _ArgType7
>
scripting::Callable* make_scripting_call(_ReturnType (*f)(scripting::Scripting*,_ArgType0, _ArgType1, _ArgType2, _ArgType3, _ArgType4, _ArgType5, _ArgType6, _ArgType7), scripting::Scripting* s)
{
	return new typename scripting::ScriptingFunctionCall<_ReturnType>::template WithArgs8<_ArgType0, _ArgType1, _ArgType2, _ArgType3, _ArgType4, _ArgType5, _ArgType6, _ArgType7>(s->engine(), "", f);
}

/*! Makes new scripting call
	\param[in] f a function
	\param[in] s engine
 */
template<
	typename _ReturnType,
	typename _ArgType0,
	typename _ArgType1,
	typename _ArgType2,
	typename _ArgType3,
	typename _ArgType4,
	typename _ArgType5,
	typename _ArgType6,
	typename _ArgType7,
	typename _ArgType8
>
scripting::Callable* make_scripting_call(_ReturnType (*f)(scripting::Scripting*,_ArgType0, _ArgType1, _ArgType2, _ArgType3, _ArgType4, _ArgType5, _ArgType6, _ArgType7, _ArgType8), scripting::Scripting* s)
{
	return new typename scripting::ScriptingFunctionCall<_ReturnType>::template WithArgs9<_ArgType0, _ArgType1, _ArgType2, _ArgType3, _ArgType4, _ArgType5, _ArgType6, _ArgType7, _ArgType8>(s->engine(), "", f);
}

/*! Makes new scripting call
	\param[in] f a function
	\param[in] s engine
 */
template<
	typename _ReturnType,
	typename _ArgType0,
	typename _ArgType1,
	typename _ArgType2,
	typename _ArgType3,
	typename _ArgType4,
	typename _ArgType5,
	typename _ArgType6,
	typename _ArgType7,
	typename _ArgType8,
	typename _ArgType9
>
scripting::Callable* make_scripting_call(_ReturnType (*f)(scripting::Scripting*,_ArgType0, _ArgType1, _ArgType2, _ArgType3, _ArgType4, _ArgType5, _ArgType6, _ArgType7, _ArgType8, _ArgType9), scripting::Scripting* s)
{
	return new typename scripting::ScriptingFunctionCall<_ReturnType>::template WithArgs10<_ArgType0, _ArgType1, _ArgType2, _ArgType3, _ArgType4, _ArgType5, _ArgType6, _ArgType7, _ArgType8, _ArgType9>(s->engine(), "", f);
}

/*! Makes new scripting call
	\param[in] f a function
	\param[in] s engine
 */
template<
	typename _ReturnType,
	typename _ArgType0,
	typename _ArgType1,
	typename _ArgType2,
	typename _ArgType3,
	typename _ArgType4,
	typename _ArgType5,
	typename _ArgType6,
	typename _ArgType7,
	typename _ArgType8,
	typename _ArgType9,
	typename _ArgType10
>
scripting::Callable* make_scripting_call(_ReturnType (*f)(scripting::Scripting*,_ArgType0, _ArgType1, _ArgType2, _ArgType3, _ArgType4, _ArgType5, _ArgType6, _ArgType7, _ArgType8, _ArgType9, _ArgType10), scripting::Scripting* s)
{
	return new typename scripting::ScriptingFunctionCall<_ReturnType>::template WithArgs11<_ArgType0, _ArgType1, _ArgType2, _ArgType3, _ArgType4, _ArgType5, _ArgType6, _ArgType7, _ArgType8, _ArgType9, _ArgType10>(s->engine(), "", f);
}

/*! Makes new scripting call
	\param[in] f a function
	\param[in] s engine
 */
template<
	typename _ReturnType,
	typename _ArgType0,
	typename _ArgType1,
	typename _ArgType2,
	typename _ArgType3,
	typename _ArgType4,
	typename _ArgType5,
	typename _ArgType6,
	typename _ArgType7,
	typename _ArgType8,
	typename _ArgType9,
	typename _ArgType10,
	typename _ArgType11
>
scripting::Callable* make_scripting_call(_ReturnType (*f)(scripting::Scripting*,_ArgType0, _ArgType1, _ArgType2, _ArgType3, _ArgType4, _ArgType5, _ArgType6, _ArgType7, _ArgType8, _ArgType9, _ArgType10, _ArgType11), scripting::Scripting* s)
{
	return new typename scripting::ScriptingFunctionCall<_ReturnType>::template WithArgs12<_ArgType0, _ArgType1, _ArgType2, _ArgType3, _ArgType4, _ArgType5, _ArgType6, _ArgType7, _ArgType8, _ArgType9, _ArgType10, _ArgType11>(s->engine(), "", f);
}

/*! Makes new scripting call
	\param[in] f a function
	\param[in] s engine
 */
template<
	typename _ReturnType,
	typename _ArgType0,
	typename _ArgType1,
	typename _ArgType2,
	typename _ArgType3,
	typename _ArgType4,
	typename _ArgType5,
	typename _ArgType6,
	typename _ArgType7,
	typename _ArgType8,
	typename _ArgType9,
	typename _ArgType10,
	typename _ArgType11,
	typename _ArgType12
>
scripting::Callable* make_scripting_call(_ReturnType (*f)(scripting::Scripting*,_ArgType0, _ArgType1, _ArgType2, _ArgType3, _ArgType4, _ArgType5, _ArgType6, _ArgType7, _ArgType8, _ArgType9, _ArgType10, _ArgType11, _ArgType12), scripting::Scripting* s)
{
	return new typename scripting::ScriptingFunctionCall<_ReturnType>::template WithArgs13<_ArgType0, _ArgType1, _ArgType2, _ArgType3, _ArgType4, _ArgType5, _ArgType6, _ArgType7, _ArgType8, _ArgType9, _ArgType10, _ArgType11, _ArgType12>(s->engine(), "", f);
}

/*! Makes new scripting call
	\param[in] f a function
	\param[in] s engine
 */
template<
	typename _ReturnType,
	typename _ArgType0,
	typename _ArgType1,
	typename _ArgType2,
	typename _ArgType3,
	typename _ArgType4,
	typename _ArgType5,
	typename _ArgType6,
	typename _ArgType7,
	typename _ArgType8,
	typename _ArgType9,
	typename _ArgType10,
	typename _ArgType11,
	typename _ArgType12,
	typename _ArgType13
>
scripting::Callable* make_scripting_call(_ReturnType (*f)(scripting::Scripting*,_ArgType0, _ArgType1, _ArgType2, _ArgType3, _ArgType4, _ArgType5, _ArgType6, _ArgType7, _ArgType8, _ArgType9, _ArgType10, _ArgType11, _ArgType12, _ArgType13), scripting::Scripting* s)
{
	return new typename scripting::ScriptingFunctionCall<_ReturnType>::template WithArgs14<_ArgType0, _ArgType1, _ArgType2, _ArgType3, _ArgType4, _ArgType5, _ArgType6, _ArgType7, _ArgType8, _ArgType9, _ArgType10, _ArgType11, _ArgType12, _ArgType13>(s->engine(), "", f);
}

/*! Makes new scripting call
	\param[in] f a function
	\param[in] s engine
 */
template<
	typename _ReturnType,
	typename _ArgType0,
	typename _ArgType1,
	typename _ArgType2,
	typename _ArgType3,
	typename _ArgType4,
	typename _ArgType5,
	typename _ArgType6,
	typename _ArgType7,
	typename _ArgType8,
	typename _ArgType9,
	typename _ArgType10,
	typename _ArgType11,
	typename _ArgType12,
	typename _ArgType13,
	typename _ArgType14
>
scripting::Callable* make_scripting_call(_ReturnType (*f)(scripting::Scripting*,_ArgType0, _ArgType1, _ArgType2, _ArgType3, _ArgType4, _ArgType5, _ArgType6, _ArgType7, _ArgType8, _ArgType9, _ArgType10, _ArgType11, _ArgType12, _ArgType13, _ArgType14), scripting::Scripting* s)
{
	return new typename scripting::ScriptingFunctionCall<_ReturnType>::template WithArgs15<_ArgType0, _ArgType1, _ArgType2, _ArgType3, _ArgType4, _ArgType5, _ArgType6, _ArgType7, _ArgType8, _ArgType9, _ArgType10, _ArgType11, _ArgType12, _ArgType13, _ArgType14>(s->engine(), "", f);
}

/*! Makes new scripting call
	\param[in] f a function
	\param[in] s engine
 */
template<
	typename _ReturnType,
	typename _ArgType0,
	typename _ArgType1,
	typename _ArgType2,
	typename _ArgType3,
	typename _ArgType4,
	typename _ArgType5,
	typename _ArgType6,
	typename _ArgType7,
	typename _ArgType8,
	typename _ArgType9,
	typename _ArgType10,
	typename _ArgType11,
	typename _ArgType12,
	typename _ArgType13,
	typename _ArgType14,
	typename _ArgType15
>
scripting::Callable* make_scripting_call(_ReturnType (*f)(scripting::Scripting*,_ArgType0, _ArgType1, _ArgType2, _ArgType3, _ArgType4, _ArgType5, _ArgType6, _ArgType7, _ArgType8, _ArgType9, _ArgType10, _ArgType11, _ArgType12, _ArgType13, _ArgType14, _ArgType15), scripting::Scripting* s)
{
	return new typename scripting::ScriptingFunctionCall<_ReturnType>::template WithArgs16<_ArgType0, _ArgType1, _ArgType2, _ArgType3, _ArgType4, _ArgType5, _ArgType6, _ArgType7, _ArgType8, _ArgType9, _ArgType10, _ArgType11, _ArgType12, _ArgType13, _ArgType14, _ArgType15>(s->engine(), "", f);
}

}
