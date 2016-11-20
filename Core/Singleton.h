#ifndef SINGLETON_H
#define SINGLETON_H

template<typename T>

class Singleton
{
public:
    static T& get() {
        if(!sInstance) {
            sInstance = new T();
        }
        return *sInstance;
    }
private:
    static T* sInstance;
};

template<typename T>
T* Singleton<T>::sInstance = nullptr;


#endif // SINGLETON_H
