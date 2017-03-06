#pragma once

#include <alice/command.hpp>
#include <chrono>

namespace Elve {

using namespace std::chrono;
using namespace alice;

class ChronoCommand : public command
{

public:
    ChronoCommand(const environment::ptr& env) : command(env,"chrono") {
        opts.add_options()("reset,r","reset chrono");
    }

    bool execute() override {
        if(is_set("reset")) {
            startTime = high_resolution_clock::now();
        } else {
            high_resolution_clock::time_point endTime = high_resolution_clock::now();
            milliseconds ms = duration_cast<milliseconds>(endTime - startTime);
            env->out() << "Elapsed time : " << ms.count() << " [ms]\n";
            qDebug() << "Elapsed time :" << ms.count() << "[ms]";
        }
        return true;
    }
private:
    high_resolution_clock::time_point startTime;
};

}
