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

