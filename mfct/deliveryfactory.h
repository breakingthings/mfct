#ifndef _DELIVERYFACTORY_H
#define _DELIVERYFACTORY_H
#include "trip.h"
#include "delivery.h"
#include "customer.h"
#include "session.h"

class DeliveryFactory
{
public:
	DeliveryFactory(void);
	~DeliveryFactory(void);
	static void AddDelivery(const Trip &trip, const Customer &customer, const Delivery &delivery);

	
};

#endif