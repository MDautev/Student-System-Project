#ifndef CRITERIA_SCHOLARSHIP_H
#define CRITERIA_SCHOLARSHIP_H

#include "Criteria.h"

class CriteriaScholarship : public Criteria
{
public:
    bool meetsCriteria(const Student &s) const override;
};

#endif
