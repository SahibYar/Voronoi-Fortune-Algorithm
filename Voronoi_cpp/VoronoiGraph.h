#pragma once

#include<list>
using namespace std;

#include"Point.h"
#include"VoronoiEdge.h"

#ifndef VORONOIGRAPH_H
#define VORONOIGRAPH_H
class VoronoiGraph
{
public:
	list<Point> Vertices;
	list<VoronoiEdge> Edges;

	VoronoiGraph()
	{
		Vertices.clear();
		Edges.clear();
	}
};

#endif