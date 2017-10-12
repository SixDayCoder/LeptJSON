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

			v.type = LeptType::LEPT_NULL;
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
	const char* json = context.json;

	/*
	json数字的构成
	number = [ "-" ] int [ frac ] [ exp ]
	其中
	(0)负号可选
	(1)int = "0" / digit1-9 *digit
	(2)frac = "." 1*digit
	(3)exp = ("e" / "E") ["-" / "+"] 1*digit
	*/

	/*(0)校验负号*/
	if (*json == '-')
		json++;

	/*(1)校验int, 两种情况,*/
	if (*json == '0') {
		json++;
	}
	else {

		if (!ISDIGIT1TO9(*json)) return ParseState::LEPT_PARSE_INVALID_VALUE;

		for (json++; ISDIGIT(*json); json++);

	}

	/*(2)校验小数点*/
	if (*json == '.') {
		//小数点后至少一位是数字
		json++;

		if (!ISDIGIT(*json)) return ParseState::LEPT_PARSE_INVALID_VALUE;
			
		for (json++; ISDIGIT(*json); json++);

	}

	/*(3)校验指数部分*/
	if (*json == 'e' || *json == 'E') {
		json++;
		//跳过'e'或者'E'后,后面可以有正负号,检验
		/*(3.1)校验跳过E或者e后,是否有正负号*/
		if (*json == '+' || *json == '-') json++;

		/*(3.2)校验跳过了正负号后后面是否都为数字*/	
		for (json++; ISDIGIT(*json); json++);
	}

	//经过上述处理,json指向第一个不为数字的字符

	errno = 0;
	double number = strtold(context.json, NULL);

	if (errno == ERANGE && (number == HUGE_VAL || number == -HUGE_VAL))
		return ParseState::LEPT_PARSE_NUMBER_TOO_BIG;


	v.number = number;
	v.type = LeptType::LEPT_NUMBER;
	context.json = json;

	return ParseState::LEPT_PARSE_SUCCESS;


	/*
	errno : 
	errno is set to zero at program startup, and 
	any function of the standard C library can modify its value to some value different from zero, 
	generally to signal specific categories of error

	设定errno的初始值为0,c的标准库函数在运行时可修改它的值,一般是由一些宏来定义的

	ERANGE :
	表示一个范围错误，它在输入参数超出数学函数定义的范围时发生，errno 被设置为 ERANGE。

	Macro constant that expands to a positive expression of type double.

	HUGE_VAL:

	A function returns this value when the result of a mathematical operation yields a value 
	that is too large in magnitude to be representable with its return type

	Actually, functions may either return a positive or a negative HUGE_VAL (HUGE_VAL or -HUGE_VAL) 
	to indicate the sign of the result.


	*/
}
