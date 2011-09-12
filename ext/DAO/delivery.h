#ifndef _DELIVERY_H
#define _DELIVERY_H
#include "objectbase.h"
#include <cstring>

namespace BLL
{
	class Delivery : public ObjectBase
	{
	public:
		Delivery(const Delivery&);
		Delivery(void);
		~Delivery(void);
		Delivery &operator =(const Delivery&);
		int id;
		CString name;
		CString unit;
		int customer_id;
		int trip_id;
		int quantity;
		double price;
		double total;

	};

}

#endif