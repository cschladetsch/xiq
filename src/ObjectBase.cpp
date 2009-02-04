// (C) 2009 christian.schladetsch@gmail.com

#include "Common.h"
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

bool Exists(ObjectBase *base)
{
	return base && base->GetFactory() && base->GetFactory()->Exists(base);
}


void Delete(ObjectBase *base)
{
	if (!base)
		return;
	base->Delete();
}


//EOF
