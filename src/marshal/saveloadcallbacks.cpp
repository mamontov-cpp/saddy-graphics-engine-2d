#include "marshal/saveloadcallbacks.h"
#include <sadstring.h>

#define MAX_TABLE 7
char table_from[MAX_TABLE][10]={"&quot;","&amp;","&apos;","&lt;","&gt;","&#10;","&#13"};
char table_to  [MAX_TABLE][10]={"\""    ,"&"    ,"\'"    ,"<"   ,">",   "\n"   ,"\r"  };


static sad::String convert(const sad::String & str, char from[][10], char to[][10],int max = MAX_TABLE)
{
	sad::String result=str;
	for (int i=0;i<max;i++)
	{
	 result.replaceAllOccurences(from[i],to[i]);
	}
	return result;
}

sad::String SaveLoadCallback<sad::String>::load(ActionContext * context,
												const sad::String & str, 
												const sad::String & typestring)
{
	return convert(str,table_from,table_to);
}

sad::String SaveLoadCallback<sad::String>::save(const sad::String & obj)
{
	return convert(obj,table_to,table_from);
}


sad::Vector<int> SaveLoadCallback< sad::Vector<int> >::load(ActionContext * context,
															const sad::String & str,
															const sad::String & typestring)
{
	sad::Vector<int> result;
	sad::StringList lst=str.split(';');
	for (size_t i=0;i<lst.count();i++)
	{
		result<<SaveLoadCallback<int>::load(context,lst[i],
										    abstract_names::type_string< sad::Vector<int> >::type());
	}
	return result;
}

sad::String SaveLoadCallback< sad::Vector<int> >::save(const sad::Vector<int> & obj)
{
	if (obj.count()==0)
		return sad::String();
	sad::String result = SaveLoadCallback<int>::save(obj[0]);
	for (unsigned int i=1; i< obj.count();i++)
	{
	  result << ";";
	  result << SaveLoadCallback<int>::save(obj[i]);
	}
	return result;
}


hPointF SaveLoadCallback<hPointF>::load(ActionContext * context,
										const sad::String & str, 
										const sad::String & typestring)
{
	sad::StringList lst=str.split('@');
	if (lst.count()!=2)
		throw new serializable::InvalidPropertyValue(typestring,str,context);

	double params[2];
	for (int i=0;i<2;i++)
	{
		params[i]=SaveLoadCallback<double>::load(context,lst[i],typestring);
	}

	return hPointF(params[0],params[1]);
}

sad::String SaveLoadCallback<hPointF>::save(const hPointF & obj)
{
	return SaveLoadCallback<double>::save(obj.x()) + sad::String("@") + SaveLoadCallback<double>::save(obj.y());
}


hRectF  SaveLoadCallback<hRectF>::load(ActionContext * context,
									   const sad::String & str, 
									   const sad::String & typestring)
{
	sad::StringList lst=str.split(':');
	if (lst.count()!=4)
		throw new serializable::InvalidPropertyValue(typestring,str,context);

	hPointF params[4];
	for (int i=0;i<4;i++)
	{
		params[i]=SaveLoadCallback<hPointF>::load(context,lst[i],typestring);
	}

	return hRectF(params[0],params[1],params[2],params[3]);
}

sad::String SaveLoadCallback<hRectF>::save(const hRectF & obj)
{
	sad::String result = SaveLoadCallback<hPointF>::save(obj[0]);
	for (int i=1;i<4;i++)
	{
		result << sad::String(":");
		result << SaveLoadCallback<hPointF>::save(obj[i]);
	}
	return result;
}

sad::Vector<hPointF>  SaveLoadCallback< sad::Vector<hPointF> >::load(ActionContext * context,
																	 const sad::String & str, 
																	 const sad::String & typestring)
{
	sad::Vector<hPointF> result;
	sad::StringList lst=str.split(';');
	for (size_t i=0;i<lst.count();i++)
	{
		result<<SaveLoadCallback<hPointF>::load(context,lst[i],
										        abstract_names::type_string< sad::Vector<int> >::type());
	}
	return result;
}

sad::String SaveLoadCallback< sad::Vector<hPointF> >::save(const sad::Vector<hPointF> & obj)
{
	if (obj.count()==0)
		return sad::String();
	sad::String result = SaveLoadCallback<hPointF>::save(obj[0]);
	for (unsigned int i=1; i< obj.count();i++)
	{
	  result << ";";
	  result << SaveLoadCallback<hPointF>::save(obj[i]);
	}
	return result;
}


bool  SaveLoadCallback< bool >::load(ActionContext * context,
									 const sad::String & str, 
									 const sad::String & typestring)
{
	if (str=="true") return true;
	return false;
}

sad::String SaveLoadCallback< bool >::save(const bool & obj)
{
	if (obj)
		return "true";
	return "false";
}

hst::color SaveLoadCallback< hst::color >::load(ActionContext * context,
									 const sad::String & str, 
									 const sad::String & typestring)
{
	sad::StringList lst=str.split(';');
	int r, g, b ;
	if (lst.count()==3)
	{
		r = SaveLoadCallback<int>::load(context,lst[0], abstract_names::type_string<hst::color >::type());
		g = SaveLoadCallback<int>::load(context,lst[1], abstract_names::type_string<hst::color >::type());
		b = SaveLoadCallback<int>::load(context,lst[2], abstract_names::type_string<hst::color >::type());
	}
	else 
	{
		throw new serializable::InvalidPropertyValue(typestring,str,context);
	}
	return hst::color(r,g,b);
}

sad::String SaveLoadCallback< hst::color >::save(const hst::color & obj)
{
	sad::String t(";");
	return SaveLoadCallback<int>::save(obj.r()) + t + 
		   SaveLoadCallback<int>::save(obj.g()) + t +
		   SaveLoadCallback<int>::save(obj.b());
}
