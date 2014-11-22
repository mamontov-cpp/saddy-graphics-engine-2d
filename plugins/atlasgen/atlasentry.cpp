#include "atlasentry.h"

AtlasEntry::AtlasEntry()
{

}


bool AtlasEntry::isValid() const
{
	return InputTextureName.exists() && Name.exists();
}


bool AtlasEntry::canBeWritten() const
{
	return OutputTextureName.exists() && TextureRectangle.exists() && Size.exists();
}


int AtlasEntry::index() const
{
	if (Index.exists())
	{
		return Index.value();
	}
	return 0;
}


QString AtlasEntry::getFullName() const
{
	QString result = Name.value();
	if (Index.exists())
	{
		result += ":";
		result += QString::number(Index.value());
	}
	return result; 
}

