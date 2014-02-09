/*! \file valuetotype.h
	\author HiddenSeeker

	A simple helper for converting a picojson::value to specific type
 */
#pragma once
#include "getproperty.h"
#include "../../maybe.h"
#include "../sadcolor.h"
#include "../sadrect.h"
#include "../sadstring.h"

namespace picojson
{

/*! Tries to converts specific value to specific type
 */
template<typename _Type>
class ValueToType
{
public:
	/*! Tries to convert a picojson::value to specific type
		\param[in] v value
		\return a result (with value if any)
	 */
	static sad::Maybe<_Type> get(const picojson::value & v);
};

/*! Tries to converts specific value to unsigned char
 */
template<>
class ValueToType<unsigned char>
{
public:
	/*! Tries to convert a picojson::value to unsigned char
		\param[in] v value
		\return a result (with value if any)
	 */
	static sad::Maybe<unsigned char> get(const picojson::value & v)
	{
		sad::Maybe<unsigned char> result;
		if (v.is<double>())
		{
			double a = v.get<double>();
			if (a >= -0.000001 && a <= 255.000001)
			{
				result.setValue((unsigned char)a);
			}
		}
		return result;
	}
};

/*! Tries to converts specific value to double
 */
template<>
class ValueToType<double>
{
public:
	/*! Tries to convert a picojson::value to double
		\param[in] v value
		\return a result (with value if any)
	 */
	static sad::Maybe<double> get(const picojson::value & v)
	{
		sad::Maybe<double> result;
		if (v.is<double>())
		{
			double a = v.get<double>();
			result.setValue(a);
		}
		return result;
	}
};

/*! Tries to converts specific value to string
 */
template<>
class ValueToType<sad::String>
{
public:
	/*! Tries to convert a picojson::value to string
		\param[in] v value
		\return a result (with value if any)
	 */
	static sad::Maybe<sad::String> get(const picojson::value & v)
	{
		sad::Maybe<sad::String> result;
		if (v.is<std::string>())
		{
			result.setValue(v.get<std::string>());
		}
		return result;
	}
};

/*! Tries to converts specific value to color
 */
template<>
class ValueToType<sad::Color>
{
public:
	/*! Tries to convert a picojson::value to color
		\param[in] v value
		\return a result (with value if any)
	 */
	static sad::Maybe<sad::Color> get(const picojson::value & v)
	{
		sad::Maybe<sad::Color> result;
		picojson::value const * ro = picojson::get_property(v, "r");
		picojson::value const * go = picojson::get_property(v, "g");
		picojson::value const * bo = picojson::get_property(v, "b");
		if (ro && go && bo)
		{
			sad::Maybe<unsigned char> r = picojson::ValueToType<unsigned char>::get(*ro);
			sad::Maybe<unsigned char> g = picojson::ValueToType<unsigned char>::get(*go);
			sad::Maybe<unsigned char> b = picojson::ValueToType<unsigned char>::get(*bo);
			if (r.exists() && g.exists() && b.exists())
			{
				result.setValue(sad::Color(r.value(), g.value(), b.value()));
			}
		}
		return result;
	}
};

/*! Tries to converts specific value to color with alpha channel
 */
template<>
class ValueToType<sad::AColor>
{
public:
	/*! Tries to convert a picojson::value to color with alpha channel
		\param[in] v value
		\return a result (with value if any)
	 */
	static sad::Maybe<sad::AColor> get(const picojson::value & v)
	{
		sad::Maybe<sad::AColor> result;
		sad::Maybe<sad::Color> clr = picojson::ValueToType<sad::Color>::get(v);
		picojson::value const * ao = picojson::get_property(v, "a");
		if (clr.exists() && ao)
		{
			sad::Maybe<unsigned char> a = picojson::ValueToType<unsigned char>::get(*ao);
			if (a.exists())
			{
				sad::AColor res(
					clr.value().r(), 
					clr.value().g(),
					clr.value().b(),
					a.value()
				);
				result.setValue(res);
			}
		}
		return result;
	}
};

/*! Tries to converts specific value to point
 */
template<>
class ValueToType<sad::Point2D>
{
public:
	/*! Tries to convert a picojson::value to point
		\param[in] v value
		\return a result (with value if any)
	 */
	static sad::Maybe<sad::Point2D> get(const picojson::value & v)
	{
		sad::Maybe<sad::Point2D> result;
		picojson::value const * xo = picojson::get_property(v, "x");
		picojson::value const * yo = picojson::get_property(v, "y");
		if (xo && yo)
		{
			sad::Maybe<double> x = picojson::ValueToType<double>::get(*xo);
			sad::Maybe<double> y = picojson::ValueToType<double>::get(*yo);
			if (x.exists() && y.exists())
			{
				result.setValue(sad::Point2D(x.value(), y.value()));
			}
		}
		return result;
	}
};

/*! Tries to converts specific value to rect
 */
template<>
class ValueToType<sad::Rect2D>
{
public:
	/*! Tries to convert a picojson::value to point
		\param[in] v value
		\return a result (with value if any)
	 */
	static sad::Maybe<sad::Rect2D> get(const picojson::value & v)
	{
		sad::Maybe<sad::Rect2D> result;
		picojson::value const * p1o = picojson::get_property(v, "p1");
		picojson::value const * p2o = picojson::get_property(v, "p2");
		picojson::value const * p3o = picojson::get_property(v, "p3");
		picojson::value const * p4o = picojson::get_property(v, "p4");
		// First try to create rectangle by four points
		if (p1o && p2o && p3o && p4o)
		{
			sad::Maybe<sad::Point2D> p1 = picojson::ValueToType<sad::Point2D>::get(*p1o);
			sad::Maybe<sad::Point2D> p2 = picojson::ValueToType<sad::Point2D>::get(*p2o);
			sad::Maybe<sad::Point2D> p3 = picojson::ValueToType<sad::Point2D>::get(*p3o);
			sad::Maybe<sad::Point2D> p4 = picojson::ValueToType<sad::Point2D>::get(*p4o);
			if (p1.exists() && p2.exists() && p3.exists() && p4.exists())
			{
				result.setValue(sad::Rect2D(
					p1.value(), p2.value(), p3.value(), p4.value()
				));
			}
		}
		else
		{
			// Try to create rectangle by two points
			if (p1o && p3o)
			{
				sad::Maybe<sad::Point2D> p1 = picojson::ValueToType<sad::Point2D>::get(*p1o);
				sad::Maybe<sad::Point2D> p3 = picojson::ValueToType<sad::Point2D>::get(*p3o);
				if (p1.exists() && p3.exists())
				{
					result.setValue(sad::Rect2D(p1.value(), p3.value()));
				}
			}
		}
		return result;
	}
};
/*! Tries to convert picojson::value to type
	\param[in] v a value, to be converted
	\return value if any
 */
template<
	typename _Type
>
sad::Maybe<_Type> to_type(const picojson::value & v)
{
	return picojson::ValueToType<_Type>::get(v);	
}

/*! Tries to convert picojson::value to type
	\param[in] v a value, to be converted
	\return value if any
 */
template<
	typename _Type
>
sad::Maybe<_Type> to_type(picojson::value * v)
{
	sad::Maybe<_Type> result;
	if (v)
	{
		result = picojson::ValueToType<_Type>::get(*v);
	}
	return 	result;
}

}
