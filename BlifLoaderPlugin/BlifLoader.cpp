#include "BlifLoader.h"
#include <fstream>
#include <sstream>
#include <cstdio>
#include <QDebug>
#include <QFile>
#include <QJsonObject>

#include <algorithm>
#include <functional>
#include <cctype>
#include <locale>

using namespace std;
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

static inline bool getTrueLine(ifstream& file,string& toFill) {
    toFill = "";
    string line;
    if(file.eof())
    {
        return false;
    }
    while(getline(file,line)) {
        trim(line);
        toFill += line;
        if(line.back() != '\\') {
            return true;
        } else {
            toFill.pop_back(); //Remove the trailing back-slash
        }
    }
    return true;
}



BlifLoader::BlifLoader(QObject *parent) :
    QObject(parent)
{
}

int find(const NodeIDs& vec, const NodeID& key) {
    for(int i = 0; i < vec.size(); i++) {
        if(vec[i] == key) {
            return i;
        }
    }
    return -1;
}

SharedGraph BlifLoader::load(const QString &filepath) {

    ifstream file(filepath.toStdString());
    if(!file.is_open()) {
        throw std::runtime_error("couldn't open file \"" + filepath.toStdString() + "\"");
    }
    qDebug() << "Loading BLIF file" << filepath;

    NodeDatas data;
    string line;

    NodeIDs outputs;

    Index inputI = 0;
    Index outputI = 0;
    string lastname;

    while(getTrueLine(file,line)) {
        stringstream lss(line);
        string com;
        lss >> com;
        if(com[0] == '#') {
            qDebug() << "line" << line.c_str() << "is a comment";
            continue;
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
                    //inputs.push_back(inname);
                    //addDescription(inname,Node::INPUT,inputI++);
                    data.emplace(std::piecewise_construct,
                            std::forward_as_tuple(inname),
                            std::forward_as_tuple(inname,NodeIDs(),INPUT,QJsonObject(),inputI++));
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
                    outputs.push_back(outname);
                }
                lss >> std::ws;
            }
        }
        else if(com == ".names") {
            string name;
            NodeIDs names;
            while(getline(lss,name,' ')) {
                trim(name);
                if(name.length() != 0) {
                    names.push_back(name);
                }
            }
            lastname = name;
            names.pop_back(); //Remove destination

            int outindex = find(outputs,name);

            NodeType type = (outindex != -1) ? OUTPUT : NODE;

            data.emplace(std::piecewise_construct,
                         std::forward_as_tuple(name),
                         std::forward_as_tuple(name,names,type,QJsonObject(),outindex));
        }
        else if(com[0] == '0' or com[0] == '1' or com[0] == '-') { //Truth tables
            if(lastname.size()) {
                string last;
                lss >> last;

                QJsonObject tt;
                if(!data.at(lastname).properties()["truthtable"].isUndefined()) {
                    QJsonObject tt = data.at(lastname).properties()["truthtable"].toObject();
                }
                tt[com.c_str()] = last.c_str();
                data.at(lastname).properties()["truthtable"] = tt;
            }
        }
    }
    qDebug() <<"file loaded";
    SharedData sdata = make_shared<GraphData>(data,filepath);
    return make_shared<Graph>(sdata);
}



#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(BlifLoaderPlugin, BlifLoader)
#endif // QT_VERSION < 0x050000
