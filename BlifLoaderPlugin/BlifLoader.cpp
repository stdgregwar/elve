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

SharedGraph BlifLoader::load(const QString &filepath) {

    ifstream file(filepath.toStdString());
    if(!file.is_open()) {
        throw std::runtime_error("couldn't open file \"" + filepath.toStdString() + "\"");
    }
    qDebug() << "Loading BLIF file" << filepath;

    NodeDescriptions desc;
    AdjacencyList adj;
    string line;

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
                    desc.emplace(std::piecewise_construct,
                                 std::forward_as_tuple(inname),
                                 std::forward_as_tuple(inname,Node::INPUT,QJsonObject(),inputI++));
                    //addDescription(inname,Node::INPUT,inputI++);
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
                    desc.emplace(std::piecewise_construct,
                                 std::forward_as_tuple(outname),
                                 std::forward_as_tuple(outname,Node::OUTPUT,QJsonObject(),outputI++));
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
                    desc.emplace(std::piecewise_construct,
                                 std::forward_as_tuple(name),
                                 std::forward_as_tuple(name,Node::NODE,QJsonObject(),0));
                }
            }
            lastname = name;
            for(size_t i = 0; i < names.size()-1;i++) {
                adj.push_back({names[i],name});
            }
        }
        else if(com[0] == '0' or com[0] == '1' or com[0] == '-') { //Truth tables
            if(lastname.size()) {
                string last;
                lss >> last;

                QJsonObject tt;
                if(!desc.at(lastname).properties["truthtable"].isUndefined()) {
                    QJsonObject tt = desc.at(lastname).properties["truthtable"].toObject();
                }
                tt[com.c_str()] = last.c_str();
                desc.at(lastname).properties["truthtable"] = tt;
            }
        }
    }
    qDebug() <<"file loaded";
    return make_shared<Graph>(desc,adj);
}



#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(BlifLoaderPlugin, BlifLoader)
#endif // QT_VERSION < 0x050000
