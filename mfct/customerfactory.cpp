#include "customerfactory.h"
#include "session.h"

CustomerFactory::CustomerFactory(void)
{
}


CustomerFactory::~CustomerFactory(void)
{
}

shared_ptr<vector<Customer> > CustomerFactory::GetByTripId(int trip_id)
{
	Session sess;
	sess << _T("SELECT c.id as id,c.name as name, c.address as address FROM customers c ")
			_T(" INNER JOIN deliveries d ON d.customer_id = c.id ")
			_T(" INNER JOIN trips t ON t.id = d.trip_id WHERE t.id = ? GROUP BY c.id, c.name, c.address") << Param(trip_id);
	
	return sess.ExecAndFetch<Customer>();
		
}


shared_ptr<Customer> CustomerFactory::AddOrGetCustomer(const CString &name, const CString &address)
{
	Session sess;
	sess << _T("SELECT * FROM customers WHERE name = ?") << Param(name);
	shared_ptr<Customer > customer = sess.Get<Customer>();
	if(!customer->IsNull())
	{
		return customer;
	} else {
		sess.ClearParams();
		sess << _T("INSERT INTO customers (name, address) VALUES (?, ?)") << Param(name) << Param(address);
		sess.Execute();
		
		sess.ClearParams();
		sess << _T("SELECT last_insert_rowid();");
		int id = sess.GetInt();
		sess.ClearParams();
		sess << _T("SELECT * FROM customers WHERE id = ?") << Param(id);
		return sess.Get<Customer>();
	}
}