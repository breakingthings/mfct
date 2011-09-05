#ifndef _PARAM_H
#define _PARAM_H
#include <tchar.h>
#include <sqlite3.h>

class Param
{
public:

	enum ParamType { TYPE_INT, TYPE_DOUBLE, TYPE_TCHAR, TYPE_STRING };
	Param(const Param &param);
	Param(int value);
	Param(const TCHAR *value);
	Param(double value);
	ParamType GetType() const;
	const void *GetValue() const;
	Param &operator=(const Param &T);
	~Param(void);
private:
	ParamType m_type;
	void *m_value;
	int *m_instance_count;

};

#endif