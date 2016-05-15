#pragma once

#ifndef VEVENT_H
#define VEVENT_H

class VEvent
{
public:
	double Y;
	double X;

	int CompareTo(VEvent obj)
	{
		if (Y > obj.Y)
			return 1;
		else if (Y < obj.Y)
			return -1;

		if (X > obj.X)
			return 1;
		else if (X == obj.X)
			return 0;
		else
			return -1;
	}
	virtual ~VEvent() {  }
};

#endif