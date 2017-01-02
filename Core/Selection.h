#ifndef SELECTION_H
#define SELECTION_H

#include <unordered_set>
#include <Node.h>

class Selection : public NodeIDSet
{
public:
    Selection();
    Selection(const NodeIDSet& set);
    void add(const NodeID& id);
    void add(const Selection& other);
    void sub(const Selection& other);
    void sub(const NodeID& id);
    QJsonArray json() const;
    static Selection fromJson(const QJsonArray& arr);
};

QDebug operator<<(QDebug stream, const Selection& s);

#endif // SELECTION_H
