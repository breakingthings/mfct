#include "session.h"
#include "TripFactory.h"

using namespace BLL;

namespace DLL
{

	TripFactory::TripFactory(void)
	{
	}


	TripFactory::~TripFactory(void)
	{
	}


	shared_ptr<Trip> TripFactory::AddTrip()
	{	

		Session sess;	
		sess << _T("INSERT INTO trips (deleted) VALUES (?)") << Param(0);
		sess.Execute();
		sess.ClearParams();

		sess << _T("SELECT * FROM trips ORDER BY id DESC LIMIT 1");
		shared_ptr<Trip> t = sess.Get<Trip>();
		return t;
	}

	shared_ptr<vector<Trip> > TripFactory::GetAllTrips()
	{
		Session sess;
		sess << _T("SELECT * FROM trips ORDER BY id ASC");
		shared_ptr<vector<Trip> > trips = sess.ExecAndFetch<Trip>();
		return trips;
	}

	shared_ptr<Trip> TripFactory::GetById(int id)
	{
		Session sess;
		sess << _T("SELECT * FROM trips WHERE id = ?") << Param(id);
		shared_ptr<Trip> t = sess.Get<Trip>();
		return t;
	}

	void TripFactory::DeleteAll()
	{
		Session sess;
		sess << _T("DELETE FROM trips");
		sess.Execute();
	}

}