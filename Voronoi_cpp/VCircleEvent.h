#pragma once

#include<cmath>
#include"VEvent.h"
#include"VDataNode.h"

#ifndef VCIRCLEEVENT_H
#define VCIRCLEEVENT_H

class VCircleEvent : public VEvent
{
public:
	VDataNode NodeN, NodeL, NodeR;
	Point Center;
	VCircleEvent operator=(const VCircleEvent& other)
	{
		NodeN = other.NodeN;
		NodeL = other.NodeL;
		NodeR = other.NodeL;
		Center = other.Center;
		Valid = other.Valid;
		return *this;
	}

	VCircleEvent()
	{
		Point p(NAN, NAN);
		Center = p;
	}

	double Y()
	{
		return sqrt((Center.data[0] - NodeN.DataPoint.data[0]) * (Center.data[0] - NodeN.DataPoint.data[0]) +
			(Center.data[1] - NodeN.DataPoint.data[1]) * (Center.data[1] - NodeN.DataPoint.data[1]));
	}

	double X() { return Center.data[0]; }
	bool Valid = true;
};

#endif