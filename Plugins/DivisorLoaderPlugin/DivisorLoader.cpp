#include "DivisorLoader.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonObject>
#include <QFileInfo>
#include <QDebug>

#include <algorithm>
#include <functional>
#include <cctype>
#include <locale>
#include <iostream>
#include <fstream>

using namespace std;
using namespace Elve;
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

int find(const NodeIDs& vec, const NodeID& key) {
    for(int i = 0; i < vec.size(); i++) {
        if(vec[i] == key) {
            return i;
        }
    }
    return -1;
}

SharedGraph DivisorLoader::readBlif(const QString &filepath) {

    ifstream file(filepath.toStdString());
    if(!file.is_open()) {
        throw std::runtime_error("couldn't open file \"" + filepath.toStdString() + "\"");
    }
    qDebug() << "Loading BLIF file" << filepath;

    GraphData::Builder b;
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
                    b.setType(inname,INPUT);
                    b.setIoIndex(inname,inputI++);
                }
                lss >> std::ws;
            }
        }
        else if(com == ".outputs") {
            string outname;
            while(getline(lss,outname,' ')) { //TODO
                trim(outname);
                if(outname != "") {
                    b.setType(outname,OUTPUT);
                    b.setIoIndex(outname,outputI++);
                }
                lss >> std::ws;
            }
        }
        else if(com == ".names") {
            string name;
            NodeNames names;
            while(getline(lss,name,' ')) {
                trim(name);
                if(name.length() != 0) {
                    names.push_back(name);
                }
            }
            lastname = name;
            names.pop_back(); //Remove destination

            b.setDependencies(name,names);
        }
        else if(com[0] == '0' or com[0] == '1' or com[0] == '-') { //Truth tables
            if(lastname.size()) {
                string last;
                lss >> last;

                QJsonObject tt;
                if(!b.properties(lastname)["truthtable"].isUndefined()) {
                    tt = b.properties(lastname)["truthtable"].toObject();
                }
                tt[com.c_str()] = last.c_str();
                b.properties(lastname)["truthtable"] = tt;
            }
        }
    }

    /*//Replace outputs with dummies if needed
    for(NodeID id : b.outputs()) {
        Dependencies deps = b.dependencies(id);
        if(deps.size() > 1) { //Replace by a extra top node
            NodeName oName = b.name(id);

            //Create new node
            NodeName nName = oName+"_and";
            b.setDependencies(nName,deps);
            b.setProperties(nName,b.properties(oName));

            //Edit output
            b.setDependencies(oName,{nName});
            QJsonObject tt;
            tt["1"] = "1";
            b.properties(oName)["truthtable"] = tt;
        }
    }*/
    qDebug() <<"file loaded";
    SharedData sdata = b.build(filepath);
    return make_shared<Graph>(sdata);//->clusterize(1);
}


DivisorLoader::DivisorLoader()
{

}

Elve::SharedGraph DivisorLoader::load(const QString &filepath) {
    QFile file(filepath);

    if(!file.open(QFile::ReadOnly)) {
        throw std::runtime_error("Failed to read file" + filepath.toStdString());
    }
    QFileInfo infos(file);
    QString basePath = infos.absolutePath();
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll(), &error);
    if(error.error != QJsonParseError::NoError) {
        throw std::runtime_error("Error parsing json : \n" + error.errorString().toStdString());
    }

    //Colors
    QHash<int,QString> colorMap;
    QJsonArray colors = doc.object().value("colors").toArray();
    for(const QJsonValue& v : colors) {
        QJsonObject col = v.toObject();
        colorMap[col.value("layer").toInt()] = col.value("color").toString();
    }

    //Inputs outputs
    GraphData::Builder b;
    {
        QJsonArray inputs = doc.object().value("inputs").toArray();
        int j = 0;
        for(const QJsonValue& v : inputs) {
            NodeName name = v.toString().toStdString();
            b.setIoIndex(name,j++);
            b.setType(name,NodeType::INPUT);
        }
    }
    {
        QJsonArray outputs = doc.object().value("outputs").toArray();
        int j = 0;
        for(const QJsonValue& v : outputs) {
            NodeName name = v.toString().toStdString();
            b.setIoIndex(name,j++);
            b.setType(name,NodeType::OUTPUT);
        }
    }

    //Sub graphs
    int lastLayer = 0;
    NodeNames lastDivs;
    QJsonArray divisors = doc.object().value("divisors").toArray();
    for(const QJsonValue& v : divisors)
    {
        QJsonObject div = v.toObject();
        NodeName name = div.value("output").toString().toStdString();
        int layer = div.value("layer").toInt();
        if(layer > lastLayer) {
            lastDivs.clear();
            lastLayer = layer;
        }
        lastDivs.push_back(name);
        b.addProperty(name,"color",colorMap.value(layer));
        b.addProperty(name,"level",layer+1);

        QJsonObject toShow;
        toShow.insert("func",div.value("func"));
        toShow.insert("cost",div.value("cost"));
        toShow.insert("layer",div.value("layer"));
        toShow.insert("inputs",div.value("inputs"));
        b.addProperty(name,"toShow",toShow);
        NodeNames deps;
        for(const QJsonValue& v : div.value("inputs").toArray()) {
            deps.push_back(v.toString().toStdString());
        }
        b.setDependencies(name,deps);
    }
    //Rest
    if(doc.object().contains("depntk")) {
        QString name = doc.object().value("depntk").toString();
        SharedGraph depntk = readBlif(basePath+"/"+name);
        NodeIDSet toGroup;
        for(const NodesByID::value_type& p : depntk->nodes()) {
            const Node& n = p.second;
            if(!n.isInput()) {
                b.setNode(n.name(),n);
                if(!n.isOutput()) {
                    toGroup.insert(b.id(n.name()));
                }
            }
        }
        SharedData sdata = b.build(filepath);
        SharedGraph g = make_shared<Graph>(sdata);
        return g->group(toGroup,g->newID(),"depntk");
    } else {
        NodeName name = "depntk";
        b.setDependencies(name,lastDivs);
        b.addProperty(name,"level",lastLayer+2);
        for(const NodeID& id : b.outputs()) {
            b.setDependencies(b.name(id),{name});
        }
        SharedData sdata = b.build(filepath);
        return make_shared<Graph>(sdata);
    }
}
