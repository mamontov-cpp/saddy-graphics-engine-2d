#include "sadhash.h"

// ===============================  sad::hash_value ===============================

std::size_t sad::hash_value(const sad::String& s)
{
	boost::hash<std::string> hasher;
	return hasher(s);
}

std::size_t sad::hash_value(const sad::WString& s)
{
	boost::hash<std::wstring> hasher;
	return hasher(s);
}

// ===============================================================================