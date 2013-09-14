/*! hBuild - small preprocessor for generating makefiles for GCC and other. 
	You put a file in it, and it generates a makefile, using rules, described in template file.
*/
#define _CRT_SECURE_NO_WARNINGS 1

#include <stdio.h>

#ifdef _MSC_VER
	#include <tchar.h>
#endif

#ifdef _MSC_VER
#include <io.h>
#include <direct.h>
#else
#include <unistd.h>
#endif
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>
#include <map>
#include <memory>
#ifdef LINUX
	#include <sys/types.h>
	#include <dirent.h>
#endif

using namespace std;

/*! Removes a value from LIST at POSITION
*/
#define MAKE_REMOVE_AT(LIST,POSITION) LIST.erase(LIST.begin()+POSITION)

/*! svector is a synonim for vector of string
*/
typedef std::vector<std::string> svector;

/*! Merges pathes. If a is empty, returns b, otherwise - a"/"b
	\param[in] a first part
	\param[in] b second part
	\return merged path
*/
string merge_path(const string & a,const string & b)
{
	if (a.length()==0) return b;
	return a+string("/")+b;
}

/*! Recursively scans a path
	\param[out] vec   *.c and *.cpp pathes
	\param[in]  path  beginning path
*/
void scan_recursive(svector & vec,const string & path)
{
#ifndef LINUX
 _finddata_t dataf;
 intptr_t fptr=_findfirst(merge_path(path,"*.*").c_str(),&dataf);
 bool retcode=fptr!=-1;
 while (retcode)
 {
		if ( (! (dataf.attrib & _A_SUBDIR)) &&
			(
			 (!strcmp(dataf.name+strlen(dataf.name)-4,".cpp"))  ||
			 (!strcmp(dataf.name+strlen(dataf.name)-2,".c")) ||
             (!strcmp(dataf.name+strlen(dataf.name)-3,".cc")) 
			)
		   )
			vec.push_back(merge_path(path,dataf.name));
		if ( (dataf.attrib & _A_SUBDIR) 
			 && (strcmp(dataf.name,"."))
			 && (strcmp(dataf.name,".."))
		   )
		   scan_recursive(vec,merge_path(path,dataf.name));
		retcode=_findnext(fptr,&dataf)!=-1;
  }
  if (fptr!=-1)
	  _findclose(fptr);
#else
  DIR * dp=NULL;
  struct dirent  * dirp=NULL;
  dp=opendir(merge_path(path,".").c_str());
  //printf("Scanning dir %s\n",merge_path(path,".").c_str());
  while ( (dirp=readdir(dp)) !=NULL)
  {
      //printf("Reading dir entry %p with pointer to string %p\n",dirp,dirp->d_name);
      if  ( (!(dirp->d_type  & DT_DIR)) &&
	    (
	      (!strcmp(dirp->d_name+strlen(dirp->d_name)-4,".cpp"))  ||
          (!strcmp(dirp->d_name+strlen(dirp->d_name)-3,".cc"))       ||  
	      (!strcmp(dirp->d_name+strlen(dirp->d_name)-2,".c"))
	     ) 
           )
           vec.push_back(merge_path(path,dirp->d_name));
      if ((dirp->d_type & DT_DIR)    
	   && (strcmp(dirp->d_name,"."))
	   && (strcmp(dirp->d_name,".."))    
          )
         scan_recursive(vec,merge_path(path,dirp->d_name));
  }	  
  //printf("Scanned dir %s\n",merge_path(path,".").c_str()); 
  if  (dp!=NULL)
	closedir(dp);
#endif
}

/*! Scans a folder for *.c and *.cpp pathes
	\param[out] vec  *.c and *.cpp pathes
	\param[in]  path  scanned folder
*/
void scan_folder(svector & vec,const string & path)
{
 char cwd[500];
#ifdef LINUX
 getcwd(cwd,500);
#else
 _getcwd(cwd,500);
#endif
 int chdir_res=0;
 if (path!=".")
 {
#ifdef LINUX
	chdir_res=chdir(path.c_str());
#else
#ifdef _MSC_VER
	chdir_res=_chdir(path.c_str());
#else
    chdir_res=chdir(path.c_str());
#endif
#endif
 }
 scan_recursive(vec,"");
 if (path!=".")
#ifdef LINUX
	chdir(cwd);
#else
#ifdef _MSC_VER
	_chdir(cwd);
#else
    chdir(cwd);
#endif
#endif
}

