#include<memory>
#pragma once

#ifndef VEVENT_H
#define VEVENT_H

class VEvent : public enable_shared_from_this<VEvent>
{
public:
	virtual double Y() const = 0;
	virtual const double &X() const = 0;

	int CompareTo(shared_ptr<VEvent> obj)
	{
		if (Y() > obj->Y())
			return 1;
		else if (Y() < obj->Y())
			return -1;

		if (X() > obj->X())
			return 1;
		else if (X() == obj->X())
			return 0;
		else
			return -1;
	}
	virtual ~VEvent() {  }
};

#endif