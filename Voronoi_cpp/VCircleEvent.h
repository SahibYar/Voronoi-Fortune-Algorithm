#pragma once

#include<cmath>
#include<memory>
#include"VEvent.h"
#include"VDataNode.h"

#ifndef VCIRCLEEVENT_H
#define VCIRCLEEVENT_H

class VCircleEvent : public VEvent
{
public:
	shared_ptr<VDataNode> NodeN;
	shared_ptr<VDataNode> NodeL;
	shared_ptr<VDataNode> NodeR;
	shared_ptr<Point> Center;
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
		Center = make_shared<Point>(p);
	}

	virtual double Y() const override
	{
		return Center->data[1] + sqrt(pow((Center->data[0] - NodeN->DataPoint->data[0]), 2) + pow((Center->data[1] - NodeN->DataPoint->data[1]), 2));
	}

	virtual const double &X() const override 
	{ 
		return Center->data[0]; 
	}
	bool Valid = true;

protected:
	shared_ptr<VCircleEvent> shared_from_this()
	{
		return static_pointer_cast<VCircleEvent>(VEvent::shared_from_this());
	}

};

#endif