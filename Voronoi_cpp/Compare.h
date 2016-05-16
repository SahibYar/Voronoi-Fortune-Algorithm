#include"VEvent.h"
#include<memory>
using namespace std;

class Compare
{
public:
	bool operator() (shared_ptr<VEvent> a, shared_ptr<VEvent> b)
	{
		if (a->YY == b->YY)
			return a->XX < b->XX;
		return a->YY > b->YY;

	}
};