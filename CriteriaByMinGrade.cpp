#include "CriteriaByMinGrade.h"

CriteriaByMinGrade::CriteriaByMinGrade(double g)
    : minGrade(g) {}

bool CriteriaByMinGrade::meetsCriteria(const Student &s) const
{
    return s.calculateAverage() >= minGrade;
}
