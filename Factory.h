// (C) 2009 www.christian.schladetsch.net

#ifndef FACTORY_H_INCLUDED
#define FACTORY_H_INCLUDED

#include "Class.h"

/// stores a set of classes and instances
struct Factory
{
	typedef std::map<TypeNumber, ClassBase *> Classes;
	typedef std::map<Handle, ObjectBase *> Instances;
	typedef std::set<Handle> Deathrow;

	Classes classes;
	Instances instances;
	Deathrow deathrow;
	Handle next_handle;

public:
	Factory();
	~Factory();

	/// register a class with the factory
	template <class T>
	void AddClass()
	{
		classes[Traits<T>::Number] = new Class<T>(this);
	}

	/// make a new object given its type
	template <class T>
	T *New()
	{
		Classes::const_iterator klass = classes.find(Traits<T>::Number);
		assert(klass != classes.end());
		if (klass == classes.end())
			return 0;
		return (T *)NewFromClass(klass->second);
	}

	/// make a new object given its type number
	void New(TypeNumber N);

	/// make a new object from its class-base
	ObjectBase *NewFromClass(ClassBase *);

	/// retreive an object given its handle
	ObjectBase *GetObject(Handle handle) const;

	/// delete an existing object
	void Delete(ObjectBase *);
	void Delete(Handle handle);

	/// release resources for all objects pendind deletion
	void Purge();
};

#endif // FACTORY_H_INCLUDED

//EOF
