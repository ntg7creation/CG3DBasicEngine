#include "ObjectsData.h"

myMoveable::myMoveable(int timeS, int timeE, Bezier1D* bezier,int objectindex)
{
	this->objectindex = objectindex;
	time_start = timeS;
	time_end = timeE;
	this->bezier = bezier;
};