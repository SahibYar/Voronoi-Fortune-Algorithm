#pragma once

#include"Point.h"
#include"VEvent.h"

#ifndef VDATAEVENT_H
#define VDATAEVENT_H

class VDataEvent : public VEvent
{
public:
	shared_ptr<Point> DataPoint;

	VDataEvent(const shared_ptr<Point> &DP)
	{ 
		this->DataPoint = DP; 
	}

	virtual double Y() const override
	{
		return DataPoint->data[1];
	}

	virtual const double &X() const override
	{
		return DataPoint->data[0];
	}

protected:
	shared_ptr<VDataEvent> shared_from_this()
	{
		return static_pointer_cast<VDataEvent>(VEvent::shared_from_this());
	}
};

#endif