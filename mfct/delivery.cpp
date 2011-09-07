#include "stdafx.h"
#include "delivery.h"

Delivery::Delivery(const Delivery &delivery)
{
	*this = delivery;
}

Delivery::Delivery(void)
{
	MAPTABLE("trip");
	MAPCOLUMN("id", &id, VarInfo::INT);
	MAPCOLUMN("customer_id", &customer_id, VarInfo::INT);
	MAPCOLUMN("trip_id", &trip_id, VarInfo::INT);
	MAPCOLUMN("unit", &unit, VarInfo::STRING);
	MAPCOLUMN("name", &name, VarInfo::STRING);
	MAPCOLUMN("quantity", &quantity, VarInfo::INT);
	MAPCOLUMN("price", &price, VarInfo::DOUBLE);
	MAPCOLUMN("total", &total, VarInfo::DOUBLE);
}


Delivery::~Delivery(void)
{
}

Delivery &Delivery::operator =(const Delivery &other)
{
	ObjectBase::operator=(other);
	if(this != &other)
	{
		this->name = other.name;
		this->unit = other.unit;
		this->quantity = other.quantity;
		this->price = other.price;
		this->total = other.total;
		this->customer_id = other.customer_id;
		this->id = other.id;
		this->trip_id = other.trip_id;
	}

	return *this;
}
