#ifndef SINGLETON_H_INCLUDED
#define SINGLETON_H_INCLUDED

namespace MyOGL
{

template <typename Type>
class Singleton {
    private:
        // Classes using the Singleton<T> pattern should declare a getInstance()
        // method and call Singleton::get() from within that.
        friend Type* Type::getInstance();

        static Type* get()
        {
            if (!instance_)
                instance_ = new Type();

            return instance_;
        }

        static Type* instance_;
};

template <typename Type>
Type* Singleton<Type>::instance_ = NULL;

}

#endif // SINGLETON_H_INCLUDED
