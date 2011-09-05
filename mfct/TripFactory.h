#ifndef _TRIPFACTORY_H
#define _TRIPFACTORY_H

#include "FactoryBase.h"
#include "session.h"
#include "trip.h"
using namespace std;

class TripFactory
{
public:
	TripFactory(void);
	~TripFactory(void);
	static std::shared_ptr<Trip> AddTrip();
	static shared_ptr<vector<Trip> > GetAllTrips();
};

#endif