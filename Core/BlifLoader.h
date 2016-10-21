#ifndef BLIFLOADER_H
#define BLIFLOADER_H

#include "Graph.h"
#include <map>

class BlifLoader
{
public:
    BlifLoader(const std::string& path);
    SharedGraph load();
    Node* gate(const NodeID& id);
private:
    std::string mPath;
    SharedGraph mG;
    std::map<std::string,Index> mInputs;
    std::map<std::string,Index> mOutputs;
};


#endif // BLIFLOADER_H
