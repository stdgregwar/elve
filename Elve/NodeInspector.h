#ifndef NODEINSPECTOR_H
#define NODEINSPECTOR_H

#include <QTextEdit>
#include <ExtendedGraph.h>

class NodeInspector : public QTextEdit
{
    Q_OBJECT
public:
    NodeInspector(QWidget* parent = nullptr);
public slots:
    void graphSelectionChanged(const Elve::SharedEGraph& eg);
};

#endif // NODEINSPECTOR_H
