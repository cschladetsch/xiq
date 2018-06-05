// (C) 2009 christian.schladetsch@gmail.com

#pragma once

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

    /// delete an existing object.
    /// resources will be deleted on the next PurSge (see Factory::Purge)
    void Delete(ObjectBase *);
    void Delete(Handle handle);

    /// return true if the given object `exists`.
    /// when an object is Deleted as above, it still exists
    /// in system memory until the Purge method is called
    /// on the factory that made the object.
    /// Exists() returns false for objects that have:
    ///        * never created
    ///        * have been created and have been released
    ///        * currently created, but have been deleted, waiting for next Purge
    bool Exists(ObjectBase const *) const;
    bool Exists(Handle) const;

    /// release resources for all objects pending deletion
    void Purge();
};

