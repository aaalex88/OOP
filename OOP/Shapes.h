#pragma once

#include <ostream>
#define _USE_MATH_DEFINES
#include <math.h>

#include <assert.h>
#include <random>


#include "Base.h"
#include "Container.h"


class Shape : public Printable
{
public:
	Shape() { m_count++; }
	virtual ~Shape() { m_count--; }

	string AsString() const override final { return ShapeName() + " { " + ShapeContent() + " }\n"; }
	virtual string ShapeName() const = 0;
	virtual string ShapeContent() const = 0;

	static size_t GetCount() { return m_count; }

protected:

private:

	static size_t m_count;
};

size_t Shape::m_count = 0;

ostream & operator << (ostream & ioStream, Shape & shape)
{
	ioStream << shape.AsString();
	return ioStream;
}


double Rand()
{
	return double(rand()) / double(RAND_MAX);
}

double Rand(double min, double max)
{
	// ..we don't need  min<max
	return Rand()*(max - min) + min;
}


class Point : public Shape
{
public:
	Point(double x, double y) : Shape(), m_x(x), m_y(y) {}
	Point(const Point & p) : Shape(), m_x(p.m_x), m_y(p.m_y) {}

	string ShapeName() const override { return "Point"; }
	string ShapeContent() const override { return string("Position:") + to_string(m_x) + ";" + to_string(m_y) + ""; }

	double GetX() const { return m_x; }
	double GetY() const { return m_y; }

	Point operator - (const Point & p) const { return Point(m_x - p.m_x, m_y - p.m_y); }
	double Norm() const { return sqrt(m_x*m_x + m_y*m_y); }
	double operator * (const Point & p) const { return (m_x*p.m_x + m_y*p.m_y); }
	Point operator * (double d) const { return Point(m_x*d, m_y*d); }
	Point operator / (double d) const { return *this * (1.0/d); }
	static Point Project(const Point & p, const Point & axis) { return axis*(p*axis) / pow(axis.Norm(), 2.0); }

	bool OnRightSide(const Point & p) { return m_y*p.m_x + m_x*p.m_y > 0.0; }

	static double Distance(const Point & p1, const Point & p2) { return (p1 - p2).Norm(); }
	static double AreaOfTriangle(const Point & p1, const Point & p2, const Point & p3) { 
		Point base = p2 - p1;
		Point toTop = p3 - p1;
	//	double d = base.Norm();
		double height = ( toTop - Project(toTop, base) ).Norm();
		return 0.5 * base.Norm() * height; 
	}


	static Point CreateRandom() {
		return Point(Rand(-100, 100), Rand(-100, 100));
	}

protected:
	const double m_x, m_y;
};

static Point operator * (double d, const Point & p) { return p*d; }



class Circle : public Shape
{
public:
	Circle(double x, double y, double r) : Shape(), m_pos(x, y), m_radius(r) {}

	string ShapeName() const override { return "Circle"; }
	string ShapeContent() const override { return m_pos.ShapeContent() + " Radius:" + to_string(m_radius) + " Area:" + to_string(M_PI*m_radius*m_radius); }

	static Circle CreateRandom() {
		return Circle(Rand(-100, 100), Rand(-100, 100), Rand(10, 20));
	}
protected:
	const Point m_pos;
	const double m_radius;
};


class Rect : public Shape
{
public:
	Rect(double left, double top, double right, double bottom) : Shape(), m_leftTop(left, top), m_rightBottom(right, bottom)
	{
		assert(left < right && bottom < top);
	}

	string ShapeName() const override { return "Rect"; }
	string ShapeContent() const override {
		return string("Center:") + to_string(GetCenter().GetX()) + ";" + to_string(GetCenter().GetX()) +
			+ " Area:" + to_string(  (m_rightBottom.GetX() - m_leftTop.GetX()) * (m_leftTop.GetY() - m_rightBottom.GetY())  );
	}

