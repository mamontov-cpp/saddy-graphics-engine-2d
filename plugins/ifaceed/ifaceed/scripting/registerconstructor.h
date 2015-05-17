/*! \file registerconstructor.h
	\author HiddenSeeker

	Defines register constructor function
 */
#pragma once
#include "scripting.h"

namespace scripting
{
/*! Registers a constructor call
	\param[in] name a name of object
	\param[in] s scripting object
 */
template<
	typename _Constructed
>
void register_constructor(const QString& name, scripting::Scripting* s)
{
	QScriptClass* c = new scripting::ConstructorCall0<_Constructed>(s->engine(), name);
	s->registerScriptClass(name, c);
}

/*! Registers a constructor call
	\param[in] name a name of object
	\param[in] s scripting object
 */
template<
	typename _Constructed,
	typename _Arg0
>
void register_constructor(const QString& name, scripting::Scripting* s)
{
	QScriptClass* c = new scripting::ConstructorCall1<_Constructed, _Arg0>(s->engine(), name);
	s->registerScriptClass(name, c);
}

/*! Registers a constructor call
	\param[in] name a name of object
	\param[in] s scripting object
 */
template<
	typename _Constructed,
	typename _Arg0,
	typename _Arg1
>
void register_constructor(const QString& name, scripting::Scripting* s)
{
	QScriptClass* c = new scripting::ConstructorCall2<_Constructed, _Arg0, _Arg1>(s->engine(), name);
	s->registerScriptClass(name, c);
}

/*! Registers a constructor call
	\param[in] name a name of object
	\param[in] s scripting object
 */
template<
	typename _Constructed,
	typename _Arg0,
	typename _Arg1,
	typename _Arg2
>
void register_constructor(const QString& name, scripting::Scripting* s)
{
	QScriptClass* c = new scripting::ConstructorCall3<_Constructed, _Arg0, _Arg1, _Arg2>(s->engine(), name);
	s->registerScriptClass(name, c);
}

/*! Registers a constructor call
	\param[in] name a name of object
	\param[in] s scripting object
 */
template<
	typename _Constructed,
	typename _Arg0,
	typename _Arg1,
	typename _Arg2,
	typename _Arg3
>
void register_constructor(const QString& name, scripting::Scripting* s)
{
	QScriptClass* c = new scripting::ConstructorCall4<_Constructed, _Arg0, _Arg1, _Arg2, _Arg3>(s->engine(), name);
	s->registerScriptClass(name, c);
}

/*! Registers a constructor call
	\param[in] name a name of object
	\param[in] s scripting object
 */
template<
	typename _Constructed,
	typename _Arg0,
	typename _Arg1,
	typename _Arg2,
	typename _Arg3,
	typename _Arg4
>
void register_constructor(const QString& name, scripting::Scripting* s)
{
	QScriptClass* c = new scripting::ConstructorCall5<_Constructed, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4>(s->engine(), name);
	s->registerScriptClass(name, c);
}

/*! Registers a constructor call
	\param[in] name a name of object
	\param[in] s scripting object
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
void register_constructor(const QString& name, scripting::Scripting* s)
{
	QScriptClass* c = new scripting::ConstructorCall6<_Constructed, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5>(s->engine(), name);
	s->registerScriptClass(name, c);
}

/*! Registers a constructor call
	\param[in] name a name of object
	\param[in] s scripting object
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
void register_constructor(const QString& name, scripting::Scripting* s)
{
	QScriptClass* c = new scripting::ConstructorCall7<_Constructed, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6>(s->engine(), name);
	s->registerScriptClass(name, c);
}

/*! Registers a constructor call
	\param[in] name a name of object
	\param[in] s scripting object
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
void register_constructor(const QString& name, scripting::Scripting* s)
{
	QScriptClass* c = new scripting::ConstructorCall8<_Constructed, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7>(s->engine(), name);
	s->registerScriptClass(name, c);
}

/*! Registers a constructor call
	\param[in] name a name of object
	\param[in] s scripting object
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
void register_constructor(const QString& name, scripting::Scripting* s)
{
	QScriptClass* c = new scripting::ConstructorCall9<_Constructed, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8>(s->engine(), name);
	s->registerScriptClass(name, c);
}

/*! Registers a constructor call
	\param[in] name a name of object
	\param[in] s scripting object
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
void register_constructor(const QString& name, scripting::Scripting* s)
{
	QScriptClass* c = new scripting::ConstructorCall10<_Constructed, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9>(s->engine(), name);
	s->registerScriptClass(name, c);
}

/*! Registers a constructor call
	\param[in] name a name of object
	\param[in] s scripting object
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
void register_constructor(const QString& name, scripting::Scripting* s)
{
	QScriptClass* c = new scripting::ConstructorCall11<_Constructed, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10>(s->engine(), name);
	s->registerScriptClass(name, c);
}

/*! Registers a constructor call
	\param[in] name a name of object
	\param[in] s scripting object
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
void register_constructor(const QString& name, scripting::Scripting* s)
{
	QScriptClass* c = new scripting::ConstructorCall12<_Constructed, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11>(s->engine(), name);
	s->registerScriptClass(name, c);
}

/*! Registers a constructor call
	\param[in] name a name of object
	\param[in] s scripting object
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
void register_constructor(const QString& name, scripting::Scripting* s)
{
	QScriptClass* c = new scripting::ConstructorCall13<_Constructed, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11, _Arg12>(s->engine(), name);
	s->registerScriptClass(name, c);
}

/*! Registers a constructor call
	\param[in] name a name of object
	\param[in] s scripting object
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
void register_constructor(const QString& name, scripting::Scripting* s)
{
	QScriptClass* c = new scripting::ConstructorCall14<_Constructed, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11, _Arg12, _Arg13>(s->engine(), name);
	s->registerScriptClass(name, c);
}

/*! Registers a constructor call
	\param[in] name a name of object
	\param[in] s scripting object
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
void register_constructor(const QString& name, scripting::Scripting* s)
{
	QScriptClass* c = new scripting::ConstructorCall15<_Constructed, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11, _Arg12, _Arg13, _Arg14>(s->engine(), name);
	s->registerScriptClass(name, c);
}

/*! Registers a constructor call
	\param[in] name a name of object
	\param[in] s scripting object
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
void register_constructor(const QString& name, scripting::Scripting* s)
{
	QScriptClass* c = new scripting::ConstructorCall16<_Constructed, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11, _Arg12, _Arg13, _Arg14, _Arg15>(s->engine(), name);
	s->registerScriptClass(name, c);
}

}
