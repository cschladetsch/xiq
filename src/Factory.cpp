// (C) 2009 christian.schladetsch.net

#include "Common.h"
#include "Object.h"
#include "Factory.h"

Factory::Factory()
{
}

ObjectBase *Factory::NewFromClass(ClassBase *klass)
{
	assert(klass != 0);
	if (!klass)
		return 0;
	ObjectBase *base = klass->Create();
	base->handle = ++next_handle;
	base->Construct();
	return base;
}

/// make a new object given its type number
void Factory::New(TypeNumber N)
{
	Classes::const_iterator klass = classes.find(N);
	assert(klass != classes.end());
	if (klass != classes.end())
		NewFromClass(klass->second);
}

/// delete an existing object
void Factory::Delete(ObjectBase *base)
{
	assert(base != 0);
	if (base)
		Delete(base->GetHandle());
}

void Factory::Delete(Handle handle)
{
	Instances::const_iterator obj = instances.find(handle);
	if (obj != instances.end())
	{
		deathrow.insert(handle);
	}
}

ObjectBase *Factory::GetObject(Handle handle) const
{
	Instances::const_iterator instance = instances.find(handle);
	return instance == instances.end() ? 0 : instance->second;
}

bool Factory::Exists(ObjectBase *base) const
{
	if (!base)
		return false;
	return Exists(base->GetHandle());
}

bool Factory::Exists(Handle handle) const
{
	if (deathrow.find(handle) != deathrow.end())
		return false;
	return instances.find(handle) != instances.end();
}

void Factory::Purge()
{
	// make a copy of deathrow, since as we delete objects they may
	// in turn delete other objects, which would change deathrow
	std::vector<Handle> cp(deathrow.begin(), deathrow.end());
	deathrow.clear();

	foreach (Handle handle, cp)
	{
		ObjectBase *base = GetObject(handle);
		if (base)
		{
			delete base;
		}
	}
}

//EOF
