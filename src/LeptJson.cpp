#include "LeptJson.h"

#include <errno.h>
#include <math.h>
#include <stdlib.h>

LeptJsonParser::LeptJsonParser(const LeptContext & context)
{
	m_LeptContext.Init();
	m_LeptValue.Init();
	m_LeptContext = context;
}

void LeptJsonParser::SkipWhiteSpace(LeptContext& context)
{
	const char *p = context.json;
	while (*p == ' ' || *p == '\t' || *p == '\r' || *p == '\n')
		p++;
	context.json = p;
}

LeptParseRet LeptJsonParser::LeptParseValue(LeptContext& context, LeptValue& value)
{
	
	switch (*context.json){
		case 'n':   return LeptParseLiteral(context, "null", value);  break;
		case 'f':   return LeptParseLiteral(context, "false", value); break;
		case 't':   return LeptParseLiteral(context, "true", value);  break;
		case '\"':  return LeptParseString(context, value); break;
		case '[':   return LeptParseArray(context, value); break;
		case '\0':  return LeptParseRet::LEPT_PARSE_EXPCET_VALUE; break;
		default:    return LeptParseNumber(context, value); break;
	}
}

LeptParseRet LeptJsonParser::LeptParseLiteral(LeptContext& context, const char * literal, LeptValue& value)
{
	const char* p = context.json;

	//--------------------------------------------------------待注释----------------------------------------------------------//
	//m_LeptValue
	//--------------------------------------------------------待注释----------------------------------------------------------//

	switch (*p) {

		//parse null;
		case 'n': {
			if (p[1] != 'u' || p[2] != 'l' || p[3] != 'l') {
				return LeptParseRet::LEPT_PARSE_INVALID_VALUE;
			}
			else {
				context.json += 4;
				m_LeptValue.type = LeptType::LEPT_NULL;
				m_LeptValue.constliteral = "null";
				value = m_LeptValue;
				return LeptParseRet::LEPT_PARSE_OK;
			}
		}
		break;

		//parse false
		case 'f': {
			if (p[1] != 'a' || p[2] != 'l' || p[3] != 's' || p[4] != 'e') {
				return LeptParseRet::LEPT_PARSE_INVALID_VALUE;
			}
			else {
				context.json += 5;
				m_LeptValue.type = LeptType::LEPT_FALSE;
				m_LeptValue.constliteral = "false";
				value = m_LeptValue;
				return LeptParseRet::LEPT_PARSE_OK;
			}
		}
		break;

		//parse true
		case 't': {
			if (p[1] != 'r' || p[2] != 'u' || p[3] != 'e') {
				return LeptParseRet::LEPT_PARSE_INVALID_VALUE;
			}
			else {
				context.json += 4;
				m_LeptValue.type = LeptType::LEPT_TRUE;
				m_LeptValue.constliteral = "true";
				value = m_LeptValue;
				return LeptParseRet::LEPT_PARSE_OK;
			}
		}
		break;
		
		//invalid literal
		default:
			return LeptParseRet::LEPT_PARSE_INVALID_VALUE;
		break;

	}

}

LeptParseRet LeptJsonParser::LeptParseNumber(LeptContext& context, LeptValue& value)
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

	const char* p = context.json;
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

	double number = strtod(m_LeptContext.json, NULL);

	if (errno == ERANGE && (number == HUGE_VAL || number == -HUGE_VAL))
		return LeptParseRet::LEPT_PARSE_NUMBER_TOO_BIG;

	//--------------------------------------------------------待注释----------------------------------------------------------//
	m_LeptValue.number = number;

	m_LeptValue.type = LeptType::LEPT_NUMBRE;
	//--------------------------------------------------------待注释----------------------------------------------------------//

	value = m_LeptValue;

	context.json = p;

	return LeptParseRet::LEPT_PARSE_OK;


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

