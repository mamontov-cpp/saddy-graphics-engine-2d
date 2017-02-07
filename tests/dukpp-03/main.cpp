#pragma warning(push)
#pragma warning(disable: 4273)
#pragma warning(disable: 4351)
#include <stdio.h>
#include "3rdparty/tpunit++/tpunit++.hpp"
#pragma warning(pop)

#define TEST_INTERACTIVE

#ifdef TEST_INTERACTIVE
    #include "dukpp-03/context.h"
#endif

int main(int argc, char ** argv)
{
#ifdef TEST_INTERACTIVE
    int result = 0;
    sad::dukpp03::Context t;
    bool exit = false;
    std::cout << "Interactive test shell. Type \"quit\" to quit\n";
    do 
    {
        std::cout << "test> ";
        std::string test, error;
        std::getline(std::cin, test);
        if (test == "quit")
        {
            exit = true;
        }
        else
        {
            bool res = t.eval(test, false, &error);
            if (!res)
            {
                std::cout << error << "\n";
            }
        }
    } while(!exit);
#else
   /**
    * Run all of the registered tpunit++ tests. Returns 0 if
    * all tests are successful, otherwise returns the number
    * of failing assertions.
    */
   int result = tpunit::Tests::Run();
#endif
   return result;
}