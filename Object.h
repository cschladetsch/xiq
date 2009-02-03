// (C) 2009 www.christian.schladetsch.net

#ifndef OBJECT_H_INCLUDED
#	define OBJECT_H_INCLUDED

#	include "./ObjectBase.h"

/// Base class for all moving objects in the game
struct Object : ObjectBase
{
protected:
	friend struct Game;
	Point location;
	Direction direction;
	float speed;
	float radius;
	Color color;
	bool collides;
	Game *game;

public:
	Object() : location(0,0), direction(), speed(0), collides(false) { }

	/// general update method for objects. return false to delete the object
	virtual bool Update(GameTime) = 0;

	/// draw the object
	virtual void Draw(Matrix const &) = 0;

	Game *GetRoot() const { return game; }

	template <class T>
	T *New()
	{
		T *obj = ObjectBase::template New<T>();
		obj->game = game;
		return obj;
	}

	float GetRadius() const { return radius; }
	Color GetColor() const { return color; }
	Point GetLocation() const { return location; }
	Direction GetDirection() const { return direction; }
	bool Collides() const { return collides; }
	float GetSpeed() const { return speed; }

	virtual bool Moving() const { return direction != Direction::None; }
	bool IsMoving(Direction D) const { return direction == D; }

	virtual void SetRadius(float R) { radius = R; }
	virtual void SetLocation(Point P) { location = P; }
	virtual void SetDirection(Direction D) { direction = D; }
	virtual void SetSpeed(float S) { speed = S; }
	virtual void SetColor(Color S) { color = S; }

};

#endif // OBJECT_H_INCLUDED

//EOF
