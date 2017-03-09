#include <QtGlobal>

#if (QT_VERSION >= QT_VERSION_CHECK(5, 8, 0))
    #define RESULT 1
#else
    #define RESULT 0
#endif

int main(int argc, char** argb)
{
    return RESULT;
}