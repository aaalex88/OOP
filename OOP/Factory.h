#pragma once




class Shape;


class ShapeFactory
{
public:
	virtual Shape * Create() const = 0;
};

template <typename T>
class Factory: public ShapeFactory
{
public:
	Shape * Create() const override { return new T(T::CreateRandom()); }
};



