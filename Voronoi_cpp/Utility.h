#include"VoronoiGraph.h"
#include"VEdgeNode.h"
#include"VDataEvent.h"
#include"VCircleEvent.h"
#include"VEdgeNode.h"

#include<limits>
#include<queue>
#include<map>
#include<memory>
using namespace std;

#ifndef UTILITY_FUNCTI0NS_H
#define UTILITY_FUNCTI0NS_H

/********************************************************
*				FORWARD DECLARATIONS					*
*********************************************************/

double Distance(double x1, double y1, double x2, double y2);
int ccw(double P0x, double P0y, double P1x, double P1y, double P2x, double P2y, bool PlusOneOnZeroDegrees);
int ccw(Point P0, Point P1, Point P2, bool PlusOneOnZeroDegrees);
Point VVinfinite();
Point VVUnknown();
double ParabolicCut(double x1, double y1, double x2, double y2, double ys);
Point CircumCircleCenter(Point A, Point B, Point C);
shared_ptr<VDataNode> FirstDataNode(shared_ptr<VNode> Root);
shared_ptr<VDataNode> LeftDataNode(shared_ptr<VDataNode> Current);
shared_ptr<VDataNode> RightDataNode(shared_ptr<VDataNode> Current);
shared_ptr<VEdgeNode> EdgeToRightDataNode(shared_ptr<VDataNode> Current);
shared_ptr<VDataNode> FindDataNode(shared_ptr<VNode> Root, double ys, double x);
shared_ptr<VNode> ProcessDataEvent(shared_ptr<VDataEvent> e, shared_ptr<VNode> Root, VoronoiGraph VG, double ys, vector<shared_ptr<VDataNode>>& CircleCheckList);
shared_ptr<VNode> ProcessCircleEvent(shared_ptr<VCircleEvent> e, shared_ptr<VNode> Root, shared_ptr<VoronoiGraph> VG, double ys, vector<shared_ptr<VDataNode>>& CircleCheckList);
shared_ptr<VCircleEvent> CircleCheckDataNode(VDataNode n, double ys);
void CleanUpTree(shared_ptr<VNode> Root);
VoronoiGraph ComputeVoronoiGraph(vector<Point> Datapoints);

