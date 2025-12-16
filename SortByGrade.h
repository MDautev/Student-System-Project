#ifndef SORT_BY_GRADE_H
#define SORT_BY_GRADE_H

#include "SortStrategy.h"

class SortByGrade : public SortStrategy
{
public:
    bool compare(const Student *a, const Student *b) const override;
};

#endif
