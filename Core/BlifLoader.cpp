#include "BlifLoader.h"
#include <fstream>
#include <sstream>
#include <cstdio>
#include <QDebug>
#include <QFile>

#include <algorithm>
#include <functional>
#include <cctype>
#include <locale>

// trim from start
static inline std::string &ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(),
            std::not1(std::ptr_fun<int, int>(std::isspace))));
    return s;
}

// trim from end
static inline std::string &rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(),
            std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
    return s;
}

// trim from both ends
static inline std::string &trim(std::string &s) {
    return ltrim(rtrim(s));
}

using namespace std;

BlifLoader::BlifLoader(const std::string& path) : mPath(path) {}


SharedGraph BlifLoader::load()
{
    /*QFile qfile(mPath.c_str());
    if(!qfile.isReadable()) {
        qDebug() << "File is not readable";
    }*/

    ifstream file(mPath);
    if(!file.is_open()) {
        throw std::runtime_error("couldn't open file \"" + mPath + "\"");
    }
    qDebug() << "Loading BLIF file" << mPath.c_str();

    mG = SharedGraph(new Graph());
    string line;

    Index inputI = 0;
    Index outputI = 0;

    while(getline(file,line)) {
        stringstream lss(line);
        string com;
        lss >> com;
        if(com[0] == '#') {
            qDebug() << "line" << line.c_str() << "is a comment";
        }
        if(com == ".model") {
            string name;
            lss >> name;
        }
        else if(com == ".inputs") {
            string inname;
            while(getline(lss,inname,' ')) { //TODO
                trim(inname);
                if(inname != "") {
                    //qDebug() << "in:" << inname.c_str();
                    mInputs.emplace(inname,inputI++);
                }
                lss >> std::ws;
            }
        }
        else if(com == ".outputs") {
            string outname;
            while(getline(lss,outname,' ')) { //TODO
                trim(outname);
                if(outname != "") {
                    //qDebug() << "out:" << outname.c_str();
                    mOutputs.emplace(outname,outputI++);
                }
                lss >> std::ws;
            }
        }
        else if(com == ".names") {
            string name;
            vector<string> names;
            while(getline(lss,name,' ')) {
                trim(name);
                if(name.length() != 0) {
                    names.push_back(name);
                }
            }
            Node* g = gate(name);
            for(size_t i = 0; i < names.size()-1;i++) {
                g->addAncestor(gate(names[i]));
            }
        }
    }
    return mG;
}

Node* BlifLoader::gate(const NodeID& id)
{
    Node::Type t = mInputs.count(id) ? Node::INPUT : mOutputs.count(id) ? Node::OUTPUT : Node::NODE;
    Node* g = mG->addNode(id,t);
    switch(t) {
        case Node::INPUT:
        g->setIOIndex(mInputs[id]);
        break;
    case Node::OUTPUT:
        g->setIOIndex(mOutputs[id]);
        break;
    default:
        break;
    }
    return g;
}
