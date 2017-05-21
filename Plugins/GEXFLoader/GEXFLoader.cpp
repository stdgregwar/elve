#include "GEXFLoader.h"
#include <fstream>
#include <sstream>
#include <cstdio>
#include <QDebug>
#include <QFile>
#include <QXmlStreamReader>
#include <QJsonObject>

#include <algorithm>
#include <functional>
#include <cctype>
#include <locale>

using namespace Elve;
using namespace std;

SharedGraph GEXFLoader::load(const QString &filepath) {
    QFile file(filepath);
    if(!file.open(QFile::ReadOnly)) {
        throw std::runtime_error("Couldn't open file " + filepath.toStdString() + " for reading.");
    }
    QXmlStreamReader xml(&file);
    GraphData::Builder b;
    int iindex = 0;
    int oindex = 0;
    while(!xml.atEnd()) {
        QXmlStreamReader::TokenType t = xml.readNext();
        if(t == QXmlStreamReader::StartElement) {
            QXmlStreamAttributes attrs = xml.attributes();
            if(xml.name() == "node") {
                QString lab = attrs.value("label").toString();
                NodeName name = lab.toStdString();
                int lev = attrs.value("level").toInt();
                int id = attrs.value("id").toInt();
                b.setId(name,id);
                if(lev == 0 || (lab.startsWith("IN") && !lab.endsWith("FANOUT"))) {//Input
                    b.setType(name,NodeType::INPUT);
                    b.setIoIndex(name,iindex++);
                }
                if(lab.startsWith("OUT")) { //Output
                    b.setType(name,NodeType::OUTPUT);
                    b.setIoIndex(name,oindex++);
                }
                b.addProperty(name,"level",lev);
            } else if(xml.name() == "edge") {
                int source = attrs.value("source").toInt();
                int target = attrs.value("target").toInt();
                b.addDependency(source,target);
            }
        }
    }
    SharedData sdata = b.build(filepath);
    return make_shared<Graph>(sdata);
}


#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(BlifLoaderPlugin, BlifLoader)
#endif // QT_VERSION < 0x050000
