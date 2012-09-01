#include "path.h"
#include <templates/hstringlist.h>

hst::string path::directory(const hst::string & path)
{
	if (path.length() == 0)
		return hst::string();
	hst::string escaped = path;
	if (escaped[escaped.length()-1] == '/')
		escaped.removeLastOccurence("/");
	hst::stringlist pathparts = escaped.split('/');
	pathparts.remove(pathparts.length()-1);
	if (pathparts.length() == 0) 
		return hst::string();
	hst::string result = pathparts[0];
	for (unsigned int i=1;i<pathparts.length();i++) {
		result << "/" << pathparts[i];
	}
	return result;
}

hst::string path::concat(const hst::string & parent,const hst::string & path)
{
	if (parent.length() ==0){
		return path;
	}
	hst::string escaped = parent;
	if (escaped[escaped.length()-1] == '/')
		escaped.removeLastOccurence("/");
	hst::string escpath = path;
	if (escpath.length() == 0)
		return hst::string();
	if (escpath[0] == '/')
		escpath.remove(0);
	return escaped + "/" + escpath;
}