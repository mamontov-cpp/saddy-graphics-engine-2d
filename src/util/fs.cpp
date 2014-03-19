#include "util/fs.h"

#ifdef WIN32
	#include <windows.h>
#endif

sad::String sad::util::pathDelimiter()
{
#ifdef WIN32
	return "\\";
#endif

#ifdef LINUX
	return "/";
#endif
}


bool sad::util::isAbsolutePath(const sad::String & path)
{
	bool result = false;
	if (path.length() != 0)
	{

#ifdef WIN32
	if(path.length() > 1)
	{
		result = ((path[0] >= 'A' && path[0] <= 'Z') || (path[0] >= 'a' && path[0] <= 'z')) && (path[1] == ':');
	}
#endif

#ifdef LINUX
		result = path[0] == '/';
#endif
	}
	return result;
}


sad::String sad::util::concatPaths(const sad::String & parent,const sad::String & path)
{
	if (parent.length() ==0){
		return path;
	}
	sad::String escaped = parent;
	// Handle windows path
	if (escaped.getOccurence("\\")!=-1) 
	{
		if (escaped[escaped.length()-1] == '\\')
			escaped.removeLastOccurence("\\");
		sad::String escpath = path;
		if (escpath.getOccurence("/")!=-1)
			escpath.replaceAllOccurences("/","\\");
		if (escpath[0] == '\\')
			escpath.remove(0);
		return escaped + "\\" + escpath;
	}

	if (escaped[escaped.length()-1] == '/')
		escaped.removeLastOccurence("/");
	sad::String escpath = path;
	// Replace old path with new
	escpath.replaceAllOccurences("\\","/");
	if (escpath.length() == 0)
		return sad::String();
	if (escpath[0] == '/')
		escpath.remove(0);
	return escaped + "/" + escpath;
}
