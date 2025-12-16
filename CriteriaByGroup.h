#ifndef CRITERIA_BY_GROUP_H
#define CRITERIA_BY_GROUP_H

#include "Criteria.h"

class CriteriaByGroup : public Criteria
{
private:
    int groupNumber;

public:
    CriteriaByGroup(int g);
    bool meetsCriteria(const Student &s) const override;
};

#endif