/*! Retrieves an escaped source list, from unescaped.
   "\" replaces on "_" , "." on "_" and extensions a stripped
	\param[out] safe safe list
	\param[in]  unsafe unsafe source list
*/
void escape_list(svector & safe, const svector & unsafe)
{
	char  tmp[1000]={0};
	safe.clear();
	for (unsigned int i=0;i<unsafe.size();i++)
	{
      strcpy(tmp,unsafe[i].c_str());
	  char * p=tmp;
	  while(*p)
	  {
		  if (*p=='/' || *p=='\\' || *p=='.') *p='_';
		  ++p;
	  }
	  char * p2=strstr(tmp,"_c");
	  p=p2;
	  while(p2)
	  {
		p=p2;
		p2=strstr(p+1,"_c");
	  }
	  *p=0;
	  safe.push_back(tmp);
	}
}

/*! Excludes a file from a sources list
	\param[in,out] safe   safe list
	\param[in,out] unsafe unsafe list
	\param[in]     str    string
*/
void exclude_file( svector & safe,
				   svector  & unsafe,
				   const string  & str
				  )
{
	std::vector<int> positions;
	for (unsigned int i=0;i<unsafe.size();i++)
	{
		if (unsafe[i]==str)
		{ positions.push_back(i); }
	}

	for (unsigned int i=0;i<positions.size();i++)
	{
		MAKE_REMOVE_AT(safe,positions[i]-i);
		MAKE_REMOVE_AT(unsafe,positions[i]-i);
	}
}
class state;
/*! Class, that wraps all command name and arguments.
*/
class command_context
{
 private:
	     svector  m_commands;  //!< String representation of all commands
 public:
	    command_context() {}  // Empty
		/*! Creates a new token
		*/
		void push_token() 
		{
			if (m_commands.size()!=0)
				if (m_commands[m_commands.size()-1].length()==0)
					return;
			m_commands.push_back(string()); 
		}
		/*!  Creates a new character in command
			 \param[in] a symbol
		*/
		void push_character(char a) { m_commands[m_commands.size()-1].push_back(a); }
		/*! Creates a new token ,beginning with a
			\param[in] a symbol
		*/
		void push_token(char a)     { push_token(); push_character(a); }
		/*! Strips a beginning and ending lines
			\param[in] begin beginning symbol amount
			\param[in] end   end symbol amount
		*/
		void strip(int begin=0, int end=0)
		{
			string & curcommand=m_commands[m_commands.size()-1];
			for (int i=0;i<begin && curcommand.size()!=0;i++)
				curcommand.erase(curcommand.begin());
			for (int i=0;i<end && curcommand.size()!=0;i++)
				curcommand.erase(curcommand.begin()+curcommand.size()-1);
		}
		/*! Return arguments  of command
		*/
		const svector& args() { return m_commands; }
		/*! Evaluates a context, calling a values.
			\param[in] s state of parser
		*/
		void evaluate(state * s);

		~command_context() {} // Destroys
};

/*! Defines a  common stream state. Also contains a filenames
*/
class stream_state
{
 private:
	      string m_out;
		  svector m_safe;
		  svector m_unsafe;
 public:
	     inline svector & unsafe() { return m_unsafe;}
	     inline svector & safe()  { return m_safe;} 
	     inline string & stream() { return m_out;}
};
/*! Class of lexer state, that counts everything
*/
class lexer_state
{
 private:
		 int m_state;     //!< Lexer state;
		 int m_braces;    //!< Braces
		 int m_stripleft; //!< Strip left
		 int m_stripright; //!< Strip right
		 command_context * m_ctxt; //!< Context
		 unsigned int *   m_i;              
 public:
	     
	    //Creates new state
	    lexer_state() { m_state=0; m_braces=0; m_stripleft=0; m_stripright=0; m_ctxt=0;}
		//Destructor
		~lexer_state() {delete m_ctxt;}
		/*! Sets new state
		*/
		void setState(int s) { m_state=s; }
		
		inline void setCounter(unsigned int * i) {m_i=i;}
		inline void decCounter()  { --(*m_i); }

		inline void resetContext() { delete m_ctxt; m_ctxt=new command_context(); }
		inline command_context * context() { return m_ctxt;}

