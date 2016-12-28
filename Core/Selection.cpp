#include "Selection.h"

Selection::Selection(const NodeIDSet &set) : NodeIDSet(set)
{

}

Selection::Selection() {

}

void Selection::add(const NodeID& id)
{
    insert(id);
}

void Selection::add(const Selection& other)
{
    for(const auto& i : other) {
        add(i);
    }
}

void Selection::sub(const Selection& other)
{
    for(const auto& i : other) {
        sub(i);
    }
}

void Selection::sub(const NodeID& id)
{
    erase(id);
}
