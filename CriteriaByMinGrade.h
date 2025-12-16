#ifndef CRITERIA_BY_MIN_GRADE_H
#define CRITERIA_BY_MIN_GRADE_H

#include "Criteria.h"

class CriteriaByMinGrade : public Criteria
{
private:
    double minGrade;

public:
    CriteriaByMinGrade(double g);
    bool meetsCriteria(const Student &s) const override;
};

#endif
