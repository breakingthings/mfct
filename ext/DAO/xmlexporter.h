#ifndef _XMLEXPORTER_H
#define _XMLEXPORTER_H
#include <fstream>

#include "trip.h"

using namespace std;
using namespace BLL;


class XMLExporter
{

public:
	XMLExporter(const CString &file_name);
	void SetFile(const CString &file_name);
	~XMLExporter(void);
	/**
	* Exports trip 
	*/
	void ExportTrip(const Trip &trip);
	/**
	* Write document end
	*/
	void Finish();
private:
	ofstream m_stream;
	bool m_has_start;
	bool m_finished;
	/**
	* Writes xml element <name>value</name>
	*/
	void WriteElement(const TCHAR *name, const TCHAR *value);
	/**
	* Overloaded method for int
	*/
	void WriteElement(const TCHAR *name, const int value);
	/**
	* Overloaded method for double
	*/
	void WriteElement(const TCHAR *name, const double value);
	/**
	* Writes document header and start element
	*/
	void WriteStart();
	/**
	* Writes XML string
	*/
	void WriteXmlString(const TCHAR *string, UINT codepage = CP_UTF8);

};

#endif