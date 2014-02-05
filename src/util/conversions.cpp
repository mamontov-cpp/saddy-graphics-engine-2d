#include "util/conversions.h"

bool util::parse_uchar(const picojson::value & v, unsigned char & c)
{
	bool result = false;
	if (v.is<double>())
	{
		double a = v.get<double>();
		if (a >= -0.000001 && a <= 255.000001)
		{
			c = (unsigned char)a;
			result = true;
		}
	}
	return result;
}


bool util::parse_color(const picojson::value & v, sad::Color & c)
{
	bool result = false;
	if (v.is<picojson::object>())
	{
		const picojson::object & color = v.get<picojson::object>();
		picojson::object::const_iterator r = color.find("r");
		picojson::object::const_iterator g = color.find("g");
		picojson::object::const_iterator b = color.find("b");
		if (r != color.end() && g != color.end() && b != color.end())
		{
			unsigned char rb = 0;
			unsigned char gb = 0;
			unsigned char bb = 0;
			if (util::parse_uchar(r->second, rb) 
				&& util::parse_uchar(g->second, gb)
				&& util::parse_uchar(b->second, bb))
			{
				result = true;
				c.setR(rb);
				c.setG(gb);
				c.setB(bb);
			}
		}
	}
	return result;
}
