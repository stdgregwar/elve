#pragma once

#include <functional>
#include <algorithm>
#include <vector>

namespace Elve {

using namespace std;

template<typename T,
         template<typename> class IN,
         template<typename> class OUT,
         typename Func>
void add_if(const IN<T>& from, OUT<T>& to, Func pred) {
    for(const T& el : from) {
        if(pred(el)) {
            to.insert(el);
        }
    }
}

template<typename T>
int lazy_add(vector<T>& vec, const T& val) {
    auto it = find(vec.begin() ,vec.end(),val);
    if(it!=vec.end()) {
        return distance(vec.begin(),it);
    }
    return distance(vec.begin(),vec.insert(vec.end(),val));
}

}
