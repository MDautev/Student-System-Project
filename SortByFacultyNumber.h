#ifndef SORT_BY_FACULTY_NUMBER_H
#define SORT_BY_FACULTY_NUMBER_H

#include "SortStrategy.h"

class SortByFacultyNumber : public SortStrategy
{
public:
    bool compare(const Student *a, const Student *b) const override;
};

#endif
