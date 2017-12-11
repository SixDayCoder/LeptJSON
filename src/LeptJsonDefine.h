#ifndef  LEPTJSONDEFINE_H_
#define  LEPTJSONDEFINE_H_

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
	LEPT_PARSE_ROOT_NOT_SINGULAR
};


struct LeptValue 
{
	LeptType type;
};

struct LeptContext
{
	const char* json;
};


#endif // !LEPTJSONDEFINE_H_
