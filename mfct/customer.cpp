#include "customer.h"

Customer::Customer(const Customer &customer)
{
	*this = customer;
}

/**
* Customer business object
*/
Customer::Customer(void)
{
	MAPTABLE("trip");	
	MAPCOLUMN("id", &id, VarInfo::INT);
	MAPCOLUMN("name", &name, VarInfo::STRING);
	MAPCOLUMN("address", &address, VarInfo::STRING);
}


Customer::~Customer(void)
{
}

Customer &Customer::operator =(const Customer &other)
{
	ObjectBase::operator=(other);
	if(this != &other)
	{
		this->id = other.id;
		this->address = other.address;
		this->name = other.name;
	}

	return *this;
}

const VarInfoMap &Customer::GetMap() const
{
	return this->m_varmap;
}

