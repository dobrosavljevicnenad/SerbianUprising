#ifndef SERIALIZER_H
#define SERIALIZER_H


#include "serializable.h"

class  Serializer
{
public:
    virtual ~Serializer() = default;

    virtual void toJson(const Serializable& serializable, const QString& filepath) = 0;


    virtual void fromJson(Serializable& serializable, const QString& filepath) = 0;
};

#endif // SERIALIZER_H
