#ifndef _OBJECTBASE_H
#define _OBJECTBASE_H

#include "varinfo.h"
#include <afxtempl.h>
#include <afxcoll.h>
#include <memory>
#include <cstring>
#include <unordered_map>

using namespace std;

typedef CMap<CString, const TCHAR*, VarInfo, VarInfo&> VarInfoMap;

#define MAPTABLE(a) this->m_table_name = _T(a);
#define MAPCOLUMN(a, b, c) this->m_varmap.SetAt(_T(a), VarInfo(b,c));


class ObjectBase
{

public:
	ObjectBase(void);
	virtual ~ObjectBase(void);
	//Gets the object map
	virtual const VarInfoMap &GetMap() const = 0;
	const CString &GetTableName() const;

protected:	
	//contains map of sql result field names to object variable pointers
	VarInfoMap  m_varmap;
	CString m_table_name;
};

#endif