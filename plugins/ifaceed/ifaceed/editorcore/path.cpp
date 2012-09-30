#include "path.h"
#include <templates/hstringlist.h>

hst::string path::directory(const hst::string & path)
{
	if (path.length() == 0)
		return hst::string();
	hst::string escaped = path;
	// Extract path when on windows
	char delimiter[2]="/";
	if (escaped.getOccurence("\\")!=-1) 
	{
		delimiter[0]='\\';
	}

	if (escaped[escaped.length()-1] == delimiter[0])
		escaped.removeLastOccurence(delimiter);
	hst::stringlist pathparts = escaped.split(delimiter[0]);
	pathparts.remove(pathparts.length()-1);
	if (pathparts.length() == 0) 
		return hst::string();
	hst::string result = pathparts[0];
	for (unsigned int i=1;i<pathparts.length();i++) {
		result << delimiter << pathparts[i];
	}
	return result;
}

hst::string path::concat(const hst::string & parent,const hst::string & path)
{
	if (parent.length() ==0){
		return path;
	}
	hst::string escaped = parent;
	// Handle windows path
	if (escaped.getOccurence("\\")!=-1) 
	{
		if (escaped[escaped.length()-1] == '\\')
			escaped.removeLastOccurence("\\");
		hst::string escpath = path;
		if (escpath.getOccurence("/")!=-1)
			escpath.replaceAllOccurences("/","\\");
		if (escpath[0] == '\\')
			escpath.remove(0);
		return escaped + "\\" + escpath;
	}

	if (escaped[escaped.length()-1] == '/')
		escaped.removeLastOccurence("/");
	hst::string escpath = path;
	if (escpath.length() == 0)
		return hst::string();
	if (escpath[0] == '/')
		escpath.remove(0);
	return escaped + "/" + escpath;
}