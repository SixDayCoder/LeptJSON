#ifndef  LEPTJSONDEFINE_H_
#define  LEPTJSONDEFINE_H_

#include <assert.h>

enum class LeptType
{
	LEPT_NULL,
	LEPT_FALSE,
	LEPT_TRUE,
	LEPT_NUMBRE,
	LEPT_STRING,
	LEPT_ARRAY,
	LEPT_OBJECT
};


enum class LeptParseRet
{
	LEPT_PASER_OK,
	LEPT_PARSE_EXPCET_VALUE,
	LEPT_PARSE_INVALID_VALUE,
	LEPT_PARSE_ROOT_NOT_SINGULAR,
	LEPT_PARSE_NUMBER_TOO_BIG
};


struct LeptValue 
{
	//当且仅当type是number时number才有效
	double number;
	LeptType type;

	double GetNumber()const {
		assert(type == LeptType::LEPT_NUMBRE);
		return number;
	}

};

struct LeptContext
{
	const char* json;
};


#endif // !LEPTJSONDEFINE_H_
