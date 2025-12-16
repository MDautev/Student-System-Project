#include "CriteriaByGroup.h"

CriteriaByGroup::CriteriaByGroup(int g)
    : groupNumber(g) {}

bool CriteriaByGroup::meetsCriteria(const Student &s) const
{
    return s.getGroupNumber() == groupNumber;
}
