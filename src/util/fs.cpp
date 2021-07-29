#include "util/fs.h"

#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

#ifndef _WIN32
    #include <unistd.h>
#endif

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


sad::String sad::util::canonicalizePath(const sad::String& path)
{
    sad::String escaped = path;
    sad::String delimiter = "/";
    if (escaped.getOccurrence("\\")!=-1) 
    {
        if (escaped.getOccurrence("/")!=-1)
            escaped.replaceAllOccurrences("/","\\");
        delimiter = "\\";
    }
    else
    {
        escaped.replaceAllOccurrences("\\", "/");
    }
    sad::StringList list = escaped.split(delimiter[0]);
    bool isAbsolute = sad::util::isAbsolutePath(path);
    sad::StringList resultinglist;
    for(size_t i = 0; i < list.size(); i++)
    {
        if (list[i] != ".")
        {
            if (list[i] != "..")
            {
                resultinglist << list[i];
            } 
            else
            {
                if (resultinglist.size() >= 2)
                {
                    bool everything_is_go_upper = !isAbsolute;
                    if (everything_is_go_upper)
                    {
                        for(size_t i = 0; i < resultinglist.size(); i++)
                        {
                            everything_is_go_upper = everything_is_go_upper && resultinglist[i] == "..";
                        }
                    }
                    if (!everything_is_go_upper) 
                    {
                        resultinglist.removeAt(resultinglist.size() - 1);
                    } 
                    else
                    {
                        resultinglist << "..";
                    }
                } 
                else
                {
                    if (resultinglist.size() == 0)
                    {
                        if (!isAbsolute)
                        {
                            resultinglist << list[i];
                        }
                    } 
                    else
                    {
                        if (resultinglist.size() == 1)
                        {
                            if (!isAbsolute)
                            {
                                if (resultinglist[0] != "..") 
                                {
                                    resultinglist.removeAt(0);
                                } 
                                else 
                                {
                                    resultinglist << list[i];
                                }
                            }
                            else
                            {
                                if (delimiter == "/")
                                {
                                    resultinglist.removeAt(0);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    sad::String result = sad::join(resultinglist, delimiter);
    bool isLinuxAbsolutePath = false;
    if (path.length() != 0)
    {
        isLinuxAbsolutePath = path[0] == '/';
    }
    if (isAbsolute && isLinuxAbsolutePath && delimiter == "/")
    {
        if (result.length() != 0)
        {
            if (result[0] != '/')
            {
                result.insert('/', 0);
            }
        }
        else
        {
            result = "/";
        }
    }
    return result;
}

bool sad::util::isAbsolutePath(const sad::String & path)
{
    bool result = false;
    if (path.length() != 0)
    {
        // Window absolute path template
        if(path.length() > 1)
        {
            result = ((path[0] >= 'A' && path[0] <= 'Z') || (path[0] >= 'a' && path[0] <= 'z')) && (path[1] == ':');
        }
        // Linux absolute path template
        if (!result) 
        {
            result = path[0] == '/';
        }
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
    if (escaped.getOccurrence("\\")!=-1) 
    {
        if (escaped[escaped.length()-1] == '\\')
            escaped.removeLastOccurrence("\\");
        sad::String escpath = path;
        if (escpath.getOccurrence("/")!=-1)
            escpath.replaceAllOccurrences("/","\\");
        if (escpath[0] == '\\')
            escpath.remove(0);
        sad::String result = escaped + "\\" + escpath;
        result = sad::util::canonicalizePath(result);
        return result;
    }

    if (escaped[escaped.length()-1] == '/')
        escaped.removeLastOccurrence("/");
    sad::String escpath = path;
    // Replace old path with new
    escpath.replaceAllOccurrences("\\","/");
    if (escpath.length() == 0)
        return sad::String();
    if (escpath[0] == '/')
        escpath.remove(0);
    sad::String result = escaped + "/" + escpath;
    result = sad::util::canonicalizePath(result);
    return result;
}

sad::String sad::util::folder(const sad::String & path)
{
    if (path.length() == 0)
        return sad::String();
    sad::String escaped = path;
    // Extract path when on windows
    char delimiter[2]="/";
    if (escaped.getOccurrence("\\")!=-1) 
    {
        delimiter[0]='\\';
    }

    if (escaped[escaped.length()-1] == delimiter[0])
        escaped.removeLastOccurrence(delimiter);
    sad::StringList pathparts = escaped.split(delimiter[0]);
    pathparts.removeAt(pathparts.count()-1);
    if (pathparts.count() == 0) 
        return sad::String();
    sad::String result = "";
    if (path[0] == delimiter[0])
        result += delimiter;
    result += pathparts[0];
    for (unsigned int i=1;i<pathparts.count();i++) {
        result << delimiter << pathparts[i];
    }
    return result;
}

bool sad::util::fileExists(const char* path)
{
#ifdef _WIN32
    struct _stat buffer;   
    if(_stat (path, &buffer) == 0)
    {
        return (buffer.st_mode & S_IFMT) == S_IFREG;  
    }
    return false;
#else
    struct stat buffer;   
    if(stat (path, &buffer) == 0)
    {
        return (buffer.st_mode & S_IFMT) == S_IFREG;  
    }
    return false;
#endif
}
