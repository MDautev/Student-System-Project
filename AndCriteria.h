#ifndef AND_CRITERIA_H
#define AND_CRITERIA_H

#include "Criteria.h"

class AndCriteria : public Criteria
{
private:
    const Criteria &c1;
    const Criteria &c2;

public:
    AndCriteria(const Criteria &a, const Criteria &b);
    bool meetsCriteria(const Student &s) const override;
};

#endif
