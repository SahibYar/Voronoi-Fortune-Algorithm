#include"VEvent.h"
#include<memory>
using namespace std;

class Compare
{
public:
	bool operator() (shared_ptr<VEvent> a, shared_ptr<VEvent> b)
	{
		if (a->Y() == b->Y())
			return a->X() < b->X();
		return a->Y() > b->Y();

	}
};