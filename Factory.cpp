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

void Factory::Purge()
{
	std::vector<Handle> cp(deathrow.begin(), deathrow.end());
	deathrow.clear();

	std::vector<Handle>::const_iterator begin = cp.begin(), end = cp.end();
	for (; begin != end; ++begin)
	{
		Handle handle = *begin;
		ObjectBase *base = GetObject(handle);
		if (base)
		{
			delete base;
		}
	}
}

//EOF
