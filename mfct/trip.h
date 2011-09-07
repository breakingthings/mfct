#ifndef _TRIP_H
#define _TRIP_H
#include "objectbase.h"

using namespace std;

class Trip : public ObjectBase
{
public:
	IS_OBJECT
	Trip(const Trip&);
	Trip(void);
	~Trip(void);
	Trip &operator =(const Trip&);
	int id;
	int deleted;
};

#endif