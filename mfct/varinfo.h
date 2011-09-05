#ifndef _VARINFO_H
#define _VARINFO_H
#include <afx.h>

class VarInfo
{
public:
	enum vartype
	{
		STRING,
		INT,
		INT64,
		DOUBLE,
		NOTSET
	};
	
	VarInfo(void);
	VarInfo(const VarInfo &other);
	VarInfo(void *data, vartype dtype);
	vartype type;
	void *dataPtr;
	VarInfo &operator =(const VarInfo &);
};

#endif