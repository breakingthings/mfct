#include "objectbase.h"

ObjectBase::ObjectBase(void)
{
	
}


ObjectBase::~ObjectBase(void)
{
}

const CString &ObjectBase::GetTableName() const
{
	return this->m_table_name;
}