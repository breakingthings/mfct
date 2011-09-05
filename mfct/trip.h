#ifndef _TRIP_H
#define _TRIP_H
#include "objectbase.h"

using namespace std;

class Trip : public ObjectBase
{
public:
	Trip(const Trip&);
	Trip(void);
	~Trip(void);
	Trip &operator =(const Trip&);
	const VarInfoMap &GetMap() const;
	int id;
	int status;
	int deleted;
};

#endif