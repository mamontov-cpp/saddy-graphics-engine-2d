/*! \file constructorname.h
    \author HiddenSeeker

    Returns name for constructor of type, as he should be registered in engine
 */
#pragma once


namespace scripting
{
/*! A constructor name template
 */
template<
  typename _Type
>
struct ConstructorName0
{
    /*! A name for constructor
     */
    const char* Name;
};

/*! A constructor name template
 */
template<
  typename _Type,
  typename _Arg
>
struct ConstructorName1
{
    /*! A name for constructor
     */
    const char* Name;
};

/*! A constructor name template
 */
template<
  typename _Type,
  typename _Arg1,
  typename _Arg2
>
struct ConstructorName2
{
    /*! A name for constructor
     */
    const char* Name;
};

/*! A constructor name template
 */
template<
  typename _Type,
  typename _Arg1,
  typename _Arg2,
  typename _Arg3
>
struct ConstructorName3
{
    /*! A name for constructor
     */
    const char* Name;
};

/*! A constructor name template
 */
template<
  typename _Type,
  typename _Arg1,
  typename _Arg2,
  typename _Arg3,
  typename _Arg4
>
struct ConstructorName4
{
    /*! A name for constructor
     */
    const char* Name;
};

}
