// OOP.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"

#include <ostream>
#include <iostream>
#include <random>
#include<memory>

#include "Container.cpp"
#include "Base.h"
#include "Shapes.h"
#include "Factory.h"

vector<shared_ptr<ShapeFactory>> factory{
	shared_ptr<ShapeFactory>(new Factory<Point>), 
	shared_ptr<ShapeFactory>(new Factory<Circle>),
	shared_ptr<ShapeFactory>(new Factory<Rect>),
	shared_ptr<ShapeFactory>(new Factory<Square>),
	shared_ptr<ShapeFactory>(new Factory<Polyline>),
	shared_ptr<ShapeFactory>(new Factory<Polygon>)
};


int main()
{

	cout << "Shape counter is " << Shape::GetCount() << " at beginning" << endl << endl;

	Container<shared_ptr<Shape>> shapes;
	int count = 20 + rand() % 30;
	for (int i = 0; i < count; ++i) {
		shapes.PushBack( shared_ptr<Shape>(factory[rand() % factory.size()]->Create()) );
	}

	cout << "Count is " << count << endl;
	cout << "Number of shapes by Shape counter is " << (int)Shape::GetCount() << endl << endl;

	shapes.Foreach([](shared_ptr<const Shape> shape) { cout << shape->AsString() << endl; });

	shapes.Reset();

	cout << "Shapes array is empty : " << (shapes.IsEmpty() ? "true" : "false") << endl;
	cout << "Number of shapes by Shape counter is " << (int)Shape::GetCount() << endl;

	getchar();

    return 0;
}

