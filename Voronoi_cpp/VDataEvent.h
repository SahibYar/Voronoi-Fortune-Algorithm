#pragma once

#include"Point.h"
#include"VEvent.h"

#ifndef VDATAEVENT_H
#define VDATAEVENT_H

class VDataEvent : public VEvent
{
public:
	Point DataPoint;
	VDataEvent(Point DP) { DataPoint = DP; }
	double Y() { return DataPoint.data[1]; }
	double X() { return DataPoint.data[0]; }
};

#endif