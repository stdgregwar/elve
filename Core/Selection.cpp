#include "Selection.h"

#include <QJsonArray>
#include <QDebug>

namespace Elve {

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

QJsonArray Selection::json() const {
    QJsonArray arr;
    for(const auto& i : *this) {
        arr.append((int)i);
    }
    return arr;
}

Selection Selection::fromJson(const QJsonArray& arr) {
    NodeIDSet set; set.reserve(arr.size());
    for(const QJsonValue& v : arr) {
        set.insert(v.toInt());
    }
    return Selection(set);
}

QDebug operator<<(QDebug stream, const Selection& s) {
   for(const NodeID& i : s) {
       stream << QVariant(i);
   }
   return stream;
}

}
