/**
 * @file sington.h
 * @brief 单例基类
 */

#pragma once


template<class T>
class Sington
{
    public:
        static inline T& instance()
        {
            static  T* _instance = NULL;
            if (!_instance) {
                _instance = new T();
            }
            return *_instance;
        }
    protected:
        Sington() {}
        ~Sington() {}
    private:
        Sington(const Sington &other) {}
        Sington(const Sington &&other) {}
};


