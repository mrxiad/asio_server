#ifndef VECTOR_H
#define VECTOR_H

#include "const.h"

template<class T>
class Vector{
public:
    void add(const T& Word);
    void remove(const T& word);

private:
    T* data;
};


#endif // VECTOR_H
