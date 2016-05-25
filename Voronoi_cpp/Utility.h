#include"VoronoiGraph.h"
#include"VEdgeNode.h"
#include"VDataEvent.h"
#include"VCircleEvent.h"
#include"Compare.h"

#include<limits>
#include<map>
#include<queue>
#include<memory>
using namespace std;

#ifndef UTILITY_FUNCTI0NS_H
#define UTILITY_FUNCTI0NS_H

/********************************************************
*				FORWARD DECLARATIONS					*
*********************************************************/
double Distance(double x1, double y1, double x2, double y2);
int ccw(double P0x, double P0y, double P1x, double P1y, double P2x, double P2y, bool PlusOneOnZeroDegrees);
double ParabolicCut(double x1, double y1, double x2, double y2, double ys);
Point CircumCircleCenter(Point A, Point B, Point C);
shared_ptr<VDataNode> FirstDataNode(const shared_ptr<VNode> &Root);
shared_ptr<VDataNode> LeftDataNode(const shared_ptr<VDataNode> &Current);
shared_ptr<VDataNode> RightDataNode(const shared_ptr<VDataNode> &Current);
shared_ptr<VEdgeNode> EdgeToRightDataNode(const shared_ptr<VDataNode> &Current);
shared_ptr<VDataNode> FindDataNode(const shared_ptr<VNode> &Root, double ys, double x);
shared_ptr<VNode> ProcessDataEvent(const shared_ptr<VDataEvent> &e, shared_ptr<VNode> &Root, const shared_ptr<VoronoiGraph> &VG, double ys, vector<shared_ptr<VDataNode>> &CircleCheckList);
shared_ptr<VNode> ProcessCircleEvent(const shared_ptr<VCircleEvent> &e, const shared_ptr<VNode> &Root, const shared_ptr<VoronoiGraph> &VG, double ys, vector<shared_ptr<VDataNode>>& CircleCheckList);
shared_ptr<VCircleEvent> CircleCheckDataNode(const shared_ptr<VDataNode> &n, double ys);
void CleanUpTree(shared_ptr<VNode> Root);
VoronoiGraph ComputeVoronoiGraph(vector<Point> Datapoints);

bool Unique_Vertices(shared_ptr<Point> first, shared_ptr<Point> second)
{
	return first == second;
}

