#include "leptjson.h"

double LeptGetNumber(const LeptValue & v)
{
	assert(v.type == LeptType::LEPT_NUMBER);
	return v.number;
}

ParseState LeptParse(const char* json, LeptValue& v)
{
	assert(json != nullptr);

	v.type = LeptType::LEPT_NULL;
	LeptContext context;
	context.json = json;

	//去掉json字符串开始的空白符
	LeptParseWhitespace(context);
	
	ParseState ret;
	ret = LeptParseValue(context, v);

	if ( ret == ParseState::LEPT_PARSE_SUCCESS) {
		//去掉解析值后的空格
		LeptParseWhitespace(context);
		//如果去掉空格后字符串没有结束
		if (*context.json != '\0') {

			v.type = LeptType::LEPT_NOT_SINGULAR;

			return ParseState::LEPT_PARSE_ROOT_NOT_SINGULAR;
		}
		else
			return ParseState::LEPT_PARSE_SUCCESS;
	}

	return ret;
}

void LeptParseWhitespace(LeptContext & context)
{
	const char* p = context.json;

	while (*p == ' ' || *p == '\t' || *p == '\r' || *p == '\n')
		p++;
	
	context.json = p;
}

ParseState LeptParseValue(LeptContext& context, LeptValue& v)
{
	switch ( *context.json ){

		case 'n' :  return LeptParseNull(context, v);  

		case 't' :  return LeptParseTrue(context, v);  

		case 'f' :  return LeptParseFalse(context, v); 

		default:    return LeptParseNumber(context, v);

		case '\0':  return ParseState::LEPT_PARSE_EXPECT_VALUE; 

	}

}

ParseState LeptParseNull(LeptContext& context, LeptValue& v)
{
	assert(*context.json == 'n');

	context.json++;

	if (context.json[0] != 'u' || context.json[1] != 'l' || context.json[2] != 'l') {

		return ParseState::LEPT_PARSE_INVALID_VALUE;

	}
	else {

		context.json += 3;
		v.type = LeptType::LEPT_NULL;

		return ParseState::LEPT_PARSE_SUCCESS;
	}
	
}

ParseState LeptParseTrue(LeptContext & context, LeptValue & v)
{
	assert(*context.json == 't');

	context.json++;

	if (context.json[0] != 'r' || context.json[1] != 'u' || context.json[2] != 'e') {

		return ParseState::LEPT_PARSE_INVALID_VALUE;

	}
	else {

		context.json += 3;
		v.type = LeptType::LEPT_TRUE;

		return ParseState::LEPT_PARSE_SUCCESS;

	}

}

ParseState LeptParseFalse(LeptContext & context, LeptValue & v)
{
	assert(*context.json == 'f');

	context.json++;

	if (context.json[0] != 'a' || context.json[1] != 'l' || context.json[2] != 's' || context.json[3] != 'e' ) {

		return ParseState::LEPT_PARSE_INVALID_VALUE;

	}
	else {

		context.json += 4;
		v.type = LeptType::LEPT_FALSE;

		return ParseState::LEPT_PARSE_SUCCESS;

	}
}

ParseState LeptParseNumber(LeptContext & context, LeptValue & v)
{
	char* end = NULL;

	double number = strtold(context.json, &end);

	//end -> 第一个不为数字的指针

	if (context.json == end) {
		return ParseState::LEPT_PARSE_INVALID_VALUE;
	}
	else {

		v.number = number;
		v.type = LeptType::LEPT_NUMBER;
		context.json = end;

		return ParseState::LEPT_PARSE_SUCCESS;
	}

}