	double Area() const {
		double area = (m_rightBottom.GetX() - m_leftTop.GetX()) * (m_leftTop.GetY() - m_rightBottom.GetY());
		return area;
	}

	static Rect CreateRandom() {
		return Rect(Rand(-100, 0), Rand(0, 100), Rand(0, 100), Rand(-100, 0));
	}
protected:
	Point GetCenter() const { 
		double x = (m_leftTop.GetX() + m_rightBottom.GetX()) / 2.0;
		double y = (m_leftTop.GetY() + m_rightBottom.GetY()) / 2.0;
		return Point(x, y); 
	}

	const Point m_leftTop, m_rightBottom;
};


class Square : public Rect
{
public:
	Square(double left, double top, double side) : Rect(left, top, left+side, top-side) {}

	string ShapeName() const override { return "Square"; }
//	string ShapeContent() const override {} // don't need to change Rect's one

	static Square CreateRandom() {
		return Square(Rand(-100, 100), Rand(-100, 100), Rand(0, 100));
	}
protected:
	// nothing?
};


class Polyline : public Shape
{
public:
	Polyline() : Shape() {}
	Polyline(const Polyline & p) : Shape(), m_points(p.m_points) {}

	string ShapeName() const override { return "Polyline"; }
	string ShapeContent() const override {		
		return string("Number Of Segments:") + to_string(NumberOfSegments()) + string("; Length:") + to_string(Length());
	}

	void AddPoint(Point p) { m_points.PushBack(p); }

	size_t NumberOfSegments() const {
		size_t nPoints = m_points.Size();
		if (nPoints == 0) {
			return 0;
		}
		return nPoints - 1;
	}

	double Length() const {
		if (m_points.Size() < 2) {
			return 0.0;
		}

		double length = 0.0;
		for (size_t i = 1; i < m_points.Size(); ++i) {
			length += Point::Distance(m_points[i], m_points[i-1]);
		}

		return length;
	}

	static Polyline CreateRandom() {
		Polyline line;
		int nPoints = 2 + rand()%10;
		for (int i = 0; i < nPoints; ++i) {
			line.AddPoint(Point::CreateRandom());
		}
		return line;
	}
protected:
	Container<Point> m_points;
};


class Polygon : public Shape
{
public:
	Polygon() : Shape() {}
	Polygon(const Polygon & p) : Shape(), m_points(p.m_points) {}

	string ShapeName() const override { return "Polygon"; }
	string ShapeContent() const override {
		return string("Number Of Triangles:") + to_string(NumberOfTriangles()) + string("; Area:") + to_string(Area());
	}

	bool CanBeAdded(const Point & p) { // ensure polygon with new point would be still convex
		if (m_points.Size() <= 1)
			return true;

		for (size_t i = 1; i < m_points.Size(); ++i) {
			if (!(m_points[i] - m_points[i - 1]).OnRightSide(p))
				return false;
		}
		return true;
	}
	void AddPoint(const Point & p) { 
		assert(CanBeAdded(p));
		m_points.PushBack(p); 
	}

	size_t NumberOfTriangles() const {
		size_t nPoints = m_points.Size();
		if (nPoints < 3) {
			return 0;
		}
		return nPoints - 2;
	}

	double Area() const {
		if (m_points.Size() < 3) {
			return 0.0;
		}

		double area = 0.0;
		for (size_t i = 1; i < m_points.Size() - 1; ++i) {
			area += Point::AreaOfTriangle(m_points[i-1], m_points[i], m_points.Last());
		}

		return area;
	}

	static Polygon CreateRandom() {
		Polygon poly;
		int nPoints = 2 + rand() % 10;
		for (int i = 0; i < nPoints; ++i) {
			Point p = Point::CreateRandom();
			if (poly.CanBeAdded(p))
				poly.AddPoint(p);
		}
		return poly;
	}

protected:
	Container<Point> m_points;
};
