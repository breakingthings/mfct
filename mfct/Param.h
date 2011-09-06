#ifndef _PARAM_H
#define _PARAM_H
#include <tchar.h>
#include <sqlite3.h>
/**
* Parameter class for adding SQL statement parameters
*/
class Param
{
public:
	/**
	* parameter base type
	*/
	enum ParamType { TYPE_INT, TYPE_DOUBLE, TYPE_TCHAR, TYPE_STRING };
	Param(const Param &param);
	Param(int value);
	Param(const TCHAR *value);
	Param(double value);
	/**
	* Get type of parameter
	* @return type of parameter
	*/
	ParamType GetType() const;
	/*
	* @return pointer to value of parameter
	*/
	const void *GetValue() const;
	Param &operator=(const Param &T);
	~Param(void);
private:
	ParamType m_type;
	void *m_value;
	int *m_instance_count;

};

#endif