		inline void nullifyStrips() { m_stripleft=0; m_stripright=0; }

		inline void incBrace()  { ++m_braces;  }
		inline void decBrace()  { --m_braces;  }
		inline void zeroBrace() { m_braces=0;  }

		inline void incLeft() { ++m_stripleft; }
		inline void decLeft() { --m_stripleft; }

		inline void incRight() { ++m_stripright; }
		inline void decRight() { --m_stripright; }

		inline int state() const      { return m_state; }      //All state
		inline int braces() const     { return m_braces; }     //Amount of brackets
		inline int stripLeft() const  { return m_stripleft; }  //Left
		inline int stripRight() const { return m_stripright; } //Right
};

class basiccommand
{
 private:
	     void (*m_cmd)(state & s); //!< Command
 public:
		basiccommand() {}

	    basiccommand( void (*cmd)(state & s) ) { m_cmd=cmd; }
	    /*! Evaluates command
		*/
		virtual void evaluate(state & s) { m_cmd(s) ;}
		virtual ~basiccommand() {}
};

class command_list: public std::map<string, basiccommand *>
{

 public:
	    void insert(const string & v, basiccommand * p)
		{
			if (this->find(v)!=this->end())
			{
				basiccommand * r=(*this)[v];
				delete r;
				this->erase(v);
			}
			this->std::map<string, basiccommand *>::insert(make_pair<string,basiccommand*>(v,p));
		}
	    ~command_list()
		{
			for (command_list::iterator it=this->begin(); it!=this->end();++it)
			{
				delete it->second;
			}
		}
};
class state
{
 private:
	     command_list m_lst;
		 stream_state m_ss;
		 lexer_state  m_ls;
		 state * m_parent;
         std::string m_platform;
 public:
	    state(state * prt) { m_parent=prt; }
		command_list  & list() { return m_lst; }
		stream_state  & streamState() { return m_ss; }
		lexer_state   & lexerState()  { return m_ls; }
		std::string   & platform() { return m_platform; }
        state * parent() {return m_parent;}
		~state() {}
};


//These are lexer inner functions, they can't be explained
typedef bool (*predicate)(char);
typedef void (*handler)(state & v,char c);

std::vector<predicate> & operator<<(std::vector<predicate> & v, predicate x)
{
	v.push_back(x);
	return v;
}
std::vector<handler> & operator<<(std::vector<handler> & v, handler x)
{
	v.push_back(x);
	return v;
}

bool is_zero(char c)  { return c==0; }
bool is_slash(char c) { return c=='\\';  }
bool is_lbrace(char c) { return c=='{'; }
bool is_rbrace(char c) { return c=='}'; }
bool is_nrs(char c)      { return c==' ' || c=='\r' || c=='\n'; }
bool is_nrsrb(char c)      { return c==' ' || c=='\r' || c=='\n' || c=='}'; }
bool is_less(char c)     { return c=='<'; }
bool is_greater(char c)  { return c=='>'; }
bool is_anything(char c) { return true;   }

void finish(state & v, char c) { v.lexerState().setState(8);  }
template<int _newState>
void proceed_to(state & v, char c) { v.lexerState().setState(_newState); }
void go_to_2(state & v, char c)
{
	v.lexerState().setState(2);
	v.lexerState().resetContext();
	v.lexerState().zeroBrace();
	v.lexerState().incBrace();
	v.lexerState().context()->push_token();
}

template<int _newState>
void shield( state & v, char c) 
{
	if (c!='\\' && c!='{' && c!='}')
		v.streamState().stream()+='\\';
	 v.streamState().stream()+=c;
	 v.lexerState().setState(_newState);
}
template<int _newState>
void shield_ex( state & v, char c) 
{
	command_context * ctxt=v.lexerState().context();
	if (c!='\\' && c!='{' && c!='}' && c!='<' && c!='>' && c!='n' && c!='r' && c!='s' && c!='t')
		ctxt->push_character('\\');
	if (c!='n' && c!='r' && c!='s' && c!= 't')
		ctxt->push_character(c);
	if (c=='t') ctxt->push_character('\t');	
    if (c=='n') ctxt->push_character('\n');
	if (c=='r') ctxt->push_character('\r');
	if (c=='s') ctxt->push_character(' ');
	v.lexerState().setState(_newState);
}
void push_symbol(state & v, char c)
{
	v.streamState().stream()+=c;
}

