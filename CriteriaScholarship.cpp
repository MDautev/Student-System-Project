#include "CriteriaScholarship.h"
#include "PStudent.h"

bool CriteriaScholarship::meetsCriteria(const Student &s) const
{
    const PStudent *ps = dynamic_cast<const PStudent *>(&s);
    return ps && ps->hasScholarship();
}
