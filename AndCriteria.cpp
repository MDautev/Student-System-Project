#include "AndCriteria.h"

AndCriteria::AndCriteria(const Criteria &a, const Criteria &b)
    : c1(a), c2(b) {}

bool AndCriteria::meetsCriteria(const Student &s) const
{
    return c1.meetsCriteria(s) && c2.meetsCriteria(s);
}
