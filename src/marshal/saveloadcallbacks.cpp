#include "marshal/saveloadcallbacks.h"
#include <templates/hstringlist.h>

#define MAX_TABLE 7
char table_from[MAX_TABLE][10]={"&quot;","&amp;","&apos;","&lt;","&gt;","&#10;","&#13"};
char table_to  [MAX_TABLE][10]={"\""    ,"&"    ,"\'"    ,"<"   ,">",   "\n"   ,"\r"  };


static hst::string convert(const hst::string & str, char from[][10], char to[][10],int max = MAX_TABLE)
{
	hst::string result=str;
	for (int i=0;i<max;i++)
	{
	 result.replaceAllOccurences(from[i],to[i]);
	}
	return result;
}

hst::string SaveLoadCallback<hst::string>::load(ActionContext * context,
												const hst::string & str, 
												const hst::string & typestring)
{
	return convert(str,table_from,table_to);
}

hst::string SaveLoadCallback<hst::string>::save(const hst::string & obj)
{
	return convert(obj,table_to,table_from);
}


hst::vector<int> SaveLoadCallback< hst::vector<int> >::load(ActionContext * context,
															const hst::string & str,
															const hst::string & typestring)
{
	hst::vector<int> result;
	hst::stringlist lst=str.split(';');
	for (int i=0;i<lst.length();i++)
	{
		result<<SaveLoadCallback<int>::load(context,lst[i],
										    abstract_names::type_string< hst::vector<int> >::type());
	}
	return result;
}

hst::string SaveLoadCallback< hst::vector<int> >::save(const hst::vector<int> & obj)
{
	if (obj.count()==0)
		return hst::string();
	hst::string result = SaveLoadCallback<int>::save(obj[0]);
	for (unsigned int i=1; i< obj.count();i++)
	{
	  result << ";";
	  result << SaveLoadCallback<int>::save(obj[i]);
	}
	return result;
}


hPointF SaveLoadCallback<hPointF>::load(ActionContext * context,
										const hst::string & str, 
										const hst::string & typestring)
{
	hst::stringlist lst=str.split('@');
	if (lst.length()!=2)
		throw new serializable::InvalidPropertyValue(typestring,str,context);

	double params[2];
	for (int i=0;i<2;i++)
	{
		params[i]=SaveLoadCallback<double>::load(context,lst[i],typestring);
	}

	return hPointF(params[0],params[1]);
}

hst::string SaveLoadCallback<hPointF>::save(const hPointF & obj)
{
	return SaveLoadCallback<double>::save(obj.x()) + hst::string("@") + SaveLoadCallback<double>::save(obj.y());
}


hRectF  SaveLoadCallback<hRectF>::load(ActionContext * context,
									   const hst::string & str, 
									   const hst::string & typestring)
{
	hst::stringlist lst=str.split(':');
	if (lst.length()!=4)
		throw new serializable::InvalidPropertyValue(typestring,str,context);

	hPointF params[4];
	for (int i=0;i<4;i++)
	{
		params[i]=SaveLoadCallback<hPointF>::load(context,lst[i],typestring);
	}

	return hRectF(params[0],params[1],params[2],params[3]);
}

hst::string SaveLoadCallback<hRectF>::save(const hRectF & obj)
{
	hst::string result = SaveLoadCallback<hPointF>::save(obj[0]);
	for (int i=1;i<4;i++)
	{
		result << hst::string(":");
		result << SaveLoadCallback<hPointF>::save(obj[i]);
	}
	return result;
}

hst::vector<hPointF>  SaveLoadCallback< hst::vector<hPointF> >::load(ActionContext * context,
																	 const hst::string & str, 
																	 const hst::string & typestring)
{
	hst::vector<hPointF> result;
	hst::stringlist lst=str.split(';');
	for (int i=0;i<lst.length();i++)
	{
		result<<SaveLoadCallback<hPointF>::load(context,lst[i],
										        abstract_names::type_string< hst::vector<int> >::type());
	}
	return result;
}

hst::string SaveLoadCallback< hst::vector<hPointF> >::save(const hst::vector<hPointF> & obj)
{
	if (obj.count()==0)
		return hst::string();
	hst::string result = SaveLoadCallback<hPointF>::save(obj[0]);
	for (unsigned int i=1; i< obj.count();i++)
	{
	  result << ";";
	  result << SaveLoadCallback<hPointF>::save(obj[i]);
	}
	return result;
}


bool  SaveLoadCallback< bool >::load(ActionContext * context,
									 const hst::string & str, 
									 const hst::string & typestring)
{
	if (str=="true") return true;
	return false;
}

hst::string SaveLoadCallback< bool >::save(const bool & obj)
{
	if (obj)
		return "true";
	return "false";
}