bool Unique_Edges(shared_ptr<VoronoiEdge> first, shared_ptr<VoronoiEdge> second)
{
	return first == second;
}

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
	priority_queue<shared_ptr<VEvent>, vector<shared_ptr<VEvent>>, Compare> PQ;
	map<VDataNode, VCircleEvent> CurrentCircles;
	shared_ptr<VoronoiGraph> VG = make_shared<VoronoiGraph>();
	shared_ptr<VNode> RootNode = nullptr;

	for (vector<Point>::iterator i = Datapoints.begin(); i != Datapoints.end(); i++)
	{
		VDataEvent vd(make_shared<Point>(*i));
		shared_ptr<VEvent> vev = make_shared<VDataEvent>(vd);
		PQ.push(vev);
	}
	while (PQ.size() > 0)
	{
		shared_ptr<VEvent> VE = PQ.top();
		PQ.pop();
		vector<shared_ptr<VDataNode>> CircleCheckList;

		if (dynamic_pointer_cast<VDataEvent>(VE) != nullptr)
		{
			RootNode = ProcessDataEvent(static_pointer_cast<VDataEvent>(VE), RootNode, VG, VE->Y(), CircleCheckList);
		}
		else if (dynamic_pointer_cast<VCircleEvent>(VE) != nullptr)
		{
			shared_ptr<VCircleEvent> VE_temp = static_pointer_cast<VCircleEvent>(VE);
			for (map<VDataNode, VCircleEvent>::iterator iii = CurrentCircles.begin(); iii != CurrentCircles.end();iii++)
			{
				VDataNode temp_key = iii->first;
				if (temp_key.DataPoint == VE_temp->NodeN->DataPoint && temp_key.Left() == VE_temp->NodeN->Left() && temp_key.Right() == VE_temp->NodeN->Right() && temp_key.Parent() == VE_temp->NodeN->Parent())
				{
					CurrentCircles.erase(*VE_temp->NodeN);
					goto here;
				}
			}
	here:
			if (!VE_temp->Valid)
				continue;
			RootNode = ProcessCircleEvent(VE_temp, RootNode, VG, VE_temp->Y(), CircleCheckList);
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
			shared_ptr<VDataNode> VD = *i;

			map<VDataNode, VCircleEvent>::iterator it = CurrentCircles.find(*VD);
			if (it != CurrentCircles.end())
			{
				VDataNode temp_key = it->first;
				if (temp_key.Left() == VD->Left() && temp_key.Parent() == VD->Parent() && temp_key.Right() == VD->Right())
				{
					CurrentCircles[*VD].Valid = false;
					CurrentCircles.erase(*VD);
				}
			}
			shared_ptr<VCircleEvent> VCE = CircleCheckDataNode(VD, VE->Y());
			if (VCE != nullptr)
			{
				shared_ptr<VEvent> a = VCE;
				PQ.push(a);
				CurrentCircles[*VD] = *VCE;
			}
		}
		if (dynamic_pointer_cast<VDataEvent>(VE) != nullptr)
		{
			shared_ptr<Point> DP = static_pointer_cast<VDataEvent>(VE)->DataPoint;
			for (map<VDataNode, VCircleEvent>::iterator i = CurrentCircles.begin(); i != CurrentCircles.end(); i++)
			{
				VCircleEvent VCE = i->second;
				if (Distance(DP->data[0], DP->data[1], VCE.Center->data[0], VCE.Center->data[1]) < VCE.Y() - VCE.Center->data[1]
					&& abs(Distance(DP->data[0], DP->data[1], VCE.Center->data[0], VCE.Center->data[1]) - (VCE.Y() - VCE.Center->data[1])) > 1e-10)
					VCE.Valid = false;
			}
		}
	}
	CleanUpTree(RootNode);

	for (list<shared_ptr<VoronoiEdge>>::iterator i = VG->Edges.begin(); i != VG->Edges.end(); i++)
	{
		shared_ptr<VoronoiEdge> VE = *i;
		if (VE->Done)
			continue;
		if (isnan(VE->VVertexB->data[0]) && isnan(VE->VVertexB->data[1]))
		{
			VE->AddVertex(make_shared<Point>(numeric_limits<double>::infinity(), numeric_limits<double>::infinity()));
			if (abs(VE->LeftData->data[1] - VE->RightData->data[1]) < 1e-10 && VE->LeftData->data[0] < VE->RightData->data[0])
			{
				shared_ptr<Point> T = VE->LeftData;
				VE->LeftData = VE->RightData;
				VE->RightData = T;
			}
		}
	}

	vector<shared_ptr<VoronoiEdge>> MinuteEdges;
	for (list<shared_ptr<VoronoiEdge>>::iterator i = VG->Edges.begin(); i != VG->Edges.end(); i++)
	{
		shared_ptr<VoronoiEdge> VE = *i;

		if (!VE->IsPartlyInfinite() && VE->VVertexA->Equals(VE->VVertexB->data))
		{
			MinuteEdges.push_back(VE);
			// prevent rounding errors from expanding to holes
			for (list<shared_ptr<VoronoiEdge>>::iterator i2 = VG->Edges.begin(); i2 != VG->Edges.end(); i2++)
			{
				shared_ptr<VoronoiEdge> VE2 = *i2;
				if (VE2->VVertexA->Equals(VE->VVertexA->data))
					VE2->VVertexA = VE->VVertexA;

				if (VE2->VVertexB->Equals(VE->VVertexA->data))
					VE2->VVertexB = VE->VVertexA;
			}
		}
	}

	for (vector<shared_ptr<VoronoiEdge>>::iterator k = MinuteEdges.begin(); k != MinuteEdges.end(); k++)
	{
		for (list<shared_ptr<VoronoiEdge>>::iterator oo = VG->Edges.begin(); oo != VG->Edges.end(); oo++)
			if ((*oo)->LeftData == (*k)->LeftData && (*oo)->RightData == (*k)->RightData && (*oo)->VVertexA == (*k)->VVertexA && (*oo)->VVertexB == (*k)->VVertexB)
			{
				VG->Edges.erase(oo);
				goto herre;
			}
		herre:;
	}
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

shared_ptr<VDataNode> FirstDataNode(const shared_ptr<VNode> &Root)
{
	shared_ptr<VNode> C = Root;
	while (C->Left() != nullptr)
	{
		C = C->Left();
	}
	return static_pointer_cast<VDataNode>(C);
}

