#ifndef SORT_BY_NAME_H
#define SORT_BY_NAME_H

#include "SortStrategy.h"

class SortByName : public SortStrategy
{
public:
    bool compare(const Student *a, const Student *b) const override;
};

#endif
