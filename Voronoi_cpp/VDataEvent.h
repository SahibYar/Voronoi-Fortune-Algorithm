#pragma once

#include"Point.h"
#include"VEvent.h"

#ifndef VDATAEVENT_H
#define VDATAEVENT_H

class VDataEvent : public VEvent
{
public:
	Point DataPoint;
	VDataEvent(Point DP)
	{ 
		DataPoint = DP; 
		X = DP.data[0];
		Y = DP.data[1];
	}
	double Yy() { return DataPoint.data[1]; }
	double Xx() { return DataPoint.data[0]; }
};

#endif