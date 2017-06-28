#include "NodeDescriptor.h"

namespace Elve {

    NodeDescriptor::NodeDescriptor()
    {

    }

    QString NodeDescriptor::multipleDescription(const Elve::Selection& sel, const SharedEGraph& eg) {
        int counts[NodeType::LAST] = {};

        for(const auto& id : sel) { //Count nodes types
            auto node = eg->graph()->node(id);
            ++counts[node.type()];
        }

        return QString(
                    "<html>"
                        "%1 <b>Nodes</b><br>"
                        "%2 <b>Clusters</b><br>"
                        "%3 <b>Inputs</b><br>"
                        "%4 <b>Outputs</b><br>"
                    "</html>"
                    )
                .arg(sel.size())
                .arg(counts[CLUSTER]+counts[INPUT_CLUSTER]+counts[OUTPUT_CLUSTER])
                .arg(counts[INPUT]+counts[INPUT_CLUSTER])
                .arg(counts[OUTPUT]+counts[OUTPUT_CLUSTER]);
    }

    QString NodeDescriptor::nodeDescription(const Elve::Node& node) {
        QString props;
        if(node.properties().contains("toShow")) {
            auto toShow = node.properties().value("toShow").toObject();
            for(auto v = toShow.begin(); v != toShow.end(); v++) {
                props += v.key() + " : ";
                if(v.value().isArray()) {
                    props += "[";
                    bool first = true;
                    for(const auto& v2 : v.value().toArray()) {
                        props += (first ? "" : ", ") + v2.toString();
                        first = false;
                    }
                    props += "]";
                } else {
                    props += v.value().toVariant().toString();
                }
                props += "\n";
            }
        }
        return [&]()->QString{
            if(node.type() == CLUSTER) {
                return QString(
                            "<html>"
                               "<b>Name</b> : %1<br>"
                                   "<b>ID</b>   : %2<br>"
                                   "<b>nodeCount</b> : %3<br>"
                                   "%4"
                               "</html>"
                               )
                           .arg(node.name().c_str())
                           .arg(node.id())
                           .arg(node.data().dependencies().size())
                           .arg(props);
            } else {
                QJsonObject truthtable = node.properties()["truthtable"].toObject();
                QString table;
                for(QJsonObject::const_iterator it = truthtable.constBegin();
                        it != truthtable.constEnd();
                        it++) {
                    table += it.key() + " ";
                    table += it.value().toString() + "\n";
                }
                return QString(
                            "<html>"
                            "<b>Name</b> : %1<br>"
                                   "<b>ID</b>   : %2<br>"
                                   "<b>level</b>: %3<br>"
                                   "<b>truthtable</b> :\n %4<br>"
                                   "<b>ancestorsCount</b> : %5<br>"
                                   "%6"
                               "</html>"
                               )
                           .arg(node.name().c_str())
                           .arg(node.id())
                           .arg(QString::number(node.level()))
                           .arg(table)
                           .arg(node.data().dependencies().size())
                           .arg(props);
            }
        }();
    }

}
