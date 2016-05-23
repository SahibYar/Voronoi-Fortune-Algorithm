#pragma once

#include"Point.h"
#include"VNode.h"

#ifndef VDATANODE_H
#define VDATANODE_H

class VDataNode : public VNode
{
public:
	shared_ptr<Point> DataPoint;
	VDataNode(const shared_ptr<Point> &dp) { DataPoint = dp; }
	
	VDataNode()
	{
		DataPoint = make_shared<Point>(NAN, NAN);
	}

	VDataNode operator=(const VDataNode& other)
	{
		DataPoint = other.DataPoint;
		return *this;
	}

	bool operator<(const VDataNode& other) const
	{
		if (DataPoint->data[0] == other.DataPoint->data[0])
			return DataPoint->data[1] < other.DataPoint->data[1];
		return DataPoint->data[0] < other.DataPoint->data[0];
	}
};

#endif