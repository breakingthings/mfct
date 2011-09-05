#ifndef _DELIVERYFACTORY_H
#define _DELIVERYFACTORY_H
#include "trip.h"
#include "delivery.h"
#include "customer.h"
#include "session.h"

using namespace std;

class DeliveryFactory
{
public:
	DeliveryFactory(void);
	~DeliveryFactory(void);
	static void AddDelivery(shared_ptr<Trip> trip, shared_ptr<Customer> customer,const CString &name, 
	const CString &unit, int quantity, double price, double total);
	static shared_ptr<vector<Delivery> > GetDeliveries(int trip_id);
private:
	DeliveryFactory &operator=(DeliveryFactory const &);
};

#endif