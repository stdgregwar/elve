#pragma once

#include <string>
#include <vector>
#include <QJsonValue>

namespace Elve {

class CommandHistory
{
public:
    CommandHistory();
    CommandHistory(const QJsonValue& json);
    CommandHistory(const CommandHistory& before);
    void add(const std::string& cmd);
    void pop();
    const std::string& last() const;
    QJsonValue json() const;
private:
    void _addToJson(QJsonArray& jarr) const;
    std::vector<std::string> mCmds;
};

}
