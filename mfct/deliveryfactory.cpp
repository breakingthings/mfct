#include "stdafx.h"
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

void DeliveryFactory::UpdateDeliveryCount(int delivery_id, int new_quantity)
{
	Session sess;
	sess << _T("SELECT * FROM deliveries WHERE id = ? ") << Param(delivery_id);
	shared_ptr<Delivery> d = sess.Get<Delivery>();

	if(d->quantity != new_quantity)
	{
		sess.ClearParams();
		double new_total = (double)new_quantity * d->price;
		sess << _T("UPDATE deliveries SET quantity = ?, total = ? WHERE id = ?") << Param(new_quantity) 
			<< Param(new_total) << Param(d->id);
		sess.Execute();
	}

}

shared_ptr<vector<Delivery> > DeliveryFactory::GetDeliveries(int trip_id, int customer_id)
{
	Session sess;
	sess << _T("SELECT * FROM deliveries WHERE trip_id = ? AND customer_id = ? ORDER BY id ASC") << Param(trip_id) 
		<< Param(customer_id);
	return sess.ExecAndFetch<Delivery>();
}


DeliveryFactory::DeliveryFactory(void)
{
}


DeliveryFactory::~DeliveryFactory(void)
{
}
