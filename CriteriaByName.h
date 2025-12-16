#ifndef CRITERIA_BY_NAME_H
#define CRITERIA_BY_NAME_H

#include "Criteria.h"
#include <string>

class CriteriaByName : public Criteria
{
private:
    std::string name;

public:
    CriteriaByName(const std::string &n);
    bool meetsCriteria(const Student &s) const override;
};

#endif
