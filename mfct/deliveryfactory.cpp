#include "deliveryfactory.h"
#include "session.h"

void DeliveryFactory::AddDelivery(shared_ptr<Trip> trip, shared_ptr<Customer> customer,const CString &name, 
	const CString &unit, int quantity, double price, double total)
{
	ASSERT(total == (double)quantity * price);
	Session sess;
	sess << _T("INSERT INTO deliveries (trip_id, customer_id, name, unit, quantity, price, total) VALUES ")
		_T(" (?,?,?,?,?,?,?)") << Param(trip->id) << Param(customer->id) << Param(name) << Param(unit) 
		<< Param(quantity) << Param(price) << Param(total);
	sess.Execute();
	
}

shared_ptr<vector<Delivery> > DeliveryFactory::GetDeliveries(int trip_id)
{
	Session sess;
	sess << _T("SELECT * FROM deliveries WHERE trip_id = ?") << Param(trip_id);
	return sess.ExecAndFetch<Delivery>();
	
}

DeliveryFactory::DeliveryFactory(void)
{
}


DeliveryFactory::~DeliveryFactory(void)
{
}
