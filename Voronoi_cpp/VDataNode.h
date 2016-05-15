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
};

#endif