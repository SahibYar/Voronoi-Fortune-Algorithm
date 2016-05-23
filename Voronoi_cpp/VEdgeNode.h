#pragma once

#ifndef VEDGENODE_H
#define VEDGENODE_H
#include"VNode.h"
#include"VoronoiEdge.h"

class VEdgeNode : public VNode
{
public:
	///	VEdgeNode(){ }
	shared_ptr<VoronoiEdge> Edge;
	bool Flipped;
	VEdgeNode(shared_ptr<VoronoiEdge> E, bool Fd)
	{
		Edge = E;
		Flipped = Fd;
	}

	VEdgeNode operator=(VEdgeNode& other)
	{
		this->Parent(other.Parent());
		this->Left(other.Parent());
		this->Right(other.Parent());
		Edge = other.Edge;
		Flipped = other.Flipped;
	}
	double Cut(double ys, double x)
	{
		if (!this->Flipped)
		{
			double x1 = this->Edge->LeftData->data[0];
			double y1 = this->Edge->LeftData->data[1];
			double x2 = this->Edge->RightData->data[0];
			double y2 = this->Edge->RightData->data[1];

			if (abs(x1 - x2) < 1e-10 && abs(y1 - y2) < 1e-10)
			{
				throw new exception("Identical datapoints are not allowed!");
			}

			if (abs(y1 - ys) < 1e-10 && abs(y2 - ys) < 1e-10) return x - ((x1 + x2) / 2); else;
			if (abs(y1 - ys) < 1e-10) return x - x1; else;
			if (abs(y2 - ys) < 1e-10) return x - x2; else;
			double a1 = 1 / (2 * (y1 - ys));
			double a2 = 1 / (2 * (y2 - ys));
			if (abs(a1 - a2) < 1e-10) return x - ((x1 + x2) / 2);

			double xs1 = 0.5 / (2 * a1 - 2 * a2) * (4 * a1 * x1 - 4 * a2 * x2 + 2 * sqrt(-8 * a1 * x1 * a2 * x2 - 2 * a1 * y1 + 2 * a1 * y2 + 4 * a1 * a2 * x2 * x2 + 2 * a2 * y1 + 4 * a2 * a1 * x1 * x1 - 2 * a2 * y2));
			double xs2 = 0.5 / (2 * a1 - 2 * a2) * (4 * a1 * x1 - 4 * a2 * x2 - 2 * sqrt(-8 * a1 * x1 * a2 * x2 - 2 * a1 * y1 + 2 * a1 * y2 + 4 * a1 * a2 * x2 * x2 + 2 * a2 * y1 + 4 * a2 * a1 * x1 * x1 - 2 * a2 * y2));
			if (xs1 > xs2)
			{
				double h = xs1;
				xs1 = xs2;
				xs2 = h;
			}
			if (y1 >= y2) return x - xs2;
			return x - xs1;
		}

		double x1 = this->Edge->RightData->data[0];
		double y1 = this->Edge->RightData->data[1];
		double x2 = this->Edge->LeftData->data[0];
		double y2 = this->Edge->LeftData->data[1];

		if (abs(x1 - x2) < 1e-10 && abs(y1 - y2) < 1e-10)
		{
			throw new exception("Identical datapoints are not allowed!");
		}

		if (abs(y1 - ys) < 1e-10 && abs(y2 - ys) < 1e-10) return x - ((x1 + x2) / 2); else;
		if (abs(y1 - ys) < 1e-10) return x - x1; else;
		if (abs(y2 - ys) < 1e-10) return x - x2; else;
		double a1 = 1 / (2 * (y1 - ys));
		double a2 = 1 / (2 * (y2 - ys));
		if (abs(a1 - a2) < 1e-10) return x - ((x1 + x2) / 2);

		double xs1 = 0.5 / (2 * a1 - 2 * a2) * (4 * a1 * x1 - 4 * a2 * x2 + 2 * sqrt(-8 * a1 * x1 * a2 * x2 - 2 * a1 * y1 + 2 * a1 * y2 + 4 * a1 * a2 * x2 * x2 + 2 * a2 * y1 + 4 * a2 * a1 * x1 * x1 - 2 * a2 * y2));
		double xs2 = 0.5 / (2 * a1 - 2 * a2) * (4 * a1 * x1 - 4 * a2 * x2 - 2 * sqrt(-8 * a1 * x1 * a2 * x2 - 2 * a1 * y1 + 2 * a1 * y2 + 4 * a1 * a2 * x2 * x2 + 2 * a2 * y1 + 4 * a2 * a1 * x1 * x1 - 2 * a2 * y2));
		if (xs1 > xs2)
		{
			double h = xs1;
			xs1 = xs2;
			xs2 = h;
		}
		if (y1 >= y2) return x - xs2;
		return x - xs1;
	}
};

#endif