#include "SortByFacultyNumber.h"

bool SortByFacultyNumber::compare(const Student *a, const Student *b) const
{
    return a->getFacultyNumber() < b->getFacultyNumber();
}
