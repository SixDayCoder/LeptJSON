#include "LeptJson.h"


LeptParseRet LeptJsonParse::LeptParseWhiteSpace(const char * json)
{

	assert(json != nullptr);
	const char *p = json;
	while (*p == ' ' || *p == '\t' || *p == '\r' || *p == '\n')
		p++;
	json = p;

	return LeptParseRet::LEPT_PASER_OK;
}

LeptParseRet LeptJsonParse::LeptParseLiteral(const char * json, const char * literal)
{
	assert(json != nullptr);
	const char* p = json;

	switch (*p) {

		//parse null;
		case 'n': {
			if (p[1] != 'u' || p[2] != 'l' || p[3] != 'l') {
				return LeptParseRet::LEPT_PARSE_INVALID_VALUE;
			}
			else {
				json += 4;
				m_LeptValue.type = LeptType::LEPT_NULL;
				return LeptParseRet::LEPT_PASER_OK;
			}
		}
		break;
		//parse false
		case 'f': {
			if (p[1] != 'a' || p[2] != 'l' || p[3] != 's' || p[3] != 'e') {
				return LeptParseRet::LEPT_PARSE_INVALID_VALUE;
			}
			else {
				json += 5;
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
				json += 4;
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

LeptJsonParse::LeptJsonParse(const char * json)
{
	assert(json != nullptr);

	m_LeptContext.json = json;
}

LeptParseRet LeptJsonParse::Parse()
{


	return LeptParseRet();
}
