#pragma once

#include<unordered_set>
using namespace std;

#include"Point.h"
#include"VoronoiEdge.h"

#ifndef VORONOIGRAPH_H
#define VORONOIGRAPH_H
class VoronoiGraph
{
public:
	unordered_set<Point> Vertices;
	unordered_set<VoronoiEdge> Edges;
};

#endif