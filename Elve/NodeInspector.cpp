#include "NodeInspector.h"
#include <NodeDescriptor.h>

using namespace Elve;

NodeInspector::NodeInspector(QWidget *parent) : QTextEdit(parent)
{
    setReadOnly(true); //We dont want someone to edit text in the widget
}

void NodeInspector::graphSelectionChanged(const SharedEGraph &eg) {
    auto selection = eg->currentSelection();
    if(selection.empty()) {
        clear();
        return;
    }
    auto text = [&]()->QString {
            if(selection.size() > 1) {
                return NodeDescriptor::multipleDescription(selection,eg);
            } else {
                auto id = *begin(selection);
                auto node = eg->graph()->node(id);
                return NodeDescriptor::nodeDescription(node);
            }
        }();
    setHtml(text);
}
