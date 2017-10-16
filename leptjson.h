#ifndef LEPTJSON_H__
#define LEPTJSON_H__

#include <stdlib.h> /* size_t */

typedef enum { LEPT_NULL, LEPT_FALSE, LEPT_TRUE, LEPT_NUMBER, LEPT_STRING, LEPT_ARRAY, LEPT_OBJECT } LeptType;

typedef struct {

	union {
		struct { char* s; size_t len; }s;  /* string: null-terminated string, string length */
		double n;                          /* number */
	}u;

	LeptType type;

}LeptValue;

enum {

	LeptParseSuccess = 0,
	LeptParse_EXPECT_VALUE,
	LeptParse_INVALID_VALUE,
	LeptParse_ROOT_NOT_SINGULAR,
	LeptParseNumber_TOO_BIG,
	LeptParse_MISS_QUOTATION_MARK,
	LeptParse_INVALID_STRING_ESCAPE,
	LeptParse_INVALID_STRING_CHAR

};


LeptType 
LeptGetType(const LeptValue* v);

void	 
LeptInit(LeptValue* v);

int		 
LeptParse(LeptValue* v, const char* json);

void	 
LeptFree(LeptValue* v);


int    
LeptGetBoolean(const LeptValue* v);

void	
LeptSetBoolean(LeptValue* v, int b);

double		
LeptGetNumber(const LeptValue* v);

void		
LeptSetNumber(LeptValue* v, double n);

const char* 
LeptGetString(const LeptValue* v);

size_t 
LeptGetStringLength(const LeptValue* v);

void 
LeptSetString(LeptValue* v, const char* s, size_t len);

#endif /* LEPTJSON_H__ */
