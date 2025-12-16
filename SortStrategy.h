#ifndef SORT_STRATEGY_H
#define SORT_STRATEGY_H

#include "Student.h"

class SortStrategy
{
public:
    virtual bool compare(const Student *a, const Student *b) const = 0;
    virtual ~SortStrategy() {}
};

#endif
