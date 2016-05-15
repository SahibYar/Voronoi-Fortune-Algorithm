#pragma once

#include"Point.h"
#include"VNode.h"

#ifndef VDATANODE_H
#define VDATANODE_H

class VDataNode : public VNode
{
public:
	Point DataPoint;
	VDataNode(Point dp) { DataPoint = dp; }
	
	VDataNode()
	{
		Point p(NAN, NAN);
		DataPoint = p;
	}
	bool operator<(const VDataNode& other) const
	{
		if (DataPoint.data[0] == other.DataPoint.data[0])
			return DataPoint.data[1] < other.DataPoint.data[1];
		return DataPoint.data[0] < other.DataPoint.data[0];
	}
};

#endif