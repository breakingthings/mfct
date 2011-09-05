#include "Param.h"
#include <memory>

Param::Param(const Param &param)
{
	*this = param;
}

Param::Param(int value)
{
	this->m_value = new int(1);
	this->m_instance_count = new int(1);
	*((int*)m_value) = value;
	this->m_type = TYPE_INT;
	
}

Param::Param(double value)
{
	this->m_value = new double(1);
	this->m_instance_count = new int(1);
	*((double*)m_value) = value;
	this->m_type = TYPE_DOUBLE;
}

Param::Param(float value)
{
	this->m_value = new float(1);
	this->m_instance_count = new int(1);
	*((float*)m_value) = value;
	this->m_type = TYPE_FLOAT;
}

Param::Param(const TCHAR *value)
{
	size_t size = _tcslen(value) * sizeof(TCHAR) + sizeof(TCHAR);
	this->m_value = new TCHAR[size];
	memset(this->m_value, _T('\0'), size);
	memcpy(this->m_value, value, size);

	this->m_instance_count = new int(1);
	this->m_type = TYPE_STRING;
}

Param::~Param()
{
	--*m_instance_count;
	if(*m_instance_count == 0)
	{
		if(m_type == TYPE_STRING)
		{
			delete[] this->m_value;
		} else {
			delete this->m_value;
		}
		delete this->m_instance_count;
	} else {
		this->m_value = NULL;
		this->m_instance_count = NULL;
	}
	
}

Param::ParamType Param::GetType() const
{
	return this->m_type;
}

const void *Param::GetValue() const
{
	return this->m_value;
}

Param &Param::operator=(const Param &T)
{
	 if (this != &T) {
		 this->m_value = T.m_value;
		 this->m_type = T.m_type;
		 this->m_instance_count = T.m_instance_count;
		 *this->m_instance_count += 1;
    }
    return *this;
}