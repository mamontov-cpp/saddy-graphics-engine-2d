#include "stdafx.h"
#include "log.h"
#include "templates/hhash.hpp"

int _tmain(int argc, TCHAR * argv[])
{
	//hst::log::inst()->owrite("Some text: ").owrite(5).owrite('\n').owrite(167.0f);
	//hst::log::save("log.txt");
	hst::hash<int,int>  test;
	for (int i=0;i<32;i++)
	{
	   printf("Added some test: %d\n",i);
	   test.insert(i,i);
	}
	hst::hash<int,int>::iterator ft=test.end();
	hst::hash<int,int>::iterator it=test.end();
	--it;
	for (it=test.begin();it!=test.end();++it)
	{
      printf("Tested %d %d\n",it.key(),it.value());
	}
	//printf("Tested %d %d\n",it.key(),it.value());
	return 0;
}

