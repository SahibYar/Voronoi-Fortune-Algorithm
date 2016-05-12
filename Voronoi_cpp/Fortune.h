#include"Point.h"
#include<limits>
#include<cmath>
#include<typeinfo>
#include"FortuneVoronoi.h"

#ifndef FORTUNE_H
#define FORTUNE_H

class VDataNode;	// forward Declaration
class VEdgeNode;	// forward Declaration

class VNode
{
private:
	VNode* _Parent; VNode* _Left; VNode* _Right;
public:
	void Parent(VNode* p) { _Parent = p; }
	VNode* Parent() { return _Parent; }

	void Left(VNode* p) { _Left = p; p->Parent(this); }
	VNode* Left() { return _Left; }

	void Right(VNode* p) { _Right = p; p->Parent(this); }
	VNode* Right() { return _Right; }

	void Replace(VNode& ChildOld, VNode& ChildNew)
	{
		if (_Left == &ChildOld)
			_Left = &ChildNew;
		else if (_Right == &ChildOld)
			_Right = &ChildNew;
		else throw new exception("Child not found!");
		ChildOld.Parent(NULL);
	}

	static VDataNode* FirstDataNode(VNode& Root)
	{
		VNode* C = &Root;
		while (C->Left() != NULL)
			C = C->Left();
		return static_cast<VDataNode*>(C);
	}

	static VDataNode* LeftDataNode(VDataNode& Current)
	{
		VNode* C = &Current;
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
		while (C->Right != NULL)
			C = C->Right;
		return static_cast<VDataNode*>(C);
	}

