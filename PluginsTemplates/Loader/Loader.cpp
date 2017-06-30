#include "Loader.h"
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

LoaderName::LoaderName(QObject *parent) {

}

SharedGraph LoaderName::load(const QString &filepath) {
    GraphData::Builder b;

    //YOUR CODE HERE
    //See GEFXLoader for a concrete example, avoid peeking in Blifloader as it is legacy code of pretty much bad quality

    SharedData sdata = b.build(filepath); //Build the graph data
    return make_shared<Graph>(sdata);
}
