#include<vector>
#include<string>
#include<iostream>
using namespace std;

#ifndef POINT_H
#define POINT_H

class Point
{
public:
	/// <summary>
	/// Global precision for any calculation
	/// </summary>
	static const int Precision = 10;
	vector<double> data;

	/// <summary>
	/// Build a new Point
	/// </summary>
	/// <param name="dim">The dimension</param>
	Point(int dim)
	{
		data.reserve(dim);
	}

	Point(double x, double y)
	{
		data.reserve(2);
		data.push_back(x);
		data.push_back(y);
	}
	Point()
	{
		data.reserve(2);
		data.push_back(NAN);
		data.push_back(NAN);
	}

	/// <summary>
	/// Build a new Point as a copy of an existing one
	/// </summary>
	/// <param name="O">The existing vector</param>
	Point(const Point &O)
	{
		data = O.data;
	}

	/// <summary>
	/// Build a new Point
	/// </summary>
	/// <param name="X">The elements of the vector</param>
	Point(vector<double> X)
	{
		data = X;
	}

	int Dimension()
	{
		return data.size();
	}

	/// <summary>
	/// Scale all elements by r
	/// </summary>
	/// <param name="r">The scalar</param>
	void Multiply(double r)
	{
		for (int i = 0; i < data.size(); i++) data.at(i) *= r;
	}

	static double Distance(Point a, Point b)
	{
		if (a.Dimension() != b.Dimension())
			return -1;

		double E = 0, D;
		for (int i = 0; i < a.Dimension(); i++)
		{
			D = (a.data[i] - b.data[i]);
			E += D * D;
		}
		return E;
	}

	string ToString()
	{
		string s = "(";
		for (int i = 0; i < data.size(); i++)
		{
			s += to_string(data.at(i));
			if (i == data.size() - 1)
				/* do nothing */;
			else
				s += ",";
		}
		s += ") ";
		return s;
	}

	/// <summary>
	/// Compares this Point with another one
	/// </summary>
	/// <param name="obj"></param>
	/// <returns>Bool</returns>
	bool Equals(vector<double> p2)
	{
		return data == p2;
	}

	/// <summary>
	/// Add two Points
	/// </summary>
	Point operator+(const Point &b)
	{
		if (b.data.size() != data.size())
			throw new exception("Points of different dimensions.\n");
		
		Point p(data.size());
		for (int i = 0; i < data.size(); i++)
		{
			p.data.at(i) = data.at(i) + b.data.at(i);
			data.at(i) += b.data.at(i);
		}
		return p;
	}

	/// <summary>
	/// Subtract two Points
	/// </summary>
	Point operator-(const Point &b)
	{
		if (b.data.size() != data.size())
			throw new exception("Points of different dimensions.\n");

		Point p(data.size());
		for (int i = 0; i < data.size(); i++)
		{
			p.data.at(i) = data.at(i) - b.data.at(i);
			data.at(i) -= b.data.at(i);
		}
		return p;
	}
};
#endif