void push_symcmd(state & v, char c)
{
	v.lexerState().context()->push_character(c);
}
void push_symcmd2(state & v, char c)
{
	if (v.lexerState().stripLeft()==0)
	    v.lexerState().context()->push_character(c);
	else
		v.lexerState().decLeft();
}
void inc_brace(state & v, char c)
{
	v.lexerState().incBrace();
	push_symcmd(v,c);
}
void dec_brace(state & v, char c)
{
	v.lexerState().decBrace();
	if (v.lexerState().braces()!=0)
		push_symcmd(v,c);
	else
	{
		v.lexerState().context()->evaluate(&v);
		proceed_to<0>(v,c);
	}
}
void push_and_2(state & v, char c)
{
  v.lexerState().context()->push_token(c);
  proceed_to<2>(v,c);
}
void dec_brace_start_token(state & v, char c)
{
	v.lexerState().decBrace();
	if (v.lexerState().braces()!=0)
	{
		v.lexerState().context()->push_token(c);
	}
	else
	{
		v.lexerState().context()->evaluate(&v);
		proceed_to<0>(v,c);
	}
}
void init_multi_line(state & v ,char c)
{
	v.lexerState().nullifyStrips();
	proceed_to<5>(v,c);
}
void inc_left(state & v ,char c)
{
	v.lexerState().incLeft();
}
void strip_right(state & v, char c)
{
	v.lexerState().context()->strip(0,1);
}
void strip_and_go_to5(state & v, char c)
{
	strip_right(v,c);
	push_symcmd(v,c);
	proceed_to<5>(v,c);
}
unsigned int * cur_sym;
void apply_and_go_to2(state & v, char c)
{
  v.lexerState().setState(2);
  v.lexerState().decCounter();
}
void push_token(state & v, char c)
{
	v.lexerState().context()->push_token();
}
std::string parse(const std::string & what);
std::string parse_with_custom_state(state & s, const std::string & what);
//Here goes functions for commands
void scan_folder(state & s)
{
 const svector & v=s.lexerState().context()->args();
 for (unsigned int i=1;i<v.size();i++)
	scan_folder(s.streamState().unsafe(),v[i]);
 escape_list(s.streamState().safe(),s.streamState().unsafe());
}
template<svector & (stream_state::*tasker)() ,char sep,bool add_last>
void gen_list(state & s)
{
	const svector & v=(s.streamState().*tasker)();
	for (unsigned int i=0;i<v.size();i++)
	{
		s.streamState().stream()+=v[i];
		if (i!=v.size()-1 || add_last)
			s.streamState().stream()+=sep;
	}
}

class custom_string: public basiccommand
{
 private:
     	 std::string m_str;
 public:
		 custom_string() {}
	     custom_string(const std::string & s) {m_str=s; }
		 
		 void evaluate(state & s)
		 {
			//printf("custom_string::evaluate()\n");
			s.streamState().stream()+=m_str;
		 }
};

void gen_custom_list(state & s)
{
  //printf("gen_custom_list\n");
  const svector & args=s.lexerState().context()->args();
  const svector & safe=s.streamState().safe();
  const svector & unsafe=s.streamState().unsafe();
  if (args.size()==2)
  {
    state * tmp_ptr=new state(&s);
    state tmp=tmp_ptr;
	tmp.streamState().safe()=s.streamState().safe();
	tmp.streamState().unsafe()=s.streamState().unsafe();
    for (unsigned int i=0;i<safe.size();i++)
    {
		tmp.streamState().stream().clear();
		tmp.list().insert("SOURCE",new custom_string(unsafe[i]));
		tmp.list().insert("NAME",new custom_string(safe[i]));
		s.streamState().stream()+=parse_with_custom_state(tmp,args[1]);
    }
	delete tmp_ptr;
  }
  if (args.size()>2)
  {
    state * tmp_ptr=new state(&s);
    state tmp=tmp_ptr;
	tmp.streamState().safe()=s.streamState().safe();
	tmp.streamState().unsafe()=s.streamState().unsafe();
	for (unsigned int i=0;i<safe.size();i++)
    {
		tmp.streamState().stream().clear();
		custom_string * pp1=new custom_string(unsafe[i]);
		tmp.list().insert("SOURCE",pp1);
		custom_string * pp2=new custom_string(safe[i]);
		tmp.list().insert("NAME",pp2);
		s.streamState().stream()+=parse_with_custom_state(tmp,args[1]);
		tmp.streamState().stream().clear();
		if (i!=safe.size()-1)
			s.streamState().stream()+=parse_with_custom_state(tmp,args[2]);
    }
	delete tmp_ptr;
  }
}

