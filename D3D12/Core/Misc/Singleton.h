#pragma once

template<class T>
class Singleton
{
private:
    Singleton(Singleton const&)
    {
    }

    Singleton& operator=(Singleton const&)
    {
        return *this;
    }

protected:

    Singleton() {}
    ~Singleton() {}

public:
    static T* Instance()
    {
        static T _instance;
        return &_instance;
    }
};
