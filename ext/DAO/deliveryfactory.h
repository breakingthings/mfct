#ifndef _DELIVERYFACTORY_H
#define _DELIVERYFACTORY_H
#include "trip.h"
#include "delivery.h"
#include "customer.h"
#include "session.h"

using namespace std;
using namespace BLL;

namespace DLL
{

class DeliveryFactory
{
public:
	DeliveryFactory(void);
	~DeliveryFactory(void);
	static shared_ptr<Delivery> AddDelivery(shared_ptr<Trip> trip, shared_ptr<Customer> customer,const CString &name, 
	const CString &unit, int quantity, double price);
	static shared_ptr<vector<Delivery> > GetDeliveries(int trip_id, int customer_id);
	static shared_ptr<Delivery> GetById(int id);
	static void UpdateDeliveryCount(int delivery_id, int new_quantity);
	static void DeleteAll();
private:
	DeliveryFactory &operator=(DeliveryFactory const &);
};
}
#endif