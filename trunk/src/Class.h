// (C) 2009 www.christian.schladetsch.net

#ifndef CLASS_H_INCLUDED
#define CLASS_H_INCLUDED

/// common for all Class<T> structures
struct ClassBase
{
protected:
	TypeNumber type_number;
	Factory *factory;

public:
	ClassBase(TypeNumber N, Factory *F) : type_number(N), factory(F) { }

	TypeNumber GetTypeNumber() const { return type_number; }
	Factory *GetFactory() const { return factory; }

	virtual ObjectBase *Create() const = 0;
};

/// a class for a given type T; used to make new instances
template <class T>
struct Class : ClassBase
{
	Class(Factory *F) : ClassBase(Traits<T>::Number, F) { }

	ObjectBase *Create() const
	{
		T *ptr = new T();
		ptr->klass = this;
		return ptr;
	}
};

#endif // CLASS_H_INCLUDED

//EOF
