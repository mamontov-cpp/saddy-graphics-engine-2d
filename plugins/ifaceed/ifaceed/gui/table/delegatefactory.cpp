#include "delegatefactory.h"
#include "doubledelegate.h"
#include "floatdelegate.h"
#include "intdelegate.h"
#include "longdelegate.h"
#include "longlongdelegate.h"
#include "sadacolordelegate.h"
#include "sadcolordelegate.h"
#include "sadstringdelegate.h"
#include "shortdelegate.h"
#include "signedchardelegate.h"
#include "unsignedchardelegate.h"
#include "unsignedintdelegate.h"
#include "unsignedlongdelegate.h"
#include "unsignedlonglongdelegate.h"
#include "unsignedshortdelegate.h"


gui::table::DelegateFactory::AbstractDelegate::~AbstractDelegate()
{
	
}


gui::table::DelegateFactory::DelegateFactory()
{
	this->insert<gui::table::SignedCharDelegate>("signed char");
	this->insert<gui::table::CharDelegate>("char");
	this->insert<gui::table::UnsignedCharDelegate>("unsigned char");

	this->insert<gui::table::ShortDelegate>("short");
	this->insert<gui::table::UnsignedShortDelegate>("unsigned short");

	this->insert<gui::table::IntDelegate>("int");
	this->insert<gui::table::UnsignedIntDelegate>("unsigned int");

	this->insert<gui::table::LongDelegate>("long");
	this->insert<gui::table::UnsignedLongDelegate>("unsigned long");

	this->insert<gui::table::LongLongDelegate>("long long");
	this->insert<gui::table::UnsignedLongLongDelegate>("unsigned long long");

	this->insert<gui::table::FloatDelegate>("float");
	this->insert<gui::table::DoubleDelegate>("double");

	this->insert<gui::table::SadStringDelegate>("sad::String");
	this->insert<gui::table::SadColorDelegate>("sad::Color");
	this->insert<gui::table::SadAColorDelegate>("sad::AColor");
}

gui::table::DelegateFactory::~DelegateFactory()
{
	
}

gui::table::Delegate* gui::table::DelegateFactory::create(const QString & s)
{
	gui::table::Delegate* result = NULL;
	if (m_delegates.contains(s.toStdString()))
	{
		result = m_delegates[s.toStdString()]->create();
	}
	return result;
}