void exclude(state & s)
{
	const svector & args=s.lexerState().context()->args();
    std::map<string, std::vector<string> > excluded;
    excluded.insert(std::make_pair<string, std::vector<string> >("ALL", std::vector<string>()));
    std::string platform = "ALL";
    // Populate a map of excluded files for platform
    for (unsigned int i=1;i<args.size();i++)
	{
        if (args[i].size() != 0)
        {
            if (args[i][0] == '{' && args[i][args[i].length() - 1] == '}')
            {
                std::string newplatform = args[i].substr(1, args[i].length() - 2);
                platform = newplatform;
                if (excluded.find(platform) == excluded.end())
                {
                    excluded.insert(std::make_pair<string, std::vector<string> >(platform, std::vector<string>()));
                }
            }
            else
            {
                excluded[platform].push_back(args[i]);
            }
        }
    }
    
    std::vector<std::string> & all = excluded["ALL"];
	for (unsigned int i=1;i<all.size();i++)
		exclude_file(s.streamState().safe(),s.streamState().unsafe(),all[i]);
    // Exclude files specific for platform
    if (excluded.find(s.platform()) != excluded.end())
    {
        std::vector<std::string> platformspecific = excluded[s.platform()];
        for (unsigned int i=1;i<platformspecific.size();i++)
            exclude_file(s.streamState().safe(),s.streamState().unsafe(),platformspecific[i]);
    }
}
void file(state & s)
{
	const svector & args=s.lexerState().context()->args();
	if (args.size()<3) 
	{
		printf("WARNING: not enough parameters in FILE command!\n");
		return;
	}
	FILE * fl=fopen(args[1].c_str(),"wt");
	if (!fl)
	{
		printf("WARNING: can't open file %s!\n",args[1].c_str());
		return;
	}
	state tmp(&s);
	tmp.streamState().safe()=s.streamState().safe();
	tmp.streamState().unsafe()=s.streamState().unsafe();
	std::string tmp2=parse_with_custom_state(tmp,args[2]);
    fputs(tmp2.c_str(),fl);
	fclose(fl);
}
class simplemacrocommand: public basiccommand
{
 private:
	     const char * m_to;
 public:
		simplemacrocommand(const char * to) : basiccommand(), m_to(to)  {}
	    /*! Evaluates command
		*/
		virtual void evaluate(state & s) { s.streamState().stream()+=std::string(m_to); }
		virtual ~simplemacrocommand() {}
};
/*! Main parsing function
	\param[in] what string to parse
	\return preprocessed function
*/
std::string parse(const std::string & what, int argc, char ** argv)
{
	std::string result;
	state * p=new state(NULL);
    state & s=*p;
	s.list().insert("SCAN_FOLDER",new basiccommand(scan_folder));
	s.list().insert("LIST_SOURCE_IN_LINE",new basiccommand(gen_list<&stream_state::unsafe,' ',false>));
	s.list().insert("LIST_NAMES_IN_LINE",new basiccommand(gen_list<&stream_state::safe,' ',false>));
	s.list().insert("LIST_SOURCE",new basiccommand(gen_list<&stream_state::unsafe,'\n',true>));
	s.list().insert("LIST_NAMES",new basiccommand(gen_list<&stream_state::safe,'\n',true>));
	s.list().insert("FOR_EACH",new basiccommand(gen_custom_list));
	s.list().insert("EXCLUDE",new basiccommand(exclude));
	s.list().insert("FILE",new basiccommand(file));
    s.platform() == "";
    for(int i = 4; i + 1 < argc; i += 2 )
    {
        s.list().insert(argv[i], new simplemacrocommand(argv[i+1]));
        // Handle currrent platform with platform command
        if (std::string(argv[i]) == "PLATFORM") 
        {
            s.platform() = argv[i + 1];
        }
    }
	std::string  f=parse_with_custom_state(s,what);
	delete p;
	return f;
}
 std::vector<predicate> predicates[8];
 std::vector<handler> handlers[8];
