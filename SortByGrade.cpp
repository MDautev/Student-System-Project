#include "SortByGrade.h"

bool SortByGrade::compare(const Student *a, const Student *b) const
{
    return a->calculateAverage() > b->calculateAverage(); // по успех ↓
}
