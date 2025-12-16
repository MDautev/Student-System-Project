#include "SortByName.h"

bool SortByName::compare(const Student *a, const Student *b) const
{
    return a->getName() < b->getName();
}
