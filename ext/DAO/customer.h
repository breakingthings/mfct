#ifndef _CUSTOMER_H
#define _CUSTOMER_H
#include "objectbase.h"

namespace BLL
{
	class Customer : public ObjectBase
	{
	public:
		Customer(const Customer&);
		Customer(void);
		~Customer(void);
		Customer &operator =(const Customer&);
		int id;
		CString address;
		CString name;
	};
}

#endif