LeptParseRet LeptJsonParser::LeptParseString(LeptContext& context, LeptValue& value)
{
	//json字符串的格式如下
	// " text | escape-char ",两边是双引号,中间可能有转义字符

	//字符串开头必须是双引号
	assert(*context.json == '\"');
	context.json++;

	int32_t head = context.stack.top;
	const char* p = context.json;

	while (true) {

		char ch = *p++;

		switch (ch){
			//解析到了完整的字符串
			case '\"': {
				//解析的字符串的长度
				int32_t len = context.stack.top - head;
				//解析的字符串的起始地址,弹出已经成功解析的字符
				const char* str = context.stack.Pop(len);
				value.SetString(str, len);
				//--------------------------------------------------------待注释----------------------------------------------------------//
				m_LeptValue.CleanUp();
				m_LeptValue = value;
				//--------------------------------------------------------待注释----------------------------------------------------------//
				//value = m_LeptValue;
				context.json = p;
				return LeptParseRet::LEPT_PARSE_OK;
			}
			break;

			//若碰到转义字符
			case '\\': {

				switch (*p++) {
					//如果是双引号的另一部分
					case '\"':context.stack.PutChar('\"'); break;
					//如果还是转义字符
					case '\\':context.stack.PutChar('\\'); break;
					//如果是反斜杠
					case '/': context.stack.PutChar('/'); break;
					//如果是退后符
					case 'b': context.stack.PutChar('\b'); break;
					//如果是换页符
					case 'f': context.stack.PutChar('\f'); break;
					//如果是换行符
					case 'n': context.stack.PutChar('\n'); break;
					//如果是回车符
					case 'r': context.stack.PutChar('\r'); break;
					//如果是制表符
					case 't': context.stack.PutChar('\t'); break;
					//其他情况是不合法的转义字符
					default:
						context.stack.top = head;
						value.type = LeptType::LEPT_NULL;
						m_LeptValue.type = LeptType::LEPT_NULL;
						return LeptParseRet::LEPT_PARSE_STRING_INVALID_ESCAPE;
					break;

				}

			}
			break;

			//未解析完字符串就碰到了结束标志
			case '\0': {
				context.stack.top = head;
				value.type = LeptType::LEPT_NULL;
				m_LeptValue.type = LeptType::LEPT_NULL;
				return LeptParseRet::LEPT_PARSE_STRING_MISS_QUOTATION_MARK;
			}
			break;

			//put一个字符
			default: {
				if ((unsigned char)ch < 0x20) {
					context.stack.top = head;
					value.type = LeptType::LEPT_NULL;
					m_LeptValue.type = LeptType::LEPT_NULL;
					return LeptParseRet::LEPT_PARSE_STRING_INVALID_CHAR;
				}
				else {
					context.stack.PutChar(ch);
				}
			}
			break;
		}
	}
}

LeptParseRet LeptJsonParser::LeptParseArray(LeptContext& context, LeptValue & value)
{
	//数组必须是以[开头
	assert(*context.json == '[');
	context.json++;
	//数组开头的空格
	SkipWhiteSpace(context);
	
	LeptParseRet ret;
	//LeptArray arrays;
	LeptValue element;
	element.Init();

	//解析到了空数组[]
	if (*context.json == ']') {
		value.Init();
		value.type = LeptType::LEPT_ARRAY;
		//------------------------------------------待注释--------------------------------------------------//
		m_LeptValue = value;
		//------------------------------------------待注释--------------------------------------------------//
		context.json++;
		return LeptParseRet::LEPT_PARSE_OK;
	}

	while (true) {

		element.CleanUp();

		ret = LeptParseValue(context, element);
		if (ret != LeptParseRet::LEPT_PARSE_OK) {
			break;
		}
		else {
			context.PushArrayElement(element);
			//去掉数组后面的空格
			SkipWhiteSpace(context);

			if (*context.json == ',') {
				context.json++;
				SkipWhiteSpace(context);
			}
			else if (*context.json == ']') {
				context.json++;
				value.type = LeptType::LEPT_ARRAY;
				for (size_t i = 0; i < context.valuebuffer.size(); ++i) {
					value.arrays.PushBack(context.valuebuffer[i]);
				}
				context.CleanArrayBuffer();
				//------------------------------------------待注释--------------------------------------------------//
				m_LeptValue = value;
				//------------------------------------------待注释--------------------------------------------------//
				return LeptParseRet::LEPT_PARSE_OK;
			}
			else {
				ret = LeptParseRet::LEPT_PARSE_ARRAY_MISS_COMMA_OR_SQUARE_BRACKET;
				break;
			}

		}

	}

	//执行到这里说明解析失败
	//1.清除在解析过程中产生的临时对象
	context.CleanArrayBuffer();
	//2.回收LeptStackBuffer的内存,否则会造成内存泄漏
	//TODO
	return ret;
}

LeptParseRet LeptJsonParser::Parse()
{
	//初始化为null类型
	m_LeptValue.type = LeptType::LEPT_NULL;
	LeptValue value;
	value.Init();
	//json-text = whitespace text whitespace
	//1.去掉开头部分空格whitespace
	SkipWhiteSpace(m_LeptContext);
	//2.如果解析json的text成功那么去掉后面的空格
	LeptParseRet ret = LeptParseValue(m_LeptContext, value);
	if (ret == LeptParseRet::LEPT_PARSE_OK) {
		SkipWhiteSpace(m_LeptContext);
		//3.如果解析空格完毕后不是字符串的结束,说明json的解析树有两个根
		if ((*m_LeptContext.json) != '\0') {
			//解析失败
			m_LeptContext.CleanUp();
			return LeptParseRet::LEPT_PARSE_ROOT_NOT_SINGULAR;
		}
		else {
			return LeptParseRet::LEPT_PARSE_OK;
		}
		
	}
	else {
		//解析失败
		m_LeptContext.CleanUp();
		return ret;
	}
}
