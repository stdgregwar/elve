#include "CommandHistory.h"
#include <QJsonObject>
#include <QJsonArray>

namespace Elve {

using namespace std;

CommandHistory::CommandHistory()
{
}

CommandHistory::CommandHistory(const QJsonValue& json)
{
    QJsonArray arr = json.toObject().value("cmds").toArray();
    for(const QJsonValue& v : arr) {
        mCmds.push_back(v.toString().toStdString());
    }
}

CommandHistory::CommandHistory(const CommandHistory& before) : mCmds(before.mCmds)
{
}

void CommandHistory::add(const std::string& cmd)
{
    mCmds.push_back(cmd);
}

void CommandHistory::pop()
{
    mCmds.pop_back();
}

const std::string& CommandHistory::last() const
{
    return mCmds.back();
}

QJsonValue CommandHistory::json() const
{
    QJsonObject obj;
    QJsonArray cmdarray;
    std::string cmds;
    for(const string& cmd : mCmds) {
        cmdarray.append(cmd.c_str());
        cmds += ";" + cmd;
    }
    obj.insert("cmds",cmdarray);
    obj.insert("oneline",cmds.c_str());
    return obj;
}

}
