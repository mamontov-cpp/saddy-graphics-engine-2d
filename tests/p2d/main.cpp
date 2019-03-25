#pragma warning(push)
#pragma warning(disable: 4273)
#pragma warning(disable: 4351)
#include <cstdio>
#include "3rdparty/tpunit++/tpunit++.hpp"
#pragma warning(pop)

int main(int argc, char ** argv)
{
   /**
    * Run all of the registered tpunit++ tests. Returns 0 if
    * all tests are successful, otherwise returns the number
    * of failing assertions.
    */
   int result = tpunit::Tests::Run();
   return result;
}
