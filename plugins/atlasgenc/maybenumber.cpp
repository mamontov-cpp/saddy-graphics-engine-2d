#include "maybenumber.h"

#include <math.h>

bool operator>(const MaybeNumber& n1, const MaybeNumber& n2)
{
    if (n1.exists() == false)
    {
        return true;
    }
    else
    {
        if (n2.exists()) {
            return false;
        }
    }
    return n1.value() > n2.value();
}

bool operator==(const MaybeNumber& n1, const MaybeNumber& n2)
{
    if (n1.exists() == n2.exists()) {
        if (n1.exists()) {
            return fabs(n1.value() - n2.value()) < 0.1;
        } else {
            return true;
        }
    }
    return false;
}
