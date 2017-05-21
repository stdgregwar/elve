#ifndef GRAPHDATA_H
#define GRAPHDATA_H

#include "NodeData.h"

namespace Elve {

class GraphData;
typedef std::shared_ptr<GraphData> SharedData;

class Graph;
class GraphData
{
    friend class Graph;
public:
    /**
     * @brief The graph data builder is used to efficiently generate the graph data structure without
     * the hassle of keeping intermediary values.
     *
     * The main advantage of using this class to construct Graph data is the possibility to use string to
     * refer to the nodes and edit their properties.
     *
     * @code
     * Graph::Builder b;
     * b.setType("in",INPUT);
     * b.setType("out",OUTPUT);
     * b.setDependencies("out",{"in"});
     * SharedData data = b.build();
     * @endcode
     *
     */
    class Builder{
    public:
        /**
         * @brief Builder
         */
        Builder();

        /**
         * @brief set the dependencies of a node
         * @param name name of the node
         * @param dependencies direct dependencies of the node as an array of string
         */
        void setDependencies(const NodeName& name, const NodeNames& dependencies);

        /**
         * @brief set the dependencies of a node
         * @param name name of the node
         * @param dependencies array of Dependency to have differentiated inputs
         */
        void setDependencies(const NodeName &name, const Dependencies &dependencies);

        /**
         * @brief set the properties of a node
         * @param name name of the node
         * @param props
         */
        void setId(const NodeName& name, const NodeID& id);
        void addDependency(const NodeID& from, const NodeID& to);
        void setProperties(const NodeName& name, const NodeProperties& props);

        /**
         * @brief add a property to a specific node, overriding current property
         * @param name name of the node
         * @param pname name of the property
         * @param val property value
         */
        void addProperty(const NodeName& name, const QString& pname, const QJsonValue& val);

        /**
         * @brief set inputs names for a particular node
         * @param name name of the node
         * @param names names of the inputs in order
         */
        void setNodeInputNames(const NodeName& name, const Names& names);

        /**
         * @brief set ouputs names for a particular node
         * @param name name of the node
         * @param names names of the outputs in order
         */
        void setNodeOutputNames(const NodeName& name, const Names& names);

        /**
         * @brief set all node attribute given the node of a previous graph
         * @param name new name of the node
         * @param node previous node
         */
        void setNode(const NodeName& name, const Node& node);

        /**
         * @brief get json properties of a given node
         * @param name name of the node
         * @return reference to properties
         */
        QJsonObject& properties(const NodeName& name);

        /**
         * @brief set the type of a node
         * @param name name of the node
         * @param type node type (cf NodeType)
         */
        void setType(const NodeName& name, const NodeType& type);

        /**
         * @brief set input/output index of the node
         * @param name name of the node
         * @param index
         *
         * This index is used by most of the layouting plugins
         * to know how the inputs are ordered
         */
        void setIoIndex(const NodeName& name, const Index& index);

        /**
         * @brief create a shared instance of SharedDta from the incremental builder
         * @param filename original filename of the graph, leave just a name if no filename
         * @return a SharedData instance
         *
         * THe newly created SharedData is then ready to be fed in the Graph
         * primary constructor. using std::make_shared() for example.
         */
        const SharedData build(const QString &filename);

        /**
         * @brief get the id of a specific node
         * @param name name of the node
         * @return the unique id associated to the node
         *
         */
        NodeID id(const NodeName& name);

        /**
         * @brief type
         * @param id
         * @return the type of the node
         */
        NodeType type(const NodeID& id);

        /**
         * @brief get node properties given the node id
         * @param id id of the node
         * @return
         */
        NodeProperties props(const NodeID& id);
        Index index(const NodeID& id);
        Dependencies dependencies(const NodeID& id);
        Names nodeInputNames(const NodeID& id);
        Names nodeOutputNames(const NodeID& id);
        int nodeInputCount(const NodeID& id);
        int nodeOutputCount(const NodeID& id);

        const NodeIDs& outputs() const;
        const NodeIDs& inputs() const;
        NodeName name(const NodeID& id) const;
    private:

        NodeID mID;
        std::unordered_map<NodeID,Dependencies> mDependencies;
        std::unordered_map<NodeID,NodeProperties> mProperties;
        std::unordered_map<NodeID,Names> mNodeInputsNames;
        std::unordered_map<NodeID,Names> mNodeOutputsNames;
        std::unordered_map<NodeID,int> mNodeInputCount;
        std::unordered_map<NodeID,int> mNodeOutputCount;
        std::unordered_map<NodeID,Index> mIndice;
        std::unordered_map<NodeID,NodeType> mTypes;
        std::unordered_map<NodeName,NodeID> mIDs;
        std::unordered_map<NodeID,NodeName> mNames;
        NodeIDs mOutputs;
        NodeIDs mInputs;
    };

    GraphData(const NodeDatas& nodesData = {}, QString filename = "graph");
    GraphData(const QJsonObject& obj);
    const NodeDatas& nodeDatas() const;
    const QString& filename() const;
    QJsonObject json() const;
private:
    QString mFilename;
    NodeDatas mDatas;
};

}

#endif // GRAPHDATA_H
