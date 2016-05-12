
#include<unordered_set>
#include"Point.h"

class VoronoiGraph
{
public:
	unordered_set<Point> Vertices;
	unordered_set<VoronoiEdge> Edges;
};

class VoronoiEdge
{
public:
	bool Done = false;
	Point RightData, LeftData;
	Point VVertexA = Fortune::VVUnknown(), VVertexB = Fortune::VVUnknown();

	void AddVertex(Point V)
	{
		if (VVertexA.Equals(Fortune::VVUnknown().data))
			VVertexA = V;
		else if (VVertexB.Equals(Fortune::VVUnknown().data))
			VVertexB = V;
		else throw new exception("Tried to add third vertex!");
	}

	bool IsInfinite()
	{
		return (VVertexA.Equals(Fortune::VVinfinite().data)) && (VVertexB.Equals(Fortune::VVinfinite().data));
	}
	
	bool IsPartlyInfinite()
	{
		return VVertexA.Equals(Fortune::VVinfinite().data) || VVertexB.Equals(Fortune::VVinfinite().data);
	}

	Point FixedPointI()
	{
		if (IsInfinite())
			return 0.5 * (LeftData.data[0] + RightData.data[0] + LeftData.data[1] + RightData.data[1]);
		if (!VVertexA.Equals(Fortune::VVinfinite().data)) return VVertexA;
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