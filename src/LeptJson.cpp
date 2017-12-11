#include "LeptJson.h"

#include <cerrno>
#include <cmath>
#include <cstdlib>

LeptJsonParser::LeptJsonParser(const LeptContext & context)
{
	m_LeptContext = context;
}


LeptParseRet LeptJsonParser::LeptParseWhiteSpace()
{
	const char *p = m_LeptContext.json;
	while (*p == ' ' || *p == '\t' || *p == '\r' || *p == '\n')
		p++;
	m_LeptContext.json = p;

	return LeptParseRet::LEPT_PASER_OK;
}

LeptParseRet LeptJsonParser::LeptParseValue()
{
	const char* p = m_LeptContext.json;

	switch (*p){
		case 'n':  return LeptParseLiteral("null");  break;
		case 'f':  return LeptParseLiteral("false"); break;
		case 't':  return LeptParseLiteral("true");  break;
		case '\0': return LeptParseRet::LEPT_PARSE_EXPCET_VALUE; break;
		default:   return LeptParseNumber(); break;
	}
}

LeptParseRet LeptJsonParser::LeptParseLiteral(const char * literal)
{
	const char* p = m_LeptContext.json;

	switch (*p) {

		//parse null;
		case 'n': {
			if (p[1] != 'u' || p[2] != 'l' || p[3] != 'l') {
				return LeptParseRet::LEPT_PARSE_INVALID_VALUE;
			}
			else {
				m_LeptContext.json += 4;
				m_LeptValue.type = LeptType::LEPT_NULL;
				return LeptParseRet::LEPT_PASER_OK;
			}
		}
		break;

		//parse false
		case 'f': {
			if (p[1] != 'a' || p[2] != 'l' || p[3] != 's' || p[4] != 'e') {
				return LeptParseRet::LEPT_PARSE_INVALID_VALUE;
			}
			else {
				m_LeptContext.json += 5;
				m_LeptValue.type = LeptType::LEPT_FALSE;
				return LeptParseRet::LEPT_PASER_OK;
			}
		}
		break;

		//parse true
		case 't': {
			if (p[1] != 'r' || p[2] != 'u' || p[3] != 'e') {
				return LeptParseRet::LEPT_PARSE_INVALID_VALUE;
			}
			else {
				m_LeptContext.json += 4;
				m_LeptValue.type = LeptType::LEPT_TRUE;
				return LeptParseRet::LEPT_PASER_OK;
			}
		}
		break;
		
		//invalid literal
		default:
			return LeptParseRet::LEPT_PARSE_INVALID_VALUE;
		break;

	}

}

LeptParseRet LeptJsonParser::LeptParseNumber()
{
	/*
	json数字的构成
	number = [ "-" ] int [ frac ] [ exp ]
	其中
	(0)负号可选
	(1)int = "0" / digit1-9 *digit
	(2)frac = "." 1*digit
	(3)exp = ("e" / "E") ["-" / "+"] 1*digit
	*/

	const char* p = m_LeptContext.json;
	/*(0)校验负号*/
	if (*p == '-') p++;

	/*(1)校验int, 两种情况,*/
	if (*p == '0') p++;

	else {

		if (!IsDigit1To9(*p)) {
			return LeptParseRet::LEPT_PARSE_INVALID_VALUE;
		}
		else {
			for (p++; IsDigit(*p); p++);
		}
	}

	/*(2)校验小数点*/
	if (*p == '.') {

		//小数点后至少一位是数字
		p++;

		if (!IsDigit(*p)) {
			return LeptParseRet::LEPT_PARSE_INVALID_VALUE;
		}
		else{
			for (p++; IsDigit(*p); p++);
		}
	
	}

	/*(3)校验指数部分*/
	if (*p == 'e' || *p == 'E') {

		p++;

		//跳过'e'或者'E'后,后面可以有正负号,检验
		/*(3.1)校验跳过E或者e后,是否有正负号*/
		if (*p == '+' || *p == '-') p++;

		if (!IsDigit(*p)) { 
			return LeptParseRet::LEPT_PARSE_INVALID_VALUE; 
		}
		else {
			/*(3.2)校验跳过了正负号后后面是否都为数字*/
			for (p++; IsDigit(*p); p++);
		}

	}
	//经过上述处理,json指向第一个不为数字的字符

	errno = 0;

	m_LeptValue.number = strtod(m_LeptContext.json, NULL);
	

	if (errno == ERANGE && (m_LeptValue.number == HUGE_VAL || m_LeptValue.number == -HUGE_VAL))

		return LeptParseRet::LEPT_PARSE_NUMBER_TOO_BIG;

	m_LeptValue.type = LeptType::LEPT_NUMBRE;

	m_LeptContext.json = p;

	return LeptParseRet::LEPT_PASER_OK;


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



LeptParseRet LeptJsonParser::Parse()
{
	//初始化为null类型
	m_LeptValue.type = LeptType::LEPT_NULL;

	LeptParseRet ret;

	//json-text = whitespace text whitespace
	//1.去掉开头部分空格whitespace
	LeptParseWhiteSpace();
	//2.如果解析json的text成功那么去掉后面的空格
	ret = LeptParseValue();
	if (ret == LeptParseRet::LEPT_PASER_OK) {
		LeptParseWhiteSpace();
		//3.如果解析空格完毕后不是字符串的结束,说明json的解析树有两个根
		if ((*m_LeptContext.json) != '\0') {
			return LeptParseRet::LEPT_PARSE_ROOT_NOT_SINGULAR;
		}
		else {
			return LeptParseRet::LEPT_PASER_OK;
		}
		
	}
	else {
		return ret;
	}
}
