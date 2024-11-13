//
// Created by nicol on 10/28/2024.
//

#ifndef SINGLETON_H
#define SINGLETON_H


template<class T>
class Singleton
{
public:
    static T& GetInstance()
    {
        static T instance{};
        return instance;
    }

    virtual ~Singleton() {}
    Singleton(const Singleton&) = delete;
    Singleton(Singleton&&) = delete;
    Singleton& operator=(const Singleton&) = delete;
    Singleton& operator=(Singleton&&) = delete;
protected:
    Singleton() = default;
};

#endif //SINGLETON_H
