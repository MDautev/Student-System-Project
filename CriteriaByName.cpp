#include "CriteriaByName.h"

CriteriaByName::CriteriaByName(const std::string &n)
    : name(n) {}

bool CriteriaByName::meetsCriteria(const Student &s) const
{
    return s.getName().find(name) != std::string::npos;
}
