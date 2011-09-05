#ifndef _XMLIMPORTER_H
#define _XMLIMPORTER_H
#include <afx.h>
#include "resource.h"

class XMLImporter
{
public:
	~XMLImporter(void);
	static void LoadData(const CString&);

private:
	bool m_hasError;
	XMLImporter &operator =(const XMLImporter&);
	XMLImporter(const XMLImporter&);
	XMLImporter(void);
	XMLImporter(const CString&);

	CString m_xml_file;
	const void *GetXMLSchema(DWORD*);
	bool ParseFile();
	//HMODULE GetCurrentModule(void);
};

#endif