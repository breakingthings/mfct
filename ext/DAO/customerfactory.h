#ifndef _CUSTOMERFACTORY_H
#define _CUSTOMERFACTORY_H

#include "customer.h"

using namespace std;
using namespace BLL;

namespace DLL
{


class CustomerFactory
{
public:
	CustomerFactory(void);
	~CustomerFactory(void);
	static shared_ptr<Customer> AddOrGetCustomer(const CString &name,const CString &address);
	static shared_ptr<vector<Customer> > GetByTripId(int trip_id);
	static void DeleteAll();
private:
	CustomerFactory &operator=(CustomerFactory const &);
};

}

#endif