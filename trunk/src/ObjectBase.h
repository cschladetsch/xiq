// (C) 2009 christian.schladetsch@gmail.com

#ifndef OBJECTBASE_H_INCLUDED
#define OBJECTBASE_H_INCLUDED

/// common for all game types
struct ObjectBase
{
protected:
	friend struct Factory;
	friend struct Object;
	template <class T>
	friend struct Class;

	ClassBase const *klass;	///< the class that made this object.
	Handle handle;

	ObjectBase() : klass(0) { }
	virtual ~ObjectBase() { }

public:

	Factory *GetFactory() const;
	ClassBase const *GetClass() const { return klass; }
	TypeNumber GetTypeNumber() const;
	Handle GetHandle() const { return handle; }

	virtual void Construct() { }
	void Delete();

	template <class T>
	T *New()
	{
		return GetFactory()->template New<T>();
	}
	bool Exists() const;

	template <class T>
	bool IsType() const
	{
		return GetTypeNumber() == Traits<T>::Number;
	}
};

bool Exists(ObjectBase const *);
void Delete(ObjectBase *);

#endif // OBJECTBASE_H_INCLUDED

//EOF
