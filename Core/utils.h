#ifndef UTILS_H
#define UTILS_H

template<typename T,
         template<typename> class IN,
         template<typename> class OUT>
void add_if(const IN<T>& from, OUT<T>& to,const std::function<bool(const T&)>& pred) {
    for(const T& el : from) {
        if(pred(el)) {
            to.insert(el);
        }
    }
}

#endif // UTILS_H
