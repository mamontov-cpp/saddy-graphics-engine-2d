/*! \file valuetotype.h
	\author HiddenSeeker

	A simple helper for converting a picojson::value to specific type
 */
#pragma once
#include "getproperty.h"
#include "../../maybe.h"
#include "../../sadcolor.h"
#include "../../sadrect.h"
#include "../../sadsize.h"
#include "../../sadstring.h"
#include "../../dialogue/phrase.h"

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
	static inline sad::Maybe<_Type> get(const picojson::value & v)
	{
		return sad::Maybe<_Type>();
	}
};

/*! Tries to converts specific value to bool
 */
template<>
class ValueToType<bool>
{
public:
	/*! Tries to convert a picojson::value to char
		\param[in] v value
		\return a result (with value if any)
	 */
	static sad::Maybe<bool> get(const picojson::value & v)
	{
		sad::Maybe<bool> result;
		if (v.is<bool>())
		{
			bool b = v.get<bool>();
			result.setValue(b);
		}
		if (v.is<double>())
		{
			double a = v.get<double>();
			result.setValue(a > 1.0E-6 || a < -1.0E-6);
		}
		return result;
	}
};

/*! Tries to converts specific value to char
 */
template<>
class ValueToType<char>
{
public:
	/*! Tries to convert a picojson::value to char
		\param[in] v value
		\return a result (with value if any)
	 */
	static sad::Maybe<char> get(const picojson::value & v)
	{
		sad::Maybe<char> result;
		if (v.is<double>())
		{
			double a = v.get<double>();
			if (a >= -127.000001 && a <= 127.000001)
			{
				result.setValue(static_cast<char>(a));
			}
		}
		return result;
	}
};

/*! Tries to converts specific value to signed char
 */
