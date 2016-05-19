#pragma once

#ifndef VORONOIEDGE_H
#define VORONOIEDGE_H
class VoronoiEdge
{
public:
	bool Done = false;
	Point RightData;
	Point LeftData;
	Point VVertexA;
	Point VVertexB;

	VoronoiEdge()
	{
		Done = false;
		Point p(NAN, NAN);
		RightData = p;
		LeftData = p;
		VVertexA = p;
		VVertexB = p;
	}

	VoronoiEdge operator=(const VoronoiEdge& other)
	{
		Done = other.Done;
		RightData = other.RightData;
		LeftData = other.LeftData;
		VVertexA = other.VVertexA;
		VVertexB = other.VVertexB;
		return *this;
	}

	void AddVertex(Point V)
	{
		if (isnan(VVertexA.data[0]) && isnan(VVertexA.data[1]))
			VVertexA = V;
		else if (isnan(VVertexB.data[0]) && isnan(VVertexB.data[1]))
			VVertexB = V;
		else throw new exception("Tried to add third vertex!");
	}

	bool IsInfinite()
	{
		Point p(numeric_limits<double>::infinity(), numeric_limits<double>::infinity());
		return (VVertexA.Equals(p.data)) && (VVertexB.Equals(p.data));
	}

	bool IsPartlyInfinite()
	{
		return (isinf(VVertexA.data[0]) && isinf(VVertexA.data[1])) || (isinf(VVertexB.data[0]) && isinf(VVertexB.data[1]));
	}

	Point FixedPointI()
	{
		if (this->IsInfinite()) return 0.5 * (LeftData.data[0] + RightData.data[0] + LeftData.data[1] + RightData.data[1]);
		if (!(isinf(VVertexA.data[0]) && isinf(VVertexA.data[1]))) return VVertexA;
		else return VVertexB;
	}

	Point DirectionVector()
	{
		if (!IsPartlyInfinite())
			return Point::Scalar_Multiplication((VVertexB - VVertexA), (1.0 / sqrt(Point::Distance(VVertexA, VVertexB))));
		if (LeftData.data[0] == RightData.data[0])
		{
			if (LeftData.data[1] < RightData.data[1])
			{
				Point p(-1, 0);
				return p;
			}
			Point p(1, 0);
			return p;
		}
		Point Erg(-(RightData.data[1] - LeftData.data[1]) / (RightData.data[0] - LeftData.data[0]), 1);
		if (RightData.data[0] < LeftData.data[0])
			Erg.Multiply(-1);
		Erg.Multiply(1.0 / sqrt(Point::Scalar_Product(Erg, Erg)));
		return Erg;
	}

	double Length()
	{
		if (IsPartlyInfinite())
			return numeric_limits<double>::infinity();
		return sqrt(Point::Distance(VVertexA, VVertexB));
	}
};

#endif