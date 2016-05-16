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
		XX = DP.data[0];
		YY = DP.data[1];
	}
};

#endif