template<>
class ValueToType<signed char>
{
public:
	/*! Tries to convert a picojson::value to char
		\param[in] v value
		\return a result (with value if any)
	 */
	static sad::Maybe<signed char> get(const picojson::value & v)
	{
		sad::Maybe<signed char> result;
		if (v.is<double>())
		{
			double a = v.get<double>();
			if (a >= -127.000001 && a <= 127.000001)
			{
				result.setValue(static_cast<signed char>(a));
			}
		}
		return result;
	}
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
				result.setValue(static_cast<unsigned char>(a));
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

/*! Tries to converts specific value to float
 */
template<>
class ValueToType<float>
{
public:
	/*! Tries to convert a picojson::value to float
		\param[in] v value
		\return a result (with value if any)
	 */
	static sad::Maybe<float> get(const picojson::value & v)
	{
		sad::Maybe<float> result;
		if (v.is<double>())
		{
			double a = v.get<double>();
			result.setValue(static_cast<float>(a));
		}
		return result;
	}
};

/*! Tries to converts specific value to string
 */
template<>
class ValueToType<std::string>
{
public:
	/*! Tries to convert a picojson::value to string
		\param[in] v value
		\return a result (with value if any)
	 */
	static sad::Maybe<std::string> get(const picojson::value & v)
	{
		sad::Maybe<std::string> result;
		if (v.is<std::string>())
		{
			result.setValue(v.get<std::string>());
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
		// A string conversion
		if (v.is<std::string>())
		{
			sad::String tmp = v.get<std::string>();
			sad::Vector<sad::String> parts = tmp.split(';');
			if (parts.size() != 3)
			{
				sad::uchar r = static_cast<unsigned char>(sad::String::toInt(parts[0]));
				sad::uchar g = static_cast<unsigned char>(sad::String::toInt(parts[1]));
				sad::uchar b = static_cast<unsigned char>(sad::String::toInt(parts[2]));
				result.setValue(sad::Color(r, g, b));
			}
			return result;
		}
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

/*! Tries to converts specific value to point
 */
template<>
class ValueToType<sad::Point3D>
{
public:
	/*! Tries to convert a picojson::value to point
		\param[in] v value
		\return a result (with value if any)
	 */
	static sad::Maybe<sad::Point3D> get(const picojson::value & v)
	{
		sad::Maybe<sad::Point3D> result;
		picojson::value const * xo = picojson::get_property(v, "x");
		picojson::value const * yo = picojson::get_property(v, "y");
		picojson::value const * zo = picojson::get_property(v, "z");
		if (xo && yo)
		{
			sad::Maybe<double> x = picojson::ValueToType<double>::get(*xo);
			sad::Maybe<double> y = picojson::ValueToType<double>::get(*yo);
			sad::Maybe<double> z = picojson::ValueToType<double>::get(*zo);
			if (x.exists() && y.exists() && z.exists())
			{
				result.setValue(sad::Point3D(x.value(), y.value(), z.value()));
			}
		}
		return result;
	}
};
/*! Tries to converts specific value to point
 */
template<>
class ValueToType<sad::Point2I>
{
public:
	/*! Tries to convert a picojson::value to point
		\param[in] v value
		\return a result (with value if any)
	 */
	static sad::Maybe<sad::Point2I> get(const picojson::value & v)
	{
		sad::Maybe<sad::Point2I> result;
		picojson::value const * xo = picojson::get_property(v, "x");
		picojson::value const * yo = picojson::get_property(v, "y");
		if (xo && yo)
		{
			sad::Maybe<double> x = picojson::ValueToType<double>::get(*xo);
			sad::Maybe<double> y = picojson::ValueToType<double>::get(*yo);
			if (x.exists() && y.exists())
			{
				result.setValue(sad::Point2I(static_cast<int>(x.value()), static_cast<int>(y.value())));
			}
		}
		return result;
	}
};

/*! Tries to converts specific value to point
 */
template<>
class ValueToType<sad::Point3I>
{
public:
	/*! Tries to convert a picojson::value to point
		\param[in] v value
		\return a result (with value if any)
	 */
	static sad::Maybe<sad::Point3I> get(const picojson::value & v)
	{
		sad::Maybe<sad::Point3I> result;
		picojson::value const * xo = picojson::get_property(v, "x");
		picojson::value const * yo = picojson::get_property(v, "y");
		picojson::value const * zo = picojson::get_property(v, "z");
		if (xo && yo)
		{
			sad::Maybe<double> x = picojson::ValueToType<double>::get(*xo);
			sad::Maybe<double> y = picojson::ValueToType<double>::get(*yo);
			sad::Maybe<double> z = picojson::ValueToType<double>::get(*zo);
			if (x.exists() && y.exists())
			{
				result.setValue(sad::Point3I(static_cast<int>(x.value()), static_cast<int>(y.value()), static_cast<int>(z.value())));
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
		// Conversion from string <x>;<y>;<width>;<height>
		if (v.is<std::string>())
		{
			sad::String stringtobeparsed = v.get<std::string>();
			sad::Vector<sad::String> parts = stringtobeparsed.split(';');
			if (parts.size() == 4)
			{
				double x = sad::String::toDouble(parts[0]);
				double y = sad::String::toDouble(parts[1]);
				double w = sad::String::toDouble(parts[2]);
				double h = sad::String::toDouble(parts[3]);
				sad::Point2D p(x, y);
				sad::Point2D wh(w, h);
				result.setValue(sad::Rect2D(p, p + wh));
			}
			return result;
		}
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

/*! Tries to converts specific value to rect
 */
template<>
class ValueToType<sad::Rect<sad::Point3D> >
{
public:
	/*! Tries to convert a picojson::value to point
		\param[in] v value
		\return a result (with value if any)
	 */
	static sad::Maybe<sad::Rect<sad::Point3D> > get(const picojson::value & v)
	{
		sad::Maybe<sad::Rect<sad::Point3D> > result;
		picojson::value const * p1o = picojson::get_property(v, "p1");
		picojson::value const * p2o = picojson::get_property(v, "p2");
		picojson::value const * p3o = picojson::get_property(v, "p3");
		picojson::value const * p4o = picojson::get_property(v, "p4");
		// First try to create rectangle by four points
		if (p1o && p2o && p3o && p4o)
		{
			sad::Maybe<sad::Point3D> p1 = picojson::ValueToType<sad::Point3D>::get(*p1o);
			sad::Maybe<sad::Point3D> p2 = picojson::ValueToType<sad::Point3D>::get(*p2o);
			sad::Maybe<sad::Point3D> p3 = picojson::ValueToType<sad::Point3D>::get(*p3o);
			sad::Maybe<sad::Point3D> p4 = picojson::ValueToType<sad::Point3D>::get(*p4o);
			if (p1.exists() && p2.exists() && p3.exists() && p4.exists())
			{
				result.setValue(sad::Rect<sad::Point3D>(
					p1.value(), p2.value(), p3.value(), p4.value()
				));
			}
		}
		else
		{
			// Try to create rectangle by two points
			if (p1o && p3o)
			{
				sad::Maybe<sad::Point3D> p1 = picojson::ValueToType<sad::Point3D>::get(*p1o);
				sad::Maybe<sad::Point3D> p3 = picojson::ValueToType<sad::Point3D>::get(*p3o);
				if (p1.exists() && p3.exists())
				{
					result.setValue(sad::Rect<sad::Point3D>(p1.value(), p3.value()));
				}
			}
		}
		return result;
	}
};

/*! Tries to converts specific value to rect
 */
template<>
class ValueToType<sad::Rect2I>
{
public:
	/*! Tries to convert a picojson::value to point
		\param[in] v value
		\return a result (with value if any)
	 */
	static sad::Maybe<sad::Rect2I> get(const picojson::value & v)
	{
		sad::Maybe<sad::Rect2I> result;
		// Conversion from string <x>;<y>;<width>;<height>
		if (v.is<std::string>())
		{
			sad::String stringtobeparsed = v.get<std::string>();
			sad::Vector<sad::String> parts = stringtobeparsed.split(';');
			if (parts.size() == 4)
			{
				double x = sad::String::toDouble(parts[0]);
				double y = sad::String::toDouble(parts[1]);
				double w = sad::String::toDouble(parts[2]);
				double h = sad::String::toDouble(parts[3]);
				sad::Point2I p(static_cast<int>(x), static_cast<int>(y));
				sad::Point2I wh(static_cast<int>(w), static_cast<int>(h));
				result.setValue(sad::Rect2I(p, p + wh));
			}
			return result;
		}
		picojson::value const * p1o = picojson::get_property(v, "p1");
		picojson::value const * p2o = picojson::get_property(v, "p2");
		picojson::value const * p3o = picojson::get_property(v, "p3");
		picojson::value const * p4o = picojson::get_property(v, "p4");
		// First try to create rectangle by four points
		if (p1o && p2o && p3o && p4o)
		{
			sad::Maybe<sad::Point2I> p1 = picojson::ValueToType<sad::Point2I>::get(*p1o);
			sad::Maybe<sad::Point2I> p2 = picojson::ValueToType<sad::Point2I>::get(*p2o);
			sad::Maybe<sad::Point2I> p3 = picojson::ValueToType<sad::Point2I>::get(*p3o);
			sad::Maybe<sad::Point2I> p4 = picojson::ValueToType<sad::Point2I>::get(*p4o);
			if (p1.exists() && p2.exists() && p3.exists() && p4.exists())
			{
				result.setValue(sad::Rect2I(
					p1.value(), p2.value(), p3.value(), p4.value()
				));
			}
		}
		else
		{
			// Try to create rectangle by two points
			if (p1o && p3o)
			{
				sad::Maybe<sad::Point2I> p1 = picojson::ValueToType<sad::Point2I>::get(*p1o);
				sad::Maybe<sad::Point2I> p3 = picojson::ValueToType<sad::Point2I>::get(*p3o);
				if (p1.exists() && p3.exists())
				{
					result.setValue(sad::Rect2I(p1.value(), p3.value()));
				}
			}
		}
		return result;
	}
};

/*! Tries to converts specific value to size
 */
template<>
class ValueToType<sad::Size2D>
{
public:
	/*! Tries to convert a picojson::value to point
		\param[in] v value
		\return a result (with value if any)
	 */
	static sad::Maybe<sad::Size2D> get(const picojson::value & v)
	{
		sad::Maybe<sad::Size2D> result;
		// Conversion from string <x>;<y>;<width>;<height>
		if (v.is<std::string>())
		{
			sad::String stringtobeparsed = v.get<std::string>();
			sad::Vector<sad::String> parts = stringtobeparsed.split(';');
			if (parts.size() == 2)
			{
				double w = sad::String::toDouble(parts[0]);
				double h = sad::String::toDouble(parts[1]);
				result.setValue(sad::Size2D(w, h));
			}
			return result;
		}
		picojson::value const * wo = picojson::get_property(v, "width");
		picojson::value const * ho = picojson::get_property(v, "height");
		// First try to create rectangle by four points
		if (wo && ho)
		{
			sad::Maybe<double> mw = picojson::ValueToType<double>::get(*wo);
			sad::Maybe<double> mh = picojson::ValueToType<double>::get(*ho);
			if (mw.exists() && mh.exists())
			{
				result.setValue(sad::Size2D(
					mw.value(), mh.value()
				));
			}
		}
		return result;
	}
};

/*! Tries to converts specific value to size
 */
template<>
class ValueToType<sad::Size2I>
{
public:
	/*! Tries to convert a picojson::value to point
		\param[in] v value
		\return a result (with value if any)
	 */
	static sad::Maybe<sad::Size2I> get(const picojson::value & v)
	{
		sad::Maybe<sad::Size2I> result;
		// Conversion from string <x>;<y>;<width>;<height>
		if (v.is<std::string>())
		{
			sad::String stringtobeparsed = v.get<std::string>();
			sad::Vector<sad::String> parts = stringtobeparsed.split(';');
			if (parts.size() == 2)
			{
				double w = sad::String::toDouble(parts[0]);
				double h = sad::String::toDouble(parts[1]);
				result.setValue(sad::Size2I(static_cast<int>(w), static_cast<int>(h)));
			}
			return result;
		}
		picojson::value const * wo = picojson::get_property(v, "width");
		picojson::value const * ho = picojson::get_property(v, "height");
		// First try to create rectangle by four points
		if (wo && ho)
		{
			sad::Maybe<double> mw = picojson::ValueToType<double>::get(*wo);
			sad::Maybe<double> mh = picojson::ValueToType<double>::get(*ho);
			if (mw.exists() && mh.exists())
			{
				result.setValue(sad::Size2I(
					static_cast<int>(mw.value()), static_cast<int>(mh.value())
				));
			}
		}
		return result;
	}
};

/*! Tries to converts specific value to vector of vector of colors with alpha-channel
 */
template<>
class ValueToType<sad::Vector<sad::Vector<sad::AColor> > >
{
public:
	/*! Tries to convert a picojson::value to point
		\param[in] v value
		\return a result (with value if any)
	 */
	static sad::Maybe<sad::Vector<sad::Vector<sad::AColor> > > get(const picojson::value & v)
	{
		sad::Maybe<sad::Vector<sad::Vector<sad::AColor> > > result;
		if (v.is<picojson::array>())
		{
			bool parseresult = true;
			sad::Vector<sad::Vector<sad::AColor> > tmpresult;
			const picojson::array & top = v.get<picojson::array>();
			for(size_t i = 0; i < top.size() && parseresult; i++)
			{
				const picojson::value & topentry = top[i];
				if (topentry.is<picojson::array>())
				{
					tmpresult << sad::Vector<sad::AColor>();
					const picojson::array & inner = topentry.get<picojson::array>();
					for(size_t j = 0; j < inner.size() && parseresult; j++)
					{
						sad::Maybe<sad::AColor> maybecolor = picojson::ValueToType<sad::AColor>::get(inner[j]);
						if (maybecolor.exists())
						{
							tmpresult[tmpresult.size() - 1] << maybecolor.value();
						}
						else
						{
							parseresult = false;
						}
					}
				}
				else
				{
					parseresult = false;
				}
			}
			if (parseresult)
			{
				result.setValue(tmpresult);
			}
		}
		return result;
	}
};


/*! Tries to converts specific value to vector of points
 */
template<>
class ValueToType<sad::Vector<sad::Point2D> >
{
public:
	/*! Tries to convert a picojson::value to point
		\param[in] v value
		\return a result (with value if any)
	 */
	static sad::Maybe<sad::Vector<sad::Point2D> > get(const picojson::value & v)
	{
		sad::Maybe<sad::Vector<sad::Point2D> > result;		
		if (v.is<picojson::array>())
		{
			bool parseresult = true;
			sad::Vector<sad::Point2D> tmpresult;
			const picojson::array & top = v.get<picojson::array>();
			for(size_t i = 0; i < top.size() && parseresult; i++)
			{
				const picojson::value & topentry = top[i];
				sad::Maybe<sad::Point2D> maybeentry = picojson::ValueToType<sad::Point2D>::get(top[i]);
				if (maybeentry.exists())
				{
					tmpresult << maybeentry.value();
				}
				else
				{
					parseresult = false;
				}				
			}
			if (parseresult)
			{
				result.setValue(tmpresult);
			}
		}
		return result;
	}
};

/*! Tries to converts specific value to rect
 */
template<>
class ValueToType<sad::dialogue::Phrase>
{
public:
	/*! Tries to convert a picojson::value to point
		\param[in] v value
		\return a result (with value if any)
	 */
	static sad::Maybe<sad::dialogue::Phrase> get(const picojson::value & v)
	{
		sad::Maybe<sad::dialogue::Phrase> result;		
		picojson::value const * no = picojson::get_property(v, "name");
		picojson::value const * po = picojson::get_property(v, "portrait");
		picojson::value const * pho = picojson::get_property(v, "phrase");
		picojson::value const * duo = picojson::get_property(v, "duration");
		picojson::value const * vho = picojson::get_property(v, "viewhint");		
		if (no && po && pho && duo && vho )
		{
			sad::Maybe<sad::String> p1 = picojson::ValueToType<sad::String>::get(*no);
			sad::Maybe<sad::String> p2 = picojson::ValueToType<sad::String>::get(*po);
			sad::Maybe<sad::String> p3 = picojson::ValueToType<sad::String>::get(*pho);
			sad::Maybe<double> p4 = picojson::ValueToType<double>::get(*duo);
			sad::Maybe<sad::String> p5 = picojson::ValueToType<sad::String>::get(*vho);			
			if (p1.exists() && p2.exists() && p3.exists() && p4.exists() && p5.exists())
			{
				sad::dialogue::Phrase tmp;
				tmp.setActorName(p1.value());
				tmp.setActorPortrait(p2.value());
				tmp.setPhrase(p3.value());
				tmp.setDuration(p4.value());
				tmp.setViewHint(p5.value());
				
				result.setValue(tmp);
			}
		}		
		return result;
	}
};

/*! Tries to converts specific value to vector of points
 */
template<>
class ValueToType<sad::Vector<sad::dialogue::Phrase*> >
{
public:
	/*! Tries to convert a picojson::value to point
		\param[in] v value
		\return a result (with value if any)
	 */
	static sad::Maybe<sad::Vector<sad::dialogue::Phrase*> > get(const picojson::value & v)
	{
		sad::Maybe<sad::Vector<sad::dialogue::Phrase*> > result;		
		if (v.is<picojson::array>())
		{
			bool parseresult = true;
			sad::Vector<sad::dialogue::Phrase*> tmpresult;
			const picojson::array & top = v.get<picojson::array>();
			for(size_t i = 0; i < top.size() && parseresult; i++)
			{
				const picojson::value & topentry = top[i];
				sad::Maybe<sad::dialogue::Phrase> maybeentry = picojson::ValueToType<sad::dialogue::Phrase>::get(top[i]);
				if (maybeentry.exists())
				{
					tmpresult << new sad::dialogue::Phrase(maybeentry.value());
				}
				else
				{
					parseresult = false;
				}				
			}
			if (parseresult)
			{
				result.setValue(tmpresult);
			}
			else
			{
				for(size_t i = 0; i < tmpresult.size(); i++)
				{
					delete tmpresult[i];
				}
			}	
		}
		return result;
	}
};

/*! Creates ValueToType specialization via cast from double fo specified signed type
 */
#define CREATE_VALUE_TO_TYPE_SPECIALIZATION_FOR_SIGNED_TYPE_VIA_CAST_FROM_DOUBLE(TYPE)   \
template<>                                                     \
class ValueToType< TYPE >                                      \
{                                                              \
public:                                                        \
	static inline sad::Maybe< TYPE > get(const picojson::value & v)  \
	{                                                          \
		sad::Maybe<TYPE> result;                               \
		if (v.is<double>())                                    \
		{                                                      \
			double a = v.get<double>();                        \
			result.setValue((TYPE) a);                         \
		}                                                      \
		return result;                                         \
	}                                                          \
};

CREATE_VALUE_TO_TYPE_SPECIALIZATION_FOR_SIGNED_TYPE_VIA_CAST_FROM_DOUBLE(short)
CREATE_VALUE_TO_TYPE_SPECIALIZATION_FOR_SIGNED_TYPE_VIA_CAST_FROM_DOUBLE(int)
CREATE_VALUE_TO_TYPE_SPECIALIZATION_FOR_SIGNED_TYPE_VIA_CAST_FROM_DOUBLE(long)
CREATE_VALUE_TO_TYPE_SPECIALIZATION_FOR_SIGNED_TYPE_VIA_CAST_FROM_DOUBLE(long long)

/*! Creates ValueToType specialization via cast from double fo specified unsigned type
 */
#define CREATE_VALUE_TO_TYPE_SPECIALIZATION_FOR_UNSIGNED_TYPE_VIA_CAST_FROM_DOUBLE(TYPE)   \
template<>                                                                                 \
class ValueToType< TYPE >                                                                  \
{                                                                                          \
public:                                                                                    \
	static inline sad::Maybe< TYPE > get(const picojson::value & v)                        \
	{                                                                                      \
		sad::Maybe< TYPE > result;                                                         \
		if (v.is<double>())                                                                \
		{                                                                                  \
			double a = v.get< double >();                                                  \
			if (a >= -0.000001)                                                            \
			{                                                                              \
				result.setValue(( TYPE ) a);                                               \
			}                                                                              \
		}                                                                                  \
		return result;                                                                     \
	}                                                                                      \
};

CREATE_VALUE_TO_TYPE_SPECIALIZATION_FOR_UNSIGNED_TYPE_VIA_CAST_FROM_DOUBLE(unsigned short)
CREATE_VALUE_TO_TYPE_SPECIALIZATION_FOR_UNSIGNED_TYPE_VIA_CAST_FROM_DOUBLE(unsigned int)
CREATE_VALUE_TO_TYPE_SPECIALIZATION_FOR_UNSIGNED_TYPE_VIA_CAST_FROM_DOUBLE(unsigned long)
CREATE_VALUE_TO_TYPE_SPECIALIZATION_FOR_UNSIGNED_TYPE_VIA_CAST_FROM_DOUBLE(unsigned long long)



/*! Tries to converts specific value to vector of unsigned long longs
 */
template<>
class ValueToType<sad::Vector<unsigned long long> >
{
public:
	/*! Tries to convert a picojson::value to vector of unsigned long longs
		\param[in] v value
		\return a result (with value if any)
	 */
	static sad::Maybe<sad::Vector<unsigned long long> > get(const picojson::value & v)
	{
		sad::Maybe<sad::Vector<unsigned long long> > result;		
		if (v.is<picojson::array>())
		{
			bool parseresult = true;
			sad::Vector<unsigned long long> tmpresult;
			const picojson::array & top = v.get<picojson::array>();
			for(size_t i = 0; i < top.size() && parseresult; i++)
			{
				const picojson::value & topentry = top[i];
				sad::Maybe<unsigned long long> maybeentry = picojson::ValueToType<unsigned long long>::get(top[i]);
				if (maybeentry.exists())
				{
					tmpresult << maybeentry.value();
				}
				else
				{
					parseresult = false;
				}				
			}
			if (parseresult)
			{
				result.setValue(tmpresult);
			}
		}
		return result;
	}
};


/*! Tries to converts specific value to vector of sad::String
 */
template<>
class ValueToType<sad::Vector<sad::String> >
{
public:
	/*! Tries to convert a picojson::value to vector of sad::String
		\param[in] v value
		\return a result (with value if any)
	 */
	static sad::Maybe<sad::Vector<sad::String> > get(const picojson::value & v)
	{
		sad::Maybe<sad::Vector<sad::String> > result;		
		if (v.is<picojson::array>())
		{
			bool parseresult = true;
			sad::Vector<sad::String> tmpresult;
			const picojson::array & top = v.get<picojson::array>();
			for(size_t i = 0; i < top.size() && parseresult; i++)
			{
				const picojson::value & topentry = top[i];
				sad::Maybe<sad::String> maybeentry = picojson::ValueToType<sad::String>::get(top[i]);
				if (maybeentry.exists())
				{
					tmpresult << maybeentry.value();
				}
				else
				{
					parseresult = false;
				}				
			}
			if (parseresult)
			{
				result.setValue(tmpresult);
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
sad::Maybe<_Type> to_type(picojson::value const * v)
{
	sad::Maybe<_Type> result;
	if (v)
	{
		result = picojson::ValueToType<_Type>::get(*v);
	}
	return 	result;
}

/*! Returns property value for object if such property exists and
    it can be converted to this type
    \param[in] v value
    \param[in] propertyname name of property
    \return property value
 */ 
template<
    typename _Type
>
sad::Maybe<_Type> get_property(
    const picojson::value& v,
    const sad::String& propertyname
)
{
    sad::Maybe<_Type> result;
    if (v.is<picojson::object>())
	{
        picojson::value const * propvalue = picojson::get_property(v, propertyname);
        if (propvalue)
        {
            result = picojson::to_type<_Type>(propvalue);
        }
    }
    return result;
}

}
