#ifndef OR_CRITERIA_H
#define OR_CRITERIA_H

#include "Criteria.h"

class OrCriteria : public Criteria
{
private:
    const Criteria &c1;
    const Criteria &c2;

public:
    OrCriteria(const Criteria &a, const Criteria &b);
    bool meetsCriteria(const Student &s) const override;
};

#endif
