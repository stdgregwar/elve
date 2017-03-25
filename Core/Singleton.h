#ifndef SINGLETON_H
#define SINGLETON_H

template<typename T>

class Singleton
{
public:
    Singleton(){
        sInstance = static_cast<T*>(this);
    }

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