	static VDataNode* RightDataNode(VDataNode& Current)
	{
		VNode* C = &Current;
		//1. Up
		do
		{
			if (C->Parent() == NULL)
				return NULL;
			if (C->Parent.Right() == C)
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
		while (C->Left != NULL)
			C = C->Left;
		return static_cast<VDataNode*>(C);
	}

	static VEdgeNode* EdgeToRightDataNode(VDataNode& Current)
	{
		VNode* C = &Current;
		//1. Up
		do
		{
			if (C->Parent() == NULL)
				throw new exception("No Left Leaf found!");
			if (C->Parent.Right() == C)
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
		return static_cast<VEdgeNode*>(C);
	}

	static VDataNode* FindDataNode(VNode& Root, double ys, double x)
	{
		VNode* C = &Root;
		do
		{
			if (typeid(C) == typeid(VDataNode))
				return static_cast<VDataNode*>(C);
			if (((VEdgeNode)C).Cut(ys, x) < 0)
				C = C->Left();
			else
				C = C->Right();
		} while (true);
	}

	/// <summary>
	/// Will return the new root (unchanged except in start-up)
	/// </summary>
	static VNode ProcessDataEvent(VDataEvent e, VNode Root, VoronoiGraph VG, double ys, out VDataNode[] CircleCheckList)
	{
		if (Root == null)
		{
			Root = new VDataNode(e.DataPoint);
			CircleCheckList = new VDataNode[]{ (VDataNode)Root };
			return Root;
		}
		//1. Find the node to be replaced
		VNode C = VNode.FindDataNode(Root, ys, e.DataPoint[0]);
		//2. Create the subtree (ONE Edge, but two VEdgeNodes)
		VoronoiEdge VE = new VoronoiEdge();
		VE.LeftData = ((VDataNode)C).DataPoint;
		VE.RightData = e.DataPoint;
		VE.VVertexA = Fortune.VVUnkown;
		VE.VVertexB = Fortune.VVUnkown;
		VG.Edges.Add(VE);

		VNode SubRoot;
		if (Math.Abs(VE.LeftData[1] - VE.RightData[1]) < 1e-10)
		{
			if (VE.LeftData[0] < VE.RightData[0])
			{
				SubRoot = new VEdgeNode(VE, false);
				SubRoot.Left = new VDataNode(VE.LeftData);
				SubRoot.Right = new VDataNode(VE.RightData);
			}
			else
			{
				SubRoot = new VEdgeNode(VE, true);
				SubRoot.Left = new VDataNode(VE.RightData);
				SubRoot.Right = new VDataNode(VE.LeftData);
			}
			CircleCheckList = new VDataNode[]{ (VDataNode)SubRoot.Left, (VDataNode)SubRoot.Right };
		}
		else
		{
			SubRoot = new VEdgeNode(VE, false);
			SubRoot.Left = new VDataNode(VE.LeftData);
			SubRoot.Right = new VEdgeNode(VE, true);
			SubRoot.Right.Left = new VDataNode(VE.RightData);
			SubRoot.Right.Right = new VDataNode(VE.LeftData);
			CircleCheckList = new VDataNode[]{ (VDataNode)SubRoot.Left, (VDataNode)SubRoot.Right.Left, (VDataNode)SubRoot.Right.Right };
		}

		//3. Apply subtree
		if (C.Parent == null)
			return SubRoot;
		C.Parent.Replace(C, SubRoot);
		return Root;
	}
	public static VNode ProcessCircleEvent(VCircleEvent e, VNode Root, VoronoiGraph VG, double ys, out VDataNode[] CircleCheckList)
	{
		VDataNode a, b, c;
		VEdgeNode eu, eo;
		b = e.NodeN;
		a = VNode.LeftDataNode(b);
		c = VNode.RightDataNode(b);
		if (a == null || b.Parent == null || c == null || !a.DataPoint.Equals(e.NodeL.DataPoint) || !c.DataPoint.Equals(e.NodeR.DataPoint))
		{
			CircleCheckList = new VDataNode[]{};
			return Root; // Abbruch da sich der Graph verändert hat
		}
		eu = (VEdgeNode)b.Parent;
		CircleCheckList = new VDataNode[]{ a, c };
		//1. Create the new Vertex
		Vector VNew = new Vector(e.Center[0], e.Center[1]);
		//			VNew[0] = Fortune.ParabolicCut(a.DataPoint[0],a.DataPoint[1],c.DataPoint[0],c.DataPoint[1],ys);
		//			VNew[1] = (ys + a.DataPoint[1])/2 - 1/(2*(ys-a.DataPoint[1]))*(VNew[0]-a.DataPoint[0])*(VNew[0]-a.DataPoint[0]);
		VG.Vertizes.Add(VNew);
		//2. Find out if a or c are in a distand part of the tree (the other is then b's sibling) and assign the new vertex
		if (eu.Left == b) // c is sibling
		{
			eo = VNode.EdgeToRightDataNode(a);

			// replace eu by eu's Right
			eu.Parent.Replace(eu, eu.Right);
		}
		else // a is sibling
		{
			eo = VNode.EdgeToRightDataNode(b);

			// replace eu by eu's Left
			eu.Parent.Replace(eu, eu.Left);
		}
		eu.Edge.AddVertex(VNew);
		//			///////////////////// uncertain
		//			if(eo==eu)
		//				return Root;
		//			/////////////////////

		//complete & cleanup eo
		eo.Edge.AddVertex(VNew);
		//while(eo.Edge.VVertexB == Fortune.VVUnkown)
		//{
		//    eo.Flipped = !eo.Flipped;
		//    eo.Edge.AddVertex(Fortune.VVInfinite);
		//}
		//if(eo.Flipped)
		//{
		//    Vector T = eo.Edge.LeftData;
		//    eo.Edge.LeftData = eo.Edge.RightData;
		//    eo.Edge.RightData = T;
		//}


		//2. Replace eo by new Edge
		VoronoiEdge VE = new VoronoiEdge();
		VE.LeftData = a.DataPoint;
		VE.RightData = c.DataPoint;
		VE.AddVertex(VNew);
		VG.Edges.Add(VE);

		VEdgeNode VEN = new VEdgeNode(VE, false);
		VEN.Left = eo.Left;
		VEN.Right = eo.Right;
		if (eo.Parent == null)
			return VEN;
		eo.Parent.Replace(eo, VEN);
		return Root;
	}
	static VCircleEvent CircleCheckDataNode(VDataNode n, double ys)
	{
		VDataNode* l = VNode::LeftDataNode(n);
		VDataNode* r = VNode::RightDataNode(n);
		if (l == null || r == null || l.DataPoint == r.DataPoint || l.DataPoint == n.DataPoint || n.DataPoint == r.DataPoint)
			return null;
		if (MathTools.ccw(l.DataPoint[0], l.DataPoint[1], n.DataPoint[0], n.DataPoint[1], r.DataPoint[0], r.DataPoint[1], false) <= 0)
			return null;
		Vector Center = Fortune.CircumCircleCenter(l.DataPoint, n.DataPoint, r.DataPoint);
		VCircleEvent VC = new VCircleEvent();
		VC.NodeN = n;
		VC.NodeL = l;
		VC.NodeR = r;
		VC.Center = Center;
		VC.Valid = true;
		if (VC.Y > ys || Math.Abs(VC.Y - ys) < 1e-10)
			return VC;
		return null;
	}

	public static void CleanUpTree(VNode Root)
	{
		if (Root is VDataNode)
			return;
		VEdgeNode VE = Root as VEdgeNode;
		while (VE.Edge.VVertexB == Fortune.VVUnkown)
		{
			VE.Edge.AddVertex(Fortune.VVInfinite);
			//				VE.Flipped = !VE.Flipped;
		}
		if (VE.Flipped)
		{
			Vector T = VE.Edge.LeftData;
			VE.Edge.LeftData = VE.Edge.RightData;
			VE.Edge.RightData = T;
		}
		VE.Edge.Done = true;
		CleanUpTree(Root.Left);
		CleanUpTree(Root.Right);
	}

};

class VDataNode : public VNode
{
public:
	Point DataPoint;
	VDataNode(Point dp) { DataPoint = dp; }
};

class VEdgeNode : public VNode
{
public:
	VEdgeNode(VoronoiEdge E, bool Flipped)
	{
		Edge = E;
		Flipped = Flipped;
	}
	VoronoiEdge Edge;
	bool Flipped;
	double Cut(double ys, double x)
	{
		if (!Flipped)
			return x - Fortune::ParabolicCut(Edge.LeftData[0], Edge.LeftData[1], Edge.RightData[0], Edge.RightData[1], ys);
		return x - Fortune::ParabolicCut(Edge.RightData[0], Edge.RightData[1], Edge.LeftData[0], Edge.LeftData[1], ys);
	}
};

class VEvent
{
public:
	double Y;
	double X;
	int CompareTo(VEvent obj)
	{
		if (Y > obj.Y)
			return 1;
		else if (Y < obj.Y)
			return -1;
		
		if (X > obj.X)
			return 1;
		else if (X == obj.X)
			return 0;
		else
			return -1;
	}
};

class VDataEvent : public VEvent
{
public:
	Point DataPoint;
	VDataEvent(Point DP) { DataPoint = DP; }
	double Y() { return DataPoint.data[1]; }
	double X() { return DataPoint.data[0]; }
};

class VCircleEvent : public VEvent
{
public:
	VDataNode NodeN, NodeL, NodeR;
	Point Center;
	double Y() 
	{
		return sqrt((Center.data[0] - NodeN.DataPoint.data[0]) * (Center.data[0] - NodeN.DataPoint.data[0]) + 
			(Center.data[1] - NodeN.DataPoint.data[1]) * (Center.data[1] - NodeN.DataPoint.data[1]));
	}

	double X() { return Center.data[0];}
	bool Valid = true;
};

class Fortune
{
public:
	static Point& VVinfinite()
	{
		Point p(numeric_limits<double>::infinity(), numeric_limits<double>::infinity());
		return p;
	}

	static Point& VVUnknown()
	{
		Point p(NAN, NAN);
		return p;
	}
	
	static double ParabolicCut(double x1, double y1, double x2, double y2, double ys)
	{
		//			y1=-y1;
		//			y2=-y2;
		//			ys=-ys;
		//			
		if (abs(x1 - x2) < 1e-10 && abs(y1 - y2) < 1e-10)
		{
			//				if(y1>y2)
			//					return double.PositiveInfinity;
			//				if(y1<y2)
			//					return double.NegativeInfinity;
			//				return x;
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

	static Point CircumCircleCenter(Point A, Point B, Point C)
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

	static VoronoiGraph ComputeVoronoiGraph(vector<Point> Datapoints)
	{

		BinaryPriorityQueue PQ = new BinaryPriorityQueue();
		Hashtable CurrentCircles = new Hashtable();
		VoronoiGraph VG = new VoronoiGraph();
		VNode RootNode = null;
		foreach(Vector V in Datapoints)
		{
			PQ.Push(new VDataEvent(V));
		}
		while (PQ.Count > 0)
		{
			VEvent VE = PQ.Pop() as VEvent;
			VDataNode[] CircleCheckList;
			if (VE is VDataEvent)
			{
				RootNode = VNode.ProcessDataEvent(VE as VDataEvent, RootNode, VG, VE.Y, out CircleCheckList);
			}
			else if (VE is VCircleEvent)
			{
				CurrentCircles.Remove(((VCircleEvent)VE).NodeN);
				if (!((VCircleEvent)VE).Valid)
					continue;
				RootNode = VNode.ProcessCircleEvent(VE as VCircleEvent, RootNode, VG, VE.Y, out CircleCheckList);
			}
			else throw new Exception("Got event of type " + VE.GetType().ToString() + "!");
			foreach(VDataNode VD in CircleCheckList)
			{
				if (CurrentCircles.ContainsKey(VD))
				{
					((VCircleEvent)CurrentCircles[VD]).Valid = false;
					CurrentCircles.Remove(VD);
				}
				VCircleEvent VCE = VNode.CircleCheckDataNode(VD, VE.Y);
				if (VCE != null)
				{
					PQ.Push(VCE);
					CurrentCircles[VD] = VCE;
				}
			}
			if (VE is VDataEvent)
			{
				Vector DP = ((VDataEvent)VE).DataPoint;
				foreach(VCircleEvent VCE in CurrentCircles.Values)
				{
					if (MathTools.Dist(DP[0], DP[1], VCE.Center[0], VCE.Center[1]) < VCE.Y - VCE.Center[1] && Math.Abs(MathTools.Dist(DP[0], DP[1], VCE.Center[0], VCE.Center[1]) - (VCE.Y - VCE.Center[1])) > 1e-10)
						VCE.Valid = false;
				}
			}
		}
		VNode.CleanUpTree(RootNode);
		foreach(VoronoiEdge VE in VG.Edges)
		{
			if (VE.Done)
				continue;
			if (VE.VVertexB == Fortune.VVUnkown)
			{
				VE.AddVertex(Fortune.VVInfinite);
				if (Math.Abs(VE.LeftData[1] - VE.RightData[1]) < 1e-10 && VE.LeftData[0] < VE.RightData[0])
				{
					Vector T = VE.LeftData;
					VE.LeftData = VE.RightData;
					VE.RightData = T;
				}
			}
		}

		ArrayList MinuteEdges = new ArrayList();
		foreach(VoronoiEdge VE in VG.Edges)
		{
			if (!VE.IsPartlyInfinite && VE.VVertexA.Equals(VE.VVertexB))
			{
				MinuteEdges.Add(VE);
				// prevent rounding errors from expanding to holes
				foreach(VoronoiEdge VE2 in VG.Edges)
				{
					if (VE2.VVertexA.Equals(VE.VVertexA))
						VE2.VVertexA = VE.VVertexA;
					if (VE2.VVertexB.Equals(VE.VVertexA))
						VE2.VVertexB = VE.VVertexA;
				}
			}
		}
		foreach(VoronoiEdge VE in MinuteEdges)
			VG.Edges.Remove(VE);

		return VG;
	}
};
#endif