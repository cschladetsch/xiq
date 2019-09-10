// (C) 2009 christian.schladetsch@gmail.com

#include "Precompiled.h"
#include "ObjectBase.h"
#include "Factory.h"

void ObjectBase::Delete()
{
    Factory *factory = GetFactory();
    if (!factory)
        return;
    factory->Delete(this);
}

Factory *ObjectBase::GetFactory() const
{
    return klass ? klass->GetFactory() : 0;
}

bool Exists(ObjectBase const *base)
{
    return base && base->GetFactory() && base->GetFactory()->Exists(base);
}

bool ObjectBase::Exists() const
{
    return ::Exists(this);
}

TypeNumber ObjectBase::GetTypeNumber() const
{
    if (!Exists())
        return 0;
    return GetClass()->GetTypeNumber();
}

void Delete(ObjectBase *base)
{
    if (!base)
        return;
    base->Delete();
}



