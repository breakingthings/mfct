#include "trip.h"



Trip::Trip(const Trip &trip)
{
	*this = trip;
}

Trip::Trip(void)
{
	MAPTABLE("trip");	
	MAPCOLUMN("id", &id, VarInfo::INT);
	MAPCOLUMN("deleted", &deleted, VarInfo::INT);

	
}


Trip::~Trip(void)
{
}

Trip &Trip::operator =(const Trip &other)
{
	ObjectBase::operator=(other);
	if(this != &other)
	{
		this->id = other.id;
		this->deleted = other.deleted;
	}

	return *this;
}

const VarInfoMap &Trip::GetMap() const
{
	return this->m_varmap;
}
