#ifndef _CUSTOMER_H
#define _CUSTOMER_H
#include "objectbase.h"

class Customer : public ObjectBase
{
public:
	IS_OBJECT
	Customer(const Customer&);
	Customer(void);
	~Customer(void);
	Customer &operator =(const Customer&);
	int id;
	CString address;
	CString name;
};

#endif