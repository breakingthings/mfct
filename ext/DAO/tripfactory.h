#ifndef _TRIPFACTORY_H
#define _TRIPFACTORY_H

#include "session.h"
#include "trip.h"

using namespace std;
using namespace BLL;

namespace DLL
{
	class TripFactory
	{
	public:
		TripFactory(void);
		~TripFactory(void);
		/**
		* Add new trip 
		* \return shared_ptr to the new Trip object
		*/
		static shared_ptr<Trip> AddTrip();
		/**
		* Get all trips
		* \return shared_ptr to <code>vector<Trip></code>
		*/
		static shared_ptr<vector<Trip> > GetAllTrips();
		/**
		* Get Trip object by id
		* \return shared_ptr to Trip object
		*/
		static shared_ptr<Trip> GetById(int id);
		/**
		* Delete all trips
		*/
		static void DeleteAll();

	private:
		TripFactory &operator=(TripFactory const &);
	};

}

#endif