#include "varinfo.h"

VarInfo::VarInfo(void)
{
	type = NOTSET;
	dataPtr = NULL;
}

VarInfo::VarInfo(void *data, vartype dtype)
{
		type = dtype;
		dataPtr = data;
}

VarInfo::VarInfo(const VarInfo &other)
{
		*this = other;
}

VarInfo &VarInfo::operator=(const VarInfo &other)
{
	if(this != &other)
	{
		this->type = other.type;
		this->dataPtr = other.dataPtr;
	}
	return *this;
}