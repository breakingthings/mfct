// RunUnitTests.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <gtest/gtest.h>
#include <tripfactory.h>
#include <deliveryfactory.h>
#include <customerfactory.h>
#include <xmlexporter.h>
#include <xmlimporter.h>

using namespace std;
using namespace DLL;

TEST(XmlExporter, export_xml)
{
	//Get testrunner app path
	TCHAR  appPath[MAX_PATH] = _T("");
	CString xmlFile;
	GetModuleFileName(0, appPath, sizeof(appPath) - 1);

	xmlFile = appPath;
	xmlFile = xmlFile.Left(xmlFile.ReverseFind('\\'));
	xmlFile += _T("/mfct_test.xml");
	
	//Clear test DB jsut in case
	DeliveryFactory::DeleteAll();
	CustomerFactory::DeleteAll();
	TripFactory::DeleteAll();

	//Add test data
	shared_ptr<Trip> t = TripFactory::AddTrip();	
	shared_ptr<Customer> c = CustomerFactory::AddOrGetCustomer(_T("test"), _T("test"));
	shared_ptr<Delivery> d = DeliveryFactory::AddDelivery(t, c, _T("test"), _T("test"), 10, 10.5);

	//Export test data
	XMLExporter ex(xmlFile);
	ex.ExportTrip(*t.get());
	ex.Finish();
	
	//Clear test data
	DeliveryFactory::DeleteAll();
	CustomerFactory::DeleteAll();
	TripFactory::DeleteAll();

	//Import test data from xml file
	XMLImporter::LoadData(xmlFile);
	
	//run tests that everything is OK
	shared_ptr<vector<Trip> > trips = TripFactory::GetAllTrips();
	EXPECT_EQ(1, trips->size());
	
	shared_ptr<vector<Customer> > customers = CustomerFactory::GetByTripId(trips->at(0).id); 
	EXPECT_EQ(1, customers->size());
	EXPECT_EQ(_T("test"), customers->at(0).name);

	shared_ptr<vector<Delivery> > deliveries = DeliveryFactory::GetDeliveries(trips->at(0).id, customers->at(0).id);
	
	EXPECT_EQ(1, deliveries->size());
	EXPECT_EQ(_T("test"), deliveries->at(0).name);
	EXPECT_EQ(_T("test"), deliveries->at(0).unit);
	EXPECT_EQ(10, deliveries->at(0).quantity);
	
	EXPECT_DOUBLE_EQ(10.5, deliveries->at(0).price);
	EXPECT_DOUBLE_EQ(105.0, deliveries->at(0).total);
}

TEST(TripFactory, GetById)
{
	shared_ptr<Trip> t1 = TripFactory::AddTrip();
	shared_ptr<Trip> t2 = TripFactory::GetById(t1->id);
	EXPECT_EQ(t1->id, t2->id);
}

TEST(CustomerFactory, AddOrGetCustomer)
{
	shared_ptr<Customer> c1 = CustomerFactory::AddOrGetCustomer(_T("test"), _T("test"));
	shared_ptr<Customer> c2 = CustomerFactory::AddOrGetCustomer(_T("test"), _T("test"));
	EXPECT_EQ(c1->id, c2->id);
}

TEST(DeliveryFactory, AddDelivery)
{
	shared_ptr<Trip> t = TripFactory::AddTrip();	
	shared_ptr<Customer> c = CustomerFactory::AddOrGetCustomer(_T("test"), _T("test"));
	shared_ptr<Delivery> d = DeliveryFactory::AddDelivery(t, c, _T("test"), _T("test"), 10, 100.0);
	EXPECT_EQ(1000.0, d->total);
}

TEST(DeliveryFactory, UpdateDeliveryCount)
{
	shared_ptr<Trip> t = TripFactory::AddTrip();	
	shared_ptr<Customer> c = CustomerFactory::AddOrGetCustomer(_T("test"), _T("test"));
	shared_ptr<Delivery> d = DeliveryFactory::AddDelivery(t, c, _T("test"), _T("test"), 10, 100.0);
	DLL::DeliveryFactory::UpdateDeliveryCount(d->id, 1);
	d = DeliveryFactory::GetById(d->id);
	EXPECT_EQ(100.0, d->total);
}

int _tmain(int argc, _TCHAR* argv[])
{
	Session::SetDB("test.db");
	Session::InitDB();
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