shared_ptr<VDataNode> LeftDataNode(const shared_ptr<VDataNode>& Current)
{
	shared_ptr<VNode> C = Current;
	//1. Up
	do
	{
		if (C->Parent() == nullptr)
			return nullptr;
		if (C->Parent()->Left()== C)
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
	while (C->Right() != nullptr)
		C = C->Right();
	return static_pointer_cast<VDataNode>(C);
}

shared_ptr<VDataNode> RightDataNode(const shared_ptr<VDataNode>& Current)
{
	shared_ptr<VNode> C = Current;
	//1. Up
	do
	{
		if (C->Parent() == nullptr)
			return nullptr;
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
	while (C->Left() != nullptr)
		C = C->Left();
	return static_pointer_cast<VDataNode>(C);
}

shared_ptr<VEdgeNode> EdgeToRightDataNode(const shared_ptr<VDataNode>& Current)
{
	shared_ptr<VNode> C = Current;
	//1. Up
	do
	{
		if (C->Parent() == nullptr)
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

shared_ptr<VDataNode> FindDataNode(const shared_ptr<VNode> &Root, double ys, double x)
{
	shared_ptr<VNode> C = Root;
	do
	{
		if (dynamic_pointer_cast<VDataNode>(C) != nullptr)
			return static_pointer_cast<VDataNode>(C);

		shared_ptr<VEdgeNode> Cc = static_pointer_cast<VEdgeNode>(C);
		if (Cc->Cut(ys, x) < 0)
			C = C->Left();
		else
			C = C->Right();
	} while (true);
}

/// <summary>
/// Will return the new root (unchanged except in start-up)
/// </summary>
shared_ptr<VNode> ProcessDataEvent(const shared_ptr<VDataEvent> &e, shared_ptr<VNode> &Root, const shared_ptr<VoronoiGraph> &VG, double ys, vector<shared_ptr<VDataNode>> &CircleCheckList)
{
	if (Root == nullptr)
	{
		Root = static_pointer_cast<VDataNode>(make_shared<VDataNode>(e->DataPoint));
		CircleCheckList.empty();
		CircleCheckList.push_back(static_pointer_cast<VDataNode>(Root));
		return Root;
	}

	//1. Find the node to be replaced
	shared_ptr<VNode> C =  FindDataNode(Root, ys, e->DataPoint->data[0]);

	//2. Create the subtree (ONE Edge, but two VEdgeNodes)
	shared_ptr<VoronoiEdge> VE = make_shared<VoronoiEdge>();

	VE->LeftData = static_pointer_cast<VDataNode>(C)->DataPoint;
	VE->RightData = e->DataPoint;
	VE->VVertexA = make_shared<Point>(NAN, NAN);
	VE->VVertexB = make_shared<Point>(NAN, NAN);
	VG->Edges.push_back(VE);
	VG->Edges.unique(Unique_Edges);

	shared_ptr<VNode> SubRoot;
	if (abs(VE->LeftData->data[1] - VE->RightData->data[1]) < 1e-10)
	{
		if (VE->LeftData->data[0] < VE->RightData->data[0])
		{
			SubRoot = make_shared<VEdgeNode>(VE, false);
			SubRoot->Left(make_shared<VDataNode>(VE->LeftData));
			SubRoot->Right(make_shared<VDataNode>(VE->RightData));
		}
		else
		{
			SubRoot = make_shared<VEdgeNode>(VE, true);
			SubRoot->Left(make_shared<VDataNode>(VE->RightData));
			SubRoot->Right(make_shared<VDataNode>(VE->LeftData));
		}
		CircleCheckList.push_back(static_pointer_cast<VDataNode>(SubRoot->Left()));
		CircleCheckList.push_back(static_pointer_cast<VDataNode>(SubRoot->Right()));
	}
	else
	{
		SubRoot = make_shared<VEdgeNode>(VE, false);
		SubRoot->Left(make_shared<VDataNode>(VE->LeftData));
		SubRoot->Right(make_shared<VEdgeNode>(VE, true));
		SubRoot->Right()->Left(make_shared<VDataNode>(VE->RightData));
		SubRoot->Right()->Right(make_shared<VDataNode>(VE->LeftData));
		CircleCheckList.empty();
		CircleCheckList.push_back(static_pointer_cast<VDataNode>(SubRoot->Left()));
		CircleCheckList.push_back(static_pointer_cast<VDataNode>(SubRoot->Right()->Left()));
		CircleCheckList.push_back(static_pointer_cast<VDataNode>(SubRoot->Right()->Right()));
	}

	//3. Apply subtree
	if (C->Parent() == nullptr)
		return SubRoot;
	C->Parent()->Replace(C, SubRoot);
	return Root;
}

shared_ptr<VNode> ProcessCircleEvent(const shared_ptr<VCircleEvent> &e, const  shared_ptr<VNode> &Root, const shared_ptr<VoronoiGraph> &VG, double ys, vector<shared_ptr<VDataNode>>& CircleCheckList)
{
	shared_ptr<VDataNode> a;
	shared_ptr<VDataNode> b;
	shared_ptr<VDataNode> c;

	shared_ptr<VEdgeNode> eu;
	shared_ptr<VEdgeNode> eo;

	b = e->NodeN;
	a = LeftDataNode(b);
	c = RightDataNode(b);
	if (a == nullptr || b->Parent() == nullptr || c == nullptr || !a->DataPoint->Equals(e->NodeL->DataPoint->data) || !c->DataPoint->Equals(e->NodeR->DataPoint->data))
	{
		CircleCheckList.clear();
		return Root;
	}
	eu = static_pointer_cast<VEdgeNode>(b->Parent());
	CircleCheckList.clear();
	CircleCheckList.push_back(a);
	CircleCheckList.push_back(c);

	//1. Create the new Vertex
	shared_ptr<Point> VNew = make_shared<Point>(e->Center->data[0], e->Center->data[1]);
	VG->Vertices.push_back(VNew);
	VG->Vertices.unique(Unique_Vertices);

	//2. Find out if a or c are in a distand part of the tree (the other is then b's sibling) and assign the new vertex
	if (eu->Left()== b) // c is sibling
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
	eu->Edge->AddVertex(VNew);
	eo->Edge->AddVertex(VNew);

	//2. Replace eo by new Edge
	shared_ptr<VoronoiEdge> VE = make_shared<VoronoiEdge>(); // = new VoronoiEdge();
	VE->LeftData = a->DataPoint;
	VE->RightData = c->DataPoint;
	VE->AddVertex(VNew);
	VG->Edges.push_back(VE);
	VG->Edges.unique(Unique_Edges);

	shared_ptr<VEdgeNode> VEN = make_shared<VEdgeNode>(VE, false);
	VEN->Left(eo->Left());
	VEN->Right(eo->Right());
	if (eo->Parent() == nullptr)
		return VEN;

	eo->Parent()->Replace(eo, VEN);
	return Root;
}

shared_ptr<VCircleEvent> CircleCheckDataNode(const shared_ptr<VDataNode> &n, double ys)
{
	shared_ptr<VDataNode> l = LeftDataNode(n);
	shared_ptr<VDataNode> r = RightDataNode(n);

	if (l == nullptr || r == nullptr || l->DataPoint == r->DataPoint || l->DataPoint == n->DataPoint || n->DataPoint == r->DataPoint)
		return nullptr;

	if (ccw(l->DataPoint->data[0], l->DataPoint->data[1], n->DataPoint->data[0], n->DataPoint->data[1], r->DataPoint->data[0], r->DataPoint->data[1], false) <= 0)
		return nullptr;

	shared_ptr<Point> Center = make_shared<Point>(CircumCircleCenter(*l->DataPoint, *n->DataPoint, *r->DataPoint));
	shared_ptr<VCircleEvent> VC = make_shared<VCircleEvent>();
	VC->NodeN = n;
	VC->NodeL = l;
	VC->NodeR = r;
	VC->Center = Center;
	VC->Valid = true;
	if (VC->Y() > ys || abs(VC->Y() - ys) < 1e-10)
		return VC;
	return nullptr;
}

void CleanUpTree(shared_ptr<VNode> Root)
{
	if (dynamic_pointer_cast<VDataNode>(Root) != nullptr)
		return;

	shared_ptr<VEdgeNode> VE = static_pointer_cast<VEdgeNode>(Root);
	while (isnan(VE->Edge->VVertexB->data[0]) && isnan(VE->Edge->VVertexB->data[1]))
		VE->Edge->AddVertex(make_shared<Point>(numeric_limits<double>::infinity(), numeric_limits<double>::infinity()));

	if (VE->Flipped)
	{
		shared_ptr<Point> T = VE->Edge->LeftData;
		VE->Edge->LeftData = VE->Edge->RightData;
		VE->Edge->RightData = T;
	}
	VE->Edge->Done = true;
	CleanUpTree(Root->Left());
	CleanUpTree(Root->Right());
}
#endif