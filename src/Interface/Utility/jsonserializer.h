#ifndef JSONSERIALIZER_H
#define JSONSERIALIZER_H

#include "serializer.h"

class JSONSerializer : public Serializer
{
public:
    JSONSerializer() = default;

    void toJson(const Serializable &serializable, const QString &filepath)override ;
    void fromJson(Serializable &serializable, const QString &filepath) override;
};

#endif // JSONSERIALIZER_H
