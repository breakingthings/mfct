#include "stdafx.h"
#include "objectbase.h"

ObjectBase::ObjectBase(void)
{
	m_is_null = true;
	
}

ObjectBase::~ObjectBase(void)
{
}

const CString &ObjectBase::GetTableName() const
{
	return this->m_table_name;
}

bool ObjectBase::IsNull()
{
	return this->m_is_null;
}

ObjectBase &ObjectBase::operator =(const ObjectBase& other)
{
	if(this != &other)
	{
		this->m_is_null = other.m_is_null;
	}

	return *this;
}

const VarInfoMap &ObjectBase::GetMap() const
{
	return this->m_varmap;
}
