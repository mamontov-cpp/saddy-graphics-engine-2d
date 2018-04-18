#pragma once
#include <sadstring.h>

using namespace std;
namespace game 
{

class Item 
{
public:
	Item(const sad::String&, const sad::String&);
	virtual ~Item();
protected:
	sad::String m_icon;
	sad::String m_title;
};

}