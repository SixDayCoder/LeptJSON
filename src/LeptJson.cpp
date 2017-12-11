#include "LeptJson.h"

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
		default:   return LeptParseRet::LEPT_PARSE_INVALID_VALUE; break;
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
