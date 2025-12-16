#ifndef CRITERIA_H
#define CRITERIA_H

#include "Student.h"

class Criteria
{
public:
    virtual bool meetsCriteria(const Student &s) const = 0;
    virtual ~Criteria() {}
};

#endif
