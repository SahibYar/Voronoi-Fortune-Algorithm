#pragma once

#ifndef VEVENT_H
#define VEVENT_H

class VEvent
{
public:
	double XX;
	double YY;

	int CompareTo(VEvent obj)
	{
		if (YY > obj.YY)
			return 1;
		else if (YY < obj.YY)
			return -1;

		if (XX > obj.XX)
			return 1;
		else if (XX == obj.XX)
			return 0;
		else
			return -1;
	}
	virtual ~VEvent() {  }
};

#endif