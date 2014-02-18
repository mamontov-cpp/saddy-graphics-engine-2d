#pragma warning(push)
#pragma warning(disable: 4273)
#pragma warning(disable: 4351)
#include <cstdio>
#define _INC_STDIO
#include "3rdparty/picojson/picojson.h"
#include "3rdparty/tpunit++/tpunit++.hpp"
#include <sstream>
#include <sadstring.h>
#pragma warning(pop)

/*!
 * Tests sad::Geometry2DTest
 */
struct PicoJSONTest : tpunit::TestFixture
{
 public:
   PicoJSONTest() : tpunit::TestFixture(
	   TEST(PicoJSONTest::testParse),
	   TEST(PicoJSONTest::testSerialize)
	) {}

   void testParse()
   {
	   picojson::value v;
	   v = picojson::parse_string("");

	   std::string result = picojson::get_last_error();
	   ASSERT_TRUE(v.is<picojson::null>());

	   v = picojson::parse_string("[");
	   ASSERT_TRUE(v.is<picojson::null>());

	   v = picojson::parse_string("null");
	   ASSERT_TRUE(v.is<picojson::null>());

	   v = picojson::parse_string("22");
	   ASSERT_TRUE(v.is<double>());
	   ASSERT_FLOAT_EQUAL(v.get<double>(), 22);	

	   v = picojson::parse_string("-22");
	   ASSERT_TRUE(v.is<double>());
	   ASSERT_FLOAT_EQUAL(v.get<double>(), -22);	

	   v = picojson::parse_string("true");
	   ASSERT_TRUE(v.is<bool>());
	   ASSERT_TRUE(v.get<bool>() == true);

	   v = picojson::parse_string("false");
	   ASSERT_TRUE(v.is<bool>());
	   ASSERT_TRUE(v.get<bool>() == false);

	   v = picojson::parse_string("\"\"");
	   ASSERT_TRUE(v.is<std::string>());
	   ASSERT_TRUE(v.get<std::string>() == "");

	   v = picojson::parse_string("\"test\"");
	   ASSERT_TRUE(v.is<std::string>());
	   ASSERT_TRUE(v.get<std::string>() == "test");

	   v = picojson::parse_string("[0, 1, 2, 3]");
	   ASSERT_TRUE(v.is<picojson::array>());
	   ASSERT_TRUE(v.get<picojson::array>().size() != 0);
	   ASSERT_FLOAT_EQUAL(v.get<picojson::array>()[0].get<double>(), 0);
	   ASSERT_FLOAT_EQUAL(v.get<picojson::array>()[1].get<double>(), 1);
	   ASSERT_FLOAT_EQUAL(v.get<picojson::array>()[2].get<double>(), 2);
	   ASSERT_FLOAT_EQUAL(v.get<picojson::array>()[3].get<double>(), 3);

	   v = picojson::parse_string("{\"test\":  1}");
	   ASSERT_TRUE(v.is<picojson::object>());
	   ASSERT_FLOAT_EQUAL(v.get("test").get<double>(), 1);
   }

   void testSerialize()
   {
	   picojson::value object1(picojson::object_type, false);
	   picojson::value object2(picojson::object_type, false);
	   picojson::value object3(picojson::array_type, false);
	   object3.push_back(picojson::value("11"));
	   object3.push_back(picojson::value(22.22));
	   object3.push_back(picojson::value(false));

	   object2.insert("a", picojson::value("11"));
	   object2.insert("b", object3);
	   object1.insert("11", object2);
	   sad::String json = object1.serialize();
	   sad::String result = "{\"11\":{\"a\":\"11\",\"b\":[\"11\",22.220000,false]}}";
	   json.replaceAllOccurences("\n", "");
	   json.replaceAllOccurences("\r", "");
	   json.replaceAllOccurences(" ", "");
	   ASSERT_TRUE(json == result);	   
   }

} _picojson_test; 