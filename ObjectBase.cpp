// (C) 2009 www.christian.schladetsch.net

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

//EOF
