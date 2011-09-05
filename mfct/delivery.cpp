#include "delivery.h"

Delivery::Delivery(const Delivery &T)
{
	*this = T;
}

Delivery::Delivery(void)
{
	MAPTABLE("trip");
	MAPCOLUMN("id", &id, VarInfo::INT);
	/*MAPCOLUMN("customer_name", &customer_name, VarInfo::STRING);
	MAPCOLUMN("address", &address, VarInfo::INT);*/
	
}


Delivery::~Delivery(void)
{
}

Delivery &Delivery::operator =(const Delivery &other)
{
	if(this != &other)
	{
		this->name = other.name;
		this->unit = other.unit;
		this->quantity = other.quantity;
		this->price = other.price;
		this->total = other.total;
	}

	return *this;
}

const VarInfoMap &Delivery::GetMap() const
{
	return this->m_varmap;
}