bool is_p_initted=false;
void init_pred_handlers()
{
 predicates[0]<<is_zero<<is_slash      <<is_lbrace<<is_anything;
 handlers[0]  <<finish <<proceed_to<1> <<go_to_2  <<push_symbol;

 predicates[1]<<is_anything;
 handlers[1]  <<shield<0>;
	
 predicates[2]<<is_slash     <<is_lbrace<<is_rbrace<<is_less        <<is_nrs            <<is_anything;
 handlers[2]  <<proceed_to<3><<inc_brace<<dec_brace<<init_multi_line<<push_token        <<push_symcmd;

 predicates[3]<<is_anything;
 handlers[3]  <<shield_ex<2>;
	
 predicates[5]<<is_slash     <<is_less<<is_greater<<is_anything;
 handlers[5]  <<proceed_to<6><<inc_left<<proceed_to<7><<push_symcmd2;
	
 predicates[6]<<is_anything;
 handlers[6]  <<shield_ex<5>;

 predicates[7]<<is_greater <<is_nrsrb        <<is_anything;
 handlers[7]  <<strip_right<<apply_and_go_to2<<strip_and_go_to5;
}




/*! Preprocesses string with custom initial state
	\param[in] s state
	\param[in] what string
	\return preprocessed string
*/
std::string parse_with_custom_state(state & s, const std::string & what)
{
 if (!is_p_initted)
 {
	 is_p_initted=true;
	 init_pred_handlers();
 }
 unsigned int i=0;
 s.lexerState().setCounter(&i);
 for (i=0;i<what.length();i++)
 {
 	if (s.lexerState().state()==-1)
 	{
		printf("ERROR:Unexpected character at %d: %d(\'%c\')\n",i-1,what[i-1],what[i-1]);
 		break;
	}
	if (s.lexerState().state()==8) 
		break;
	bool handled=false;
	for (unsigned int j=0;(j<predicates[s.lexerState().state()].size()) && !handled;j++)
	{
		if (predicates[s.lexerState().state()][j](what[i]))
		{
			//printf("Called symbol %c handler (%d,%d)\n",what[i],s.lexerState().state(),j);
			handlers[s.lexerState().state()][j](s,what[i]);
			handled=true;
		}
	}
	if (!handled)
		printf("WARNING: ignored %d(\'%c\') at %d\n",what[i],what[i],i);
 }

 return s.streamState().stream();
}
void command_context::evaluate(state * s)
{
 //printf("command_context::evaluate\n");
 if (m_commands.size()!=0)
	if (m_commands[m_commands.size()-1].length()==0)
		m_commands.erase(m_commands.begin()+m_commands.size()-1);
 if (m_commands.size()==0)
 {
	 printf("WARNING: Empty command detected!");
 }

 state * cur_context=s;
 bool handled=false;

 while(cur_context && !handled)
 {
  command_list::iterator it=cur_context->list().find(m_commands[0]);
  if (it==cur_context->list().end())
     cur_context=cur_context->parent();
  else
  {
	 handled=true;
	 it->second->evaluate(*s);
  }
 }
 if (!handled)
	 printf("WARNING: Unknown command: %s\n",m_commands[0].c_str());
}

/*! Main function, that does all preprocessing.
*/
int main(int argc, char* argv[])
{
	if (argc>1)
	{
		FILE * fl=fopen(argv[1],"rt");
		if (!fl) 
		{
			printf("FATAL: Can't open %s\n",argv[1]);
			return 1;
		}
		std::string old;
		char c=0;
		while (!feof(fl)) { c=fgetc(fl); old+=c; }
		fclose(fl);
		std::string new_s=parse(old, argc, argv);
		if (argc<4)
			puts(new_s.c_str());
		else
		{
			fl=fopen(argv[3],"wb");
			if (!fl) 
		    {
			 printf("FATAL: Can't open %s\n",argv[3]);
			 return 1;
		    }
			new_s.erase(new_s.begin()+(new_s.length()-1));
			fputs(new_s.c_str(),fl);
			fclose(fl);
		}
	}
	else printf("hbuild - small makefile preprocessor. Usage: hbuild <file> [-o file]  <define1> <value1> <define2> <value2> ...");
	
	return 0;
}