double Distance(double x1, double y1, double x2, double y2)
{
	return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

int ccw(double P0x, double P0y, double P1x, double P1y, double P2x, double P2y, bool PlusOneOnZeroDegrees)
{
	double dx1, dx2, dy1, dy2;
	dx1 = P1x - P0x; dy1 = P1y - P0y;
	dx2 = P2x - P0x; dy2 = P2y - P0y;
	if (dx1 * dy2 > dy1 * dx2) return +1;
	if (dx1 * dy2 < dy1 * dx2) return -1;
	if ((dx1 * dx2 < 0) || (dy1 * dy2 < 0)) return -1;
	if ((dx1 * dx1 + dy1 * dy1) < (dx2 * dx2 + dy2 * dy2) && PlusOneOnZeroDegrees)
		return +1;
	return 0;
}

int ccw(Point P0, Point P1, Point P2, bool PlusOneOnZeroDegrees)
{
	int dx1, dx2, dy1, dy2;
	dx1 = P1.data[0] - P0.data[0];
	dy1 = P1.data[1] - P0.data[1];
	dx2 = P2.data[0] - P0.data[0];
	dy2 = P2.data[1] - P0.data[1];
	if (dx1 * dy2 > dy1 * dx2) return +1;
	if (dx1 * dy2 < dy1 * dx2) return -1;
	if ((dx1 * dx2 < 0) || (dy1 * dy2 < 0)) return -1;
	if ((dx1 * dx1 + dy1 * dy1) < (dx2 * dx2 + dy2 * dy2) && PlusOneOnZeroDegrees)
		return +1;
	return 0;
}

Point VVinfinite()
{
	Point p(numeric_limits<double>::infinity(), numeric_limits<double>::infinity());
	return p;
}

Point VVUnknown()
{
	Point p(NAN, NAN);
	return p;
}

double ParabolicCut(double x1, double y1, double x2, double y2, double ys)
{
	if (abs(x1 - x2) < 1e-10 && abs(y1 - y2) < 1e-10)
	{
		throw new exception("Identical datapoints are not allowed!");
	}

	if (abs(y1 - ys) < 1e-10 && abs(y2 - ys) < 1e-10) return (x1 + x2) / 2; else;
	if (abs(y1 - ys) < 1e-10) return x1; else;
	if (abs(y2 - ys) < 1e-10) return x2; else;
	double a1 = 1 / (2 * (y1 - ys));
	double a2 = 1 / (2 * (y2 - ys));
	if (abs(a1 - a2) < 1e-10) return (x1 + x2) / 2;

	double xs1 = 0.5 / (2 * a1 - 2 * a2) * (4 * a1 * x1 - 4 * a2 * x2 + 2 * sqrt(-8 * a1 * x1 * a2 * x2 - 2 * a1 * y1 + 2 * a1 * y2 + 4 * a1 * a2 * x2 * x2 + 2 * a2 * y1 + 4 * a2 * a1 * x1 * x1 - 2 * a2 * y2));
	double xs2 = 0.5 / (2 * a1 - 2 * a2) * (4 * a1 * x1 - 4 * a2 * x2 - 2 * sqrt(-8 * a1 * x1 * a2 * x2 - 2 * a1 * y1 + 2 * a1 * y2 + 4 * a1 * a2 * x2 * x2 + 2 * a2 * y1 + 4 * a2 * a1 * x1 * x1 - 2 * a2 * y2));
	if (xs1 > xs2)
	{
		double h = xs1;
		xs1 = xs2;
		xs2 = h;
	}
	if (y1 >= y2) return xs2;
	return xs1;
}

VoronoiGraph ComputeVoronoiGraph(vector<Point> Datapoints)
{
	priority_queue<VDataEvent> PQ;
	map<VDataNode, VCircleEvent> CurrentCircles;
	shared_ptr<VoronoiGraph> VG;
	shared_ptr<VNode>  RootNode = NULL;

	for (vector<Point>::iterator i = Datapoints.begin(); i != Datapoints.end(); i++)
	{
		VDataEvent vd(*i);
		PQ.push(vd);
	}
	while (PQ.size() > 0)
	{
		VDataEvent temp = PQ.top();
		shared_ptr<VEvent> VE = make_shared<VEvent>(temp);
		vector<shared_ptr<VDataNode>> CircleCheckList;

		if (typeid(VE) == typeid(VDataEvent))
		{
			RootNode = ProcessDataEvent(static_pointer_cast<VDataEvent>(VE), RootNode, *VG, VE->Y, CircleCheckList);
		}
		else if (typeid(VE) == typeid(VCircleEvent))
		{
			shared_ptr<VCircleEvent> VE_temp = make_shared<VCircleEvent>(VE);
			CurrentCircles.erase(VE_temp->NodeN);
			if (!VE_temp->Valid)
				continue;
			RootNode = ProcessCircleEvent(static_pointer_cast<VCircleEvent>(VE), RootNode, VG, VE->Y, CircleCheckList);
		}
		else
		{
			string error_message = "Got event of type ";
			error_message += typeid(VE).name();
			error_message += "!";
			throw new exception(error_message.c_str());
		}

		for (vector<shared_ptr<VDataNode>>::iterator i = CircleCheckList.begin(); i != CircleCheckList.end(); i++) // VDataNode VD in CircleCheckList)
		{
			VDataNode VD = **i;

			map<VDataNode, VCircleEvent>::iterator it = CurrentCircles.find(VD);
			if (it != CurrentCircles.end())
			{
				CurrentCircles[VD].Valid = false;
				CurrentCircles.erase(VD);
			}
			shared_ptr<VCircleEvent> VCE = CircleCheckDataNode(VD, VE->Y);
			if (VCE != NULL)
			{
				PQ.push(*static_pointer_cast<VDataEvent>(VCE));
				CurrentCircles[VD] = *VCE;
			}
		}
		if (typeid(VE) == typeid(VDataEvent))
		{
			VDataEvent VDE_temp = *static_pointer_cast<VDataEvent>(VE);
			Point DP = VDE_temp.DataPoint;
			for (map<VDataNode, VCircleEvent>::iterator i; i != CurrentCircles.end(); i++)
			{

				VCircleEvent VCE = i->second;
				if (Distance(DP.data[0], DP.data[1], VCE.Center.data[0], VCE.Center.data[1]) < VCE.Y() - VCE.Center.data[1]
					&& abs(Distance(DP.data[0], DP.data[1], VCE.Center.data[0], VCE.Center.data[1]) - (VCE.Y() - VCE.Center.data[1])) > 1e-10)
					VCE.Valid = false;
			}
		}
	}
	CleanUpTree(RootNode);

	for (unordered_set<VoronoiEdge>::iterator i = VG->Edges.begin(); i != VG->Edges.end(); i++)//(VoronoiEdge VE in VG.Edges)
	{
		VoronoiEdge VE = *i;
		if (VE.Done)
			continue;
		if (VE.VVertexB.Equals(VVUnknown().data))
		{
			VE.AddVertex(VVinfinite());
			if (abs(VE.LeftData.data[1] - VE.RightData.data[1]) < 1e-10 && VE.LeftData.data[0] < VE.RightData.data[0])
			{
				Point T = VE.LeftData;
				VE.LeftData = VE.RightData;
				VE.RightData = T;
			}
		}
	}

	vector<VoronoiEdge> MinuteEdges;
	for (unordered_set<VoronoiEdge>::iterator i = VG->Edges.begin(); i != VG->Edges.end(); i++)
	{
		VoronoiEdge VE = *i;

		if (!VE.IsPartlyInfinite() && VE.VVertexA.Equals(VE.VVertexB.data))
		{
			MinuteEdges.push_back(VE);
			// prevent rounding errors from expanding to holes
			for (unordered_set<VoronoiEdge>::iterator i2 = VG->Edges.begin(); i2 != VG->Edges.end(); i2++)
			{
				VoronoiEdge VE2 = *i2;
				if (VE2.VVertexA.Equals(VE.VVertexA.data))
					VE2.VVertexA = VE.VVertexA;

				if (VE2.VVertexB.Equals(VE.VVertexA.data))
					VE2.VVertexB = VE.VVertexA;
			}
		}
	}

	for (vector<VoronoiEdge>::iterator k = MinuteEdges.begin(); k != MinuteEdges.end(); k++)
		VG->Edges.erase(*k);

	return *VG;
}

Point CircumCircleCenter(Point A, Point B, Point C)
{
	if (A.Equals(B.data) || B.Equals(C.data) || A.Equals(C.data))
		throw new exception("Need three different points!");
	double tx = (A.data[0] + C.data[0]) / 2;
	double ty = (A.data[1] + C.data[1]) / 2;

	double vx = (B.data[0] + C.data[0]) / 2;
	double vy = (B.data[1] + C.data[1]) / 2;

	double ux, uy, wx, wy;

	if (A.data[0] == C.data[0])
	{
		ux = 1;
		uy = 0;
	}
	else
	{
		ux = (C.data[1] - A.data[1]) / (A.data[0] - C.data[0]);
		uy = 1;
	}

	if (B.data[0] == C.data[0])
	{
		wx = -1;
		wy = 0;
	}
	else
	{
		wx = (B.data[1] - C.data[1]) / (B.data[0] - C.data[0]);
		wy = -1;
	}

	double alpha = (wy * (vx - tx) - wx * (vy - ty)) / (ux * wy - wx * uy);

	Point p(tx + alpha * ux, ty + alpha * uy);
	return p;
}

shared_ptr<VDataNode> FirstDataNode(shared_ptr<VNode> Root)
{
	shared_ptr<VNode> C = Root;
	while (C->Left() != NULL)
		C = C->Left();
	return static_pointer_cast<VDataNode>(C);
}

shared_ptr<VDataNode> LeftDataNode(shared_ptr<VDataNode> Current)
{
	shared_ptr<VNode> C = static_pointer_cast<VNode>(Current);
	//1. Up
	do
	{
		if (C->Parent() == NULL)
			return NULL;
		if (C->Parent()->Left() == C)
		{
			C = C->Parent();
			continue;
		}
		else
		{
			C = C->Parent();
			break;
		}
	} while (true);
	//2. One Left
	C = C->Left();
	//3. Down
	while (C->Right() != NULL)
		C = C->Right();
	return static_pointer_cast<VDataNode>(C);
}

shared_ptr<VDataNode> RightDataNode(shared_ptr<VDataNode> Current)
{
	shared_ptr<VNode> C = static_pointer_cast<VNode>(Current);
	//1. Up
	do
	{
		if (C->Parent() == NULL)
			return NULL;
		if (C->Parent()->Right() == C)
		{
			C = C->Parent();
			continue;
		}
		else
		{
			C = C->Parent();
			break;
		}
	} while (true);
	//2. One Right
	C = C->Right();
	//3. Down
	while (C->Left() != NULL)
		C = C->Left();
	return static_pointer_cast<VDataNode>(C);
}

shared_ptr<VEdgeNode> EdgeToRightDataNode(shared_ptr<VDataNode> Current)
{
	shared_ptr<VNode> C = static_pointer_cast<VNode>(Current);
	//1. Up
	do
	{
		if (C->Parent() == NULL)
			throw new exception("No Left Leaf found!");
		if (C->Parent()->Right() == C)
		{
			C = C->Parent();
			continue;
		}
		else
		{
			C = C->Parent();
			break;
		}
	} while (true);
	return static_pointer_cast<VEdgeNode>(C);
}

shared_ptr<VDataNode> FindDataNode(shared_ptr<VNode> Root, double ys, double x)
{
	shared_ptr<VNode> C = Root;
	do
	{
		if (typeid(C) == typeid(VDataNode))
		{
			return static_pointer_cast<VDataNode>(C);
		}
		if (make_shared<VEdgeNode>(C).get()->Cut(ys, x) < 0)
			C = C->Left();
		else
			C = C->Right();
	} while (true);
}

/// <summary>
/// Will return the new root (unchanged except in start-up)
/// </summary>
shared_ptr<VNode> ProcessDataEvent(shared_ptr<VDataEvent> e, shared_ptr<VNode> Root, VoronoiGraph VG, double ys, vector<shared_ptr<VDataNode>>& CircleCheckList)
{
	if (Root == NULL)
	{
		Root = static_pointer_cast<VNode>(make_shared<VDataNode>(e.get()->DataPoint));
		CircleCheckList.push_back(shared_ptr<VDataNode>(make_shared<VDataNode>(e->DataPoint)));
		return Root;
	}

	//1. Find the node to be replaced
	shared_ptr<VNode> C = FindDataNode(Root, ys, e->DataPoint.data[0]);

	//2. Create the subtree (ONE Edge, but two VEdgeNodes)
	shared_ptr<VoronoiEdge> VE = make_shared<VoronoiEdge>();

	shared_ptr<VDataNode> C_temp = static_pointer_cast<VDataNode>(C);
	VE->LeftData = C_temp->DataPoint;
	VE->RightData = e->DataPoint;
	VE->VVertexA = VVUnknown();
	VE->VVertexB = VVUnknown();
	VG.Edges.insert(*VE);

	shared_ptr<VNode> SubRoot;
	if (abs(VE->LeftData.data[1] - VE->RightData.data[1]) < 1e-10)
	{
		if (VE->LeftData.data[0] < VE->RightData.data[0])
		{
			SubRoot = make_shared<VEdgeNode>(*VE, false);
			SubRoot->Left(make_shared<VDataNode>(VE->LeftData));
			SubRoot->Right(make_shared<VDataNode>(VE->RightData));
		}
		else
		{
			SubRoot = make_shared<VEdgeNode>(*VE, true);
			SubRoot->Left(make_shared<VDataNode>(VE->RightData));
			SubRoot->Right(make_shared<VDataNode>(VE->LeftData));
		}
		CircleCheckList.push_back(static_pointer_cast<VDataNode>(SubRoot->Left()));
		CircleCheckList.push_back(static_pointer_cast<VDataNode>(SubRoot->Right()));
	}
	else
	{
		SubRoot = make_shared<VEdgeNode>(*VE, false);
		SubRoot->Left(make_shared<VDataNode>(VE->LeftData));
		SubRoot->Right(make_shared<VEdgeNode>(*VE, true));
		SubRoot->Right()->Left(make_shared<VDataNode>(VE->RightData));
		SubRoot->Right()->Right(make_shared<VDataNode>(VE->LeftData));
		CircleCheckList.push_back(static_pointer_cast<VDataNode>(SubRoot->Left()));
		CircleCheckList.push_back(static_pointer_cast<VDataNode>(SubRoot->Right()->Left()));
		CircleCheckList.push_back(static_pointer_cast<VDataNode>(SubRoot->Right()->Right()));
	}

	//3. Apply subtree
	if (C->Parent() == NULL)
		return SubRoot;
	C->Parent()->Replace(C, SubRoot);
	return Root;
}

shared_ptr<VNode> ProcessCircleEvent(shared_ptr<VCircleEvent> e, shared_ptr<VNode> Root, shared_ptr<VoronoiGraph> VG, double ys, vector<shared_ptr<VDataNode>>& CircleCheckList)
{
	shared_ptr<VDataNode> a;
	shared_ptr<VDataNode> b;
	shared_ptr<VDataNode> c;

	shared_ptr<VEdgeNode> eu;
	shared_ptr<VEdgeNode> eo;

	b = make_shared<VDataNode>(e->NodeN);
	a = LeftDataNode(b);
	c = RightDataNode(b);
	if (a == NULL || b->Parent() == NULL || c == NULL || !a->DataPoint.Equals(e->NodeL.DataPoint.data) || !c->DataPoint.Equals(e->NodeR.DataPoint.data))
	{
		return Root;
	}
	eu = static_pointer_cast<VEdgeNode>(b->Parent());
	CircleCheckList.push_back(a);
	CircleCheckList.push_back(c);

	//1. Create the new Vertex
	Point VNew(e->Center.data[0], e->Center.data[1]);
	VG->Vertices.insert(VNew);

	//2. Find out if a or c are in a distand part of the tree (the other is then b's sibling) and assign the new vertex
	if (eu->Left() == b) // c is sibling
	{
		eo = EdgeToRightDataNode(a);

		// replace eu by eu's Right
		eu->Parent()->Replace(eu, eu->Right());
	}
	else // a is sibling
	{
		eo = EdgeToRightDataNode(b);
		// replace eu by eu's Left
		eu->Parent()->Replace(eu, eu->Left());
	}
	eu->Edge.AddVertex(VNew);
	eo->Edge.AddVertex(VNew);

	//2. Replace eo by new Edge
	VoronoiEdge VE;// = new VoronoiEdge();
	VE.LeftData = a->DataPoint;
	VE.RightData = c->DataPoint;
	VE.AddVertex(VNew);
	VG.get()->Edges.insert(VE);

	shared_ptr<VEdgeNode> VEN = make_shared<VEdgeNode>(VE, false);
	VEN->Left(eo->Left());
	VEN->Right(eo->Right());
	if (eo->Parent() == NULL)
		return static_pointer_cast<VNode>(VEN);

	eo->Parent()->Replace(eo, VEN);
	return Root;
}

shared_ptr<VCircleEvent> CircleCheckDataNode(VDataNode n, double ys)
{
	shared_ptr<VDataNode> l = LeftDataNode(make_shared<VDataNode>(n));
	shared_ptr<VDataNode> r = RightDataNode(make_shared<VDataNode>(n));

	if (l == NULL || r == NULL || l->DataPoint.Equals(r->DataPoint.data) || l->DataPoint.Equals(n.DataPoint.data) || n.DataPoint.Equals(r->DataPoint.data))
		return NULL;

	if (ccw(l->DataPoint.data[0], l->DataPoint.data[1], n.DataPoint.data[0], n.DataPoint.data[1], r->DataPoint.data[0], r->DataPoint.data[1], false) <= 0)
		return NULL;

	Point Center = CircumCircleCenter(l->DataPoint, n.DataPoint, r->DataPoint);
	shared_ptr<VCircleEvent> VC = make_shared<VCircleEvent>();
	VC->NodeN = n;
	VC->NodeL = *l;
	VC->NodeR = *r;
	VC->Center = Center;
	VC->Valid = true;
	if (VC->Y() > ys || abs(VC->Y() - ys) < 1e-10)
		return VC;
	return NULL;
}

void CleanUpTree(shared_ptr<VNode> Root)
{
	if (typeid(*Root) == typeid(VDataNode))
		return;

	shared_ptr<VEdgeNode> VE = static_pointer_cast<VEdgeNode>(Root);
	while (VE->Edge.VVertexB.Equals(VVUnknown().data))
		VE->Edge.AddVertex(VVinfinite());

	if (VE->Flipped)
	{
		Point T = VE->Edge.LeftData;
		VE->Edge.LeftData = VE->Edge.RightData;
		VE->Edge.RightData = T;
	}
	VE->Edge.Done = true;
	CleanUpTree(Root->Left());
	CleanUpTree(Root->Right());
